#!/usr/bin/python
# -*- coding:utf-8 -*-
import getopt
import commands
import sys
from sys import argv
import os
import json
import re
import err as Err
from common import *

# raid_dic = load_raid_dic("raid_dic_path")
dest_path = '/opt/raid/'
raid_dic_path = '/opt/raid/raid_file'
conf_post = '.json'
max_md_num = 2000

tmp_scan_res_path = "/tmp/md_tmp_res"

def update_md_detail():
    commands.getoutput(
        "/usr/sbin/mdadm --detail --scan --verbose | /usr/bin/grep ARRAY | /usr/bin/awk '{print $2}' > " + tmp_scan_res_path)

def linkpath(name):
    return commands.getoutput("/usr/bin/cat " + tmp_scan_res_path + " | /usr/bin/grep " + name)

def devpath_by_name(name, orgpath):
    lpath = linkpath(name)
    if lpath == "": return orgpath
    return "/dev/" + commands.getoutput(
        "/usr/bin/ls -l \"" + lpath + "\" | /usr/bin/cut -d '>' -f2 | /usr/bin/cut -d '/' -f2")

def update_raid_info_by_scan(dic):
    res = {}
    for key, value in dic.items():
        newpath = devpath_by_name(key, value)
        res.update({key: newpath})
    return res

if os.path.exists(raid_dic_path):
    raid_dic = json.loads(commands.getoutput("/usr/bin/cat " + raid_dic_path))
else:
    raid_dic = {}

def disk_name(slot):
    # FIXME
    if (slot.find("/dev/sd") != -1 or slot.find("/dev/vg") != -1):
        return slot
    return None


def disks_from_slot(slots):
    devs = []
    failed_slot = []
    slot_list = slots.split(",")
    for slot in slot_list:
        disk = disk_name(slot)
        if (disk):
            devs.append(disk)
        else:
            failed_slot.append(slot)
    return devs, failed_slot


def md_list_mddevs():
    dev_list = []
    try:
        for dev in os.listdir('/dev'):
            if (dev.find('md') == 0) and (len(dev.split('p')) == 1) and (
                    len(dev) > 2):
                dev_list.append('/dev/' + dev)
    except:
        pass
    finally:
        return dev_list


def md_find_free_mddev():
    mddevs = md_list_mddevs()
    for i in xrange(0, 255):
        md = "/dev/md%u" % i
        if not (md in mddevs):
            return md
    return None


def raid_list():
    global raid_dic
    raid_list = []
    for i in raid_dic.keys():
        raid_list.append(i)
    return raid_list


def raid_create(chunk, level, raid_name, devs_input):
    if raid_name in raid_list():
        return False, "raid 已存在"
    mddev = md_find_free_mddev()
    if mddev == None:
        return False, "没有空闲的RAID槽位"

    devs, failed = disks_from_slot(devs_input)
    if len(devs) == 0:
        return False, "没有可用磁盘"
    dev_list = " ".join(devs)

    cmd = "/usr/sbin/mdadm -CR %s -l %s -c %s -n %u %s --metadata=1.2 --homehost=%s -f" % (
        mddev, level, chunk, len(devs), dev_list, raid_name)

    if level in ('3', '4', '5', '6', '10', '50', '60'):
        cmd += " --bitmap=internal"
    sts, out = commands.getstatusoutput(cmd)

    if sts < 0 or out.find('started') < 0:
        return False, "创建raid失败"

    global raid_dic
    raid_dic.update({raid_name: mddev})
    save_raid_dic(raid_dic_path, raid_dic)
    save_raid_dic(dest_path + raid_name + conf_post, json.dumps(devs))
    # os.makedirs(mddev)
    return True, '创建raid %s 成功!' % raid_name


class raid_attr:
    def __init__(self):
        self.Name = ''  # raid1需要特殊处理
        self.dev = ''
        self.raid_level = ''
        self.raid_state = ''  # raid1需要根据disk_cnt与disk_specs关系计算
        self.raid_strip = ''  # raid1需要特殊处理
        self.raid_rebuild = ''
        self.capacity = 0
        self.remain = 0
        self.disk_cnt = 0  # 当前磁盘个数, raid1需要计算实际的disk_list
        self.disk_list = []  # 当前磁盘列表, raid1需要特殊处理
        self.raid_uuid = ''  # 供磁盘上下线检测对应RAID使用, raid1需要特殊处理
        # self.disk_working = 0  # 考虑使用disk_cnt替代
        self.disk_specs = 0  # raid应该包含的磁盘个数，对应mdadm -D的'Raid Devices'字段, raid1需要特殊处理
        self.is_parted = False  # 是否映射用户数据卷
        self.vg_name = ''


def __get_sys_attr(dev, attr):
    if dev.find('/dev/md') >= 0:
        dev_name = dev.split('/dev/')[-1]
    else:
        dev_name = dev
    sys_path = '/sys/block/' + dev_name
    return __attr_read(sys_path, attr)


def __def_post(p):
    if len(p) == 0:
        return ""
    return p[0]


def find_attr(output, reg, post=__def_post):
    p = re.findall(reg, output)
    return post(p)


def __level_post(p):
    if len(p) == 0:
        return "None"
    level = p[0].lower().replace("raid", "")
    if level == "linear":
        level = "JBOD"
    return level


def __chunk_post(p):
    if len(p) == 0:
        return ""
    chunk = p[0].lower().replace("k", "")
    return chunk


def __state_post(p):
    if len(p) == 0:
        return "Unknown"
    state = p[0]
    if state.find("degraded") != -1:
        if state.find("recovering") != -1:
            return "rebuild"
        return "degrade"
    elif state.find("FAILED") != -1:
        return "fail"
    elif state.find("resyncing") != -1:
        return "initial"
    else:
        return "normal"


def __name_post(p):
    return p[0].split(":")[0] if len(p) > 0 else 'Unknown'


def __attr_read(path, attr):
    content = ''
    try:
        attr_path = path + os.sep + attr
        f = open(attr_path)
        content = f.readline().strip()
        f.close()
    except:
        pass
    return content


#   def disk_slot(name):
#       cmd = "us_cmd disk slot " + name + " 2>/dev/null"
#       out = commands.getoutput(cmd)
#      if (out.find(":") == -1):
#           return None
#       return out


def disk_post(p):
    if len(p) == 0:
        return ();
    slots = []
    for disk in p:
        name = disk[1]
        slots.append(name)
    return slots


def __md_fill_mdadm_attr(mddev, remain=True):
    cmd = '/usr/sbin/mdadm -D %s 2>/dev/null' % mddev
    sts, output = commands.getstatusoutput(cmd)

    if sts != 0:
        return None

    attr = raid_attr()
    attr.Name = find_attr(output, "Name : (.*)", __name_post)
    attr.dev = find_attr(output, "^(.*):")
    attr.raid_level = find_attr(output, "Raid Level : (.*)", __level_post)
    attr.raid_state = find_attr(output, "State : (.*)", __state_post)
    attr.raid_strip = find_attr(output, "Chunk Size : ([0-9]+[KMG])",
                                __chunk_post)
    rebuild_per = find_attr(output, "Rebuild Status : ([0-9]+)\%")
    resync_per = find_attr(output, "Resync Status : ([0-9]+)\%")

    if rebuild_per:
        attr.raid_rebuild = rebuild_per
    elif resync_per:
        attr.raid_rebuild = resync_per
    else:
        attr.raid_rebuild = '0'

    attr.capacity = int(__get_sys_attr(attr.dev, "size")) * 512
    if remain:
        attr.remain = attr.capacity
        # fixme  __get_remain_capacity(attr.name)
    attr.disk_list = find_attr(output, "([0-9]+\s*){4}.*(/dev/.+)", disk_post)
    attr.disk_cnt = len(attr.disk_list)
    attr.raid_uuid = find_attr(output, "UUID : (.*)")
    attr.disk_specs = int(find_attr(output, "Raid Devices : ([0-9]+)"))
    if attr.capacity != attr.remain:
        attr.is_parted = True

    cmd = "pvs |/usr/bin/grep '%s'" % mddev
    sts, output = commands.getstatusoutput(cmd)
    if sts == 0:
        cmd = "pvs|/usr/bin/grep '%s'|/usr/bin/awk '{print $2}'" % mddev
        attr.vg_name = commands.getoutput(cmd)

    return attr


def __raid0_jobd_state(dspecs, dcnt):
    return 'normal' if dcnt == dspecs else 'fail'


# 不同RAID级别在磁盘完全掉线后会导致部分信息缺失需要记录在tmpfs供查询使用
def __md_fill_tmpfs_attr(attr=raid_attr()):
    if attr.raid_level == '6':
        if attr.raid_state == 'degrade' or attr.raid_state == 'rebuild':
            if attr.disk_cnt < attr.disk_specs:
                attr.raid_state = 'degrade'
            else:
                attr.raid_state = 'rebuild'
        return attr.__dict__

    if attr.raid_level == '5':
        return attr.__dict__

    # 特殊处理: raid1,jbod没有strip属性
    if attr.raid_level == '1' or attr.raid_level == 'JBOD':
        attr.raid_strip = 'N/A'
        if attr.disk_cnt == 0:
            attr.raid_state = 'fail'

    # raid 0,1,jobd的磁盘列表需要单独处理
    # attr.disk_list = __listdir_files('%s/disk-list' % _dir)
    # attr.disk_cnt = len(attr.disk_list)

    # raid 0,1,jbod的状态在掉盘后需要手动判断
    if attr.raid_level == '0' or attr.raid_level == 'JBOD':
        attr.raid_state = __raid0_jobd_state(attr.disk_specs, attr.disk_cnt)

    return attr.__dict__


def mddev_get_attr(mddev, remain=True):
    attr = __md_fill_mdadm_attr(mddev, remain)
    if None == attr:
        return None
    return __md_fill_tmpfs_attr(attr)


def list_files(path, reg):
    if not path.endswith("/"):
        path += "/"

    r = re.compile(reg)
    names = os.listdir(path)
    f = [path + x for x in names if r.match(x)]
    return f

def update_raidfile():
    global raid_dic
    mddevs = list_files("/dev", "md[0-255]+")
    for md in mddevs:
        if md.find('p') >= 0:
            continue
        cmd = "/usr/sbin/mdadm -D %s 2>&1 | /usr/bin/grep 'Name :' | /usr/bin/awk '{print $3}' |  /usr/bin/cut -d ':' -f1" % (md)
        sts, raidname = commands.getstatusoutput(cmd)
        if(sts != 0):
            continue
        if raidname in raid_dic.keys():
            raid_dic[raidname] = md
    save_raid_dic(raid_dic_path, raid_dic)

def md_get_mddev_raw(raid_name):
    mddevs = list_files("/dev", "md[0-255]+")
    for md in mddevs:
        if md.find('p') >= 0:
            continue
        # 尝试从mdadm获取信息
        cmd = '/usr/sbin/mdadm -D %s 2>&1 | /usr/bin/grep %s >/dev/null' % (md, raid_name)
        sts, out = commands.getstatusoutput(cmd)
        if sts == 0:
            return md
    return None

def is_md_raid_legal(raid_name, mddev):
    cmd = '/usr/sbin/mdadm -D %s 2>&1 | /usr/bin/grep %s >/dev/null' % (mddev, raid_name)
    sts, out = commands.getstatusoutput(cmd)
    if(sts == 0):
        return True
    return False

def md_get_mddev(raid_name):
    md = md_get_mddev_raw(raid_name)
    if(md == None):
        update_raidfile()
        return md_get_mddev_raw(raid_name)
    return md

def md_info_mddevs(mddevs=None, remain=True):
    if (mddevs == None):
        mddevs = md_list_mddevs()
    md_attrs = []
    for mddev in mddevs:
        attr = mddev_get_attr(mddev, remain)
        if (attr):
            md_attrs.append(attr)
    return True, {"total": len(md_attrs), "raids": md_attrs}


def md_info(raid_name=None, remain=True):
    update_md_detail()
    global raid_dic
    raid_dic = update_raid_info_by_scan(raid_dic)
    if (raid_name == None):
        mddevs = None
    else:
        mddevs = [md_get_mddev(raid_name)];
    return md_info_mddevs(mddevs, remain);


def mddev_get_disks(mddev):
    # reg = re.compile(r"(sd[a-z]+)\[[0-9]+\]")
    reg = re.compile(r"(sd\w+)")
    cmd = "/usr/bin/cat /proc/mdstat |/usr/bin/grep \'" + (
        os.path.basename(mddev)) + " \' 2>/dev/null"
    sts, out = commands.getstatusoutput(cmd)
    if sts != 0:
        return []
    disks = reg.findall(out)
    rdisks = ["/dev/" + x for x in disks]

    return rdisks


def md_stop(mddev):
    cmd = "/usr/sbin/mdadm -S %s 2>&1" % mddev
    sts, out = commands.getstatusoutput(cmd)
    if out.find('mdadm: stopped') < 0:
        return -1, '设备正在被占用!'
    cmd = "rm -f %s 2>&1" % mddev
    sts, out = commands.getstatusoutput(cmd)
    if sts != 0:
        return -1, '无法删除设备节点! %s' %(out)
    return sts, '%s' % (out)


def set_disk_free(diskname):
    cmd = "/usr/sbin/mdadm --zero-superblock %s 2>&1" % diskname
    sts, out = commands.getstatusoutput(cmd)

    # 修改判断清空superblock的条件
    # 常见的出错提示
    #   mdadm: Unrecognised md component device - /dev/sdb
    #   mdadm: Unrecognised md component device - /dev/sdg
    # 以上出错mdadm返回值均为0
    if len(out) == 0:
        return True
    else:
        return False


def set_disks_free(disks):
    res = ""
    for dev in disks:
        err = set_disk_free(dev)
        if err == False:
            res += " " + dev
    # disk_name_update(disks)
    return res


def md_del(raid_name):
    if raid_name not in raid_list():
        return False, "raid不存在"

    try:
        mdinfo = md_info(raid_name)['raids'][0]
        md_uuid = mdinfo['raid_uuid']
        md_state = mdinfo['raid_state']
    except:
        md_uuid = ''
        md_state = 'fail'

    # FIXME if raid uesd by vol
    # if md_state != 'fail' and __md_used(mdname):
    # return False, '卷组 %s 存在未删除的用户数据卷，请先删除！' % mdname
    global raid_dic
    mddev = raid_dic[raid_name]
    disks = mddev_get_disks(mddev)
    sts, msg = md_stop(mddev)
    if sts != 0:
        return False, "停止%s失败!%s" % (raid_name, msg)
    # 删除设备节点
    # __md_remove_devnode(raid_name)

    # tmpfs_remove_md_info(raid_name)

    # 专用热备盘设置为空闲盘
    # for slot in md_get_special(md_uuid):
    #     disk_set_type(slot, 'Free')
    res = set_disks_free(disks)
    if res != "":
        return False, "清除磁盘信息失败，请手动清除"

    del (raid_dic[raid_name])
    save_raid_dic(raid_dic_path, raid_dic)
    cmd = "rm -f %s" % (dest_path + raid_name + conf_post)
    os.system(cmd)
    # os.remove(mddev)
    # sysmon_event('vg', 'remove', '%s,disks=%s' % (mdinfo['name'],_disk_slot_list_str(disks)), '卷组 %s 删除成功!' % mdinfo['name'])
    return True, "删除raid成功"


def stopraid(raid_name):
    if raid_name not in raid_list():
        return False, "raid不存在"

    try:
        mdinfo = md_info(raid_name)['raids'][0]
        md_uuid = mdinfo['raid_uuid']
        md_state = mdinfo['raid_state']
    except:
        md_uuid = ''
        md_state = 'fail'

    # FIXME if raid uesd by vol
    # if md_state != 'fail' and __md_used(mdname):
    # return False, '卷组 %s 存在未删除的用户数据卷，请先删除！' % mdname
    global raid_dic
    mddev = raid_dic[raid_name]
    disks = mddev_get_disks(mddev)
    sts, msg = md_stop(mddev)
    if sts != 0:
        return False, "停止%s失败!%s" % (raid_name, msg)

    return True, "停止raid成功"

def raidinfo_by_raidname(raid_name):
    remain = True
    if raid_name not in raid_list():
        return False, "raid不存在"

    global raid_dic
    mddev = raid_dic[raid_name]
    if(not is_md_raid_legal(raid_name, mddev)):
        mddev = md_get_mddev(raid_name)
    if(not is_md_raid_legal(raid_name, mddev)):
        return False,None
    attr = mddev_get_attr(mddev, remain)

    return True, attr


def is_disk_mount(dev_path):
    status, mount_info = commands.getstatusoutput(
        "/usr/bin/sudo /usr/bin/mount | /usr/bin/grep " + dev_path)
    if status != 0: return False
    return True


class dev_attr:
    def __init__(self):
        self.name = ''
        self.size = ''
        self.raid_state = ''


def list_devs():
    devs = []
    cmd = "/usr/bin/ls /sys/block/ | /usr/bin/grep sd 2>/dev/null"
    sts, out = commands.getstatusoutput(cmd)
    if sts != 0:
        return False, "devs is unavailable "

    out = out.split("\n")
    unmount_disks = ["/dev/" + x for x in out]
    for x in unmount_disks:
        if not is_disk_mount(x):
            devs.append(x)
    return devs


def get_dev_attr(dev):
    attr = dev_attr()
    attr.name = dev
    attr.raid_state = "no"
    cmd = "/usr/bin/cat /sys/block/%s/size" % dev[5:]
    sts, out = commands.getstatusoutput(cmd)
    attr.size = out

    md_list = md_list_mddevs()
    for md in md_list:
        if dev in mddev_get_disks(md):
            attr.raid_state = md
            break
    return attr.__dict__


def dev_list():
    dev_attrs = []
    for dev in list_devs():
        dev_attrs.append(get_dev_attr(dev))
    return True, {"total": len(dev_attrs), "devs": dev_attrs}


def sethotspare(raid_name, dev):
    global raid_dic
    mddev = raid_dic[raid_name]
    cmd = "/usr/sbin/mdadm %s -a %s" % (mddev, dev)
    sts, out = commands.getstatusoutput(cmd)
    if sts != 0:
        return False, "sethostspare fail!"
    return True, "设置热备盘成功!"


def is_md_exist(path):
    return os.path.exists(path)


def mdpath(num):
    return "/dev/md" + str(num)


def find_empty_mdpath():
    i = 127
    while (i < max_md_num):
        if (not is_md_exist(mdpath(i))):
            return mdpath(i)
        i = i + 1
    raise


def regroupraid(raidname):
    disks = json.loads(load_raid_dic(dest_path + raidname + conf_post))
    mddev = find_empty_mdpath()
    raid_dic[raidname] = mddev
    dev_list = " ".join(disks)
    cmd = "/usr/sbin/mdadm -Af %s %s 2>&1" % (mddev, dev_list)
    sts, out = commands.getstatusoutput(cmd)
    if sts != 0:
        return False, "重组%s失败!%s" % (raidname, out)
    save_raid_dic(raid_dic_path, raid_dic)
    return True, "重组成功, %s" % (out)


def scan_raid():
    global raid_dic
    # raid_name,cur_raid_name,dev,stop
    for raid in raid_dic.keys():
        success, msg = regroupraid(raid)
        if not success:
            return success, msg
    return True, "重组成功"
