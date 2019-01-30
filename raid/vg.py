#!/usr/bin/python
# -*- coding:utf-8 -*-
from raid import *
import commands

class vg_attr:
    def __init__(self):
        self.Name = ''
        self.size_total = ''
        self.size_free = ''
        self.pv_num = 0
        self.lv_num = 0

def get_vg_attr(vg_name):
	attr = vg_attr()
	cmd = "vgs|grep %s |awk '{print $1}'" %vg_name
	attr.Name = commands.getoutput(cmd)

	cmd = "vgs|grep %s |awk '{print $2}'" %vg_name
	attr.pv_num = int(commands.getoutput(cmd))

	cmd = "vgs|grep %s |awk '{print $3}'" %vg_name
	attr.lv_num = int(commands.getoutput(cmd))

	cmd = "vgs|grep %s |awk '{print $6}'" %vg_name
	attr.size_total = commands.getoutput(cmd)

	cmd = "vgs|grep %s |awk '{print $7}'" %vg_name
	attr.size_free = commands.getoutput(cmd)

	return attr.__dict__

def list_vgs():
    vgs_list = []
    cmd = "vgs"
    sts,out = commands.getstatusoutput(cmd)
    length = int(commands.getoutput("vgs|wc -l"))
    for i in range(2, length+1):
        cmd = "vgs|sed -n %dp |awk '{print $1}'" %i
        name = commands.getoutput(cmd)
        if is_disk_mount(name):
            continue;
        vgs_list.append(name)
    return vgs_list

def vg_info():    
    vg_attrs = []
    for vg in list_vgs():    
		vg_attrs.append(get_vg_attr(vg))
    return  True,{"total": len(vg_attrs), "vgs": vg_attrs}

def vginfo_by_vgname(vg_name):
    if vg_name not in list_vgs():
        return False,"卷组不存在"

    attr = get_vg_attr(vg_name)
    return True,attr

class vol_attr:
    def __init__(self):
        self.Name = ''
        self.size = ''
        self.vg_name = ''

def list_vols(vg_name):
    vols_list = []
    cmd = "lvs|grep %s" %vg_name
    sts,out = commands.getstatusoutput(cmd)

    cmd = "lvs|grep %s|wc -l" %vg_name
    length = int(commands.getoutput(cmd))
    for i in range(1, length+1):
        cmd = "lvs|grep %s|sed -n %dp |awk '{print $1}'" %(vg_name,i)
        name = commands.getoutput(cmd)
        vols_list.append(name)
    return vols_list

def get_vol_attr(vg_name, vol_name):
    attr = vol_attr()
    attr.Name = vol_name
    attr.vg_name = vg_name
    cmd = "lvs|grep %s |grep %s |awk '{print $4}'" %(vg_name,vol_name)
    attr.size = commands.getoutput(cmd)

    return attr.__dict__

def vol_info(vg_name):
    vol_attrs = []
    for vol in list_vols(vg_name):    
        vol_attrs.append(get_vol_attr(vg_name, vol))
    return  True,{"total": len(vol_attrs), "vols": vol_attrs}

def volinfo_by_volname(vg_name, vol_name):
    if vol_name not in list_vols(vg_name):
        return False,"数据卷不存在"

    attr = get_vol_attr(vg_name, vol_name)
    return True,attr

class fs_attr:
    def __init__(self):
        self.dev = ''
        self.type = ''
        self.size_total = ''
        self.size_used = ''
        self.mount_dir = ''

def listfs():
    cmd = "df -hT|grep -v tmpfs|grep -v boot|wc -l"
    length = int(commands.getoutput(cmd))
    mount_list = []
    for i in range(2, length+1):
        cmd = "df -hT|grep -v tmpfs|grep -v boot|sed -n %dp |awk '{print $7}'" %i
        mount_dir = commands.getoutput(cmd)
        if mount_dir != "/":
            mount_list.append(mount_dir)

    return mount_list

def get_mount_attr(mount_dir):
    attr = fs_attr()
    cmd = "df -hT |grep %s|grep -v %s/|awk '{print $1}'" %(mount_dir,mount_dir)
    attr.dev = commands.getoutput(cmd)

    cmd = "df -hT |grep %s|grep -v %s/|awk '{print $2}'" %(mount_dir,mount_dir)
    attr.type = commands.getoutput(cmd)

    cmd = "df -hT |grep %s|grep -v %s/|awk '{print $3}'" %(mount_dir,mount_dir)
    attr.size_total = commands.getoutput(cmd)

    cmd = "df -hT |grep %s|grep -v %s/|awk '{print $4}'" %(mount_dir,mount_dir)
    attr.size_used = commands.getoutput(cmd)

    attr.mount_dir = mount_dir

    return attr.__dict__

def fd_info():
    fs_attrs = []
    for fs in listfs():    
        fs_attrs.append(get_mount_attr(fs))
    return  True,{"total": len(fs_attrs), "fs": fs_attrs}

def fsinfo_by_mountname(mount_name):
    if mount_name not in listfs():
        return False,"该目录未挂载文件系统"

    attr = get_mount_attr(mount_name)
    return True,attr

