#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
from sys import argv
from raid import *
from vg import *
import err as Err

color_red='\033[1;31;40m'
color_green='\033[1;32;40m'
color_yellow='\033[1;33;40m'
color_blue='\033[1;34;40m'
color_purple='\033[1;35;40m'
color_cyan='\033[1;36;40m'
color_reset='\033[0m'

raid_dic = {}

def help(arg):
	print color_green
	print "----------menu--------------------"
	print color_yellow
	print "help                                                            ; show this"
	print color_green
	print "listdev                                                         ; list all dev info"
	print "createraid <trunk_sec> <raid_level> <raid_name> <dev1>,...[devn]; create raid"
	print "raidinfo <raid_name>                                            ; show raid info by raid_name"
	print "listraid                                                        ; show all raid info"
	print "scanraid                                                        ; scan disk and regroup all raid"
	print "regroupraid <raid_name>                                         ; regroup raid by raid name"
	print color_blue
	print "stopraid <raid_name>                                            ; stop raid by raid name"
	print "removeraid <raid_name>                                          ; remove raid by raid name"
	print "sethotspare <raid_name> <dev>                                   ; set disk into some raid"
	print "createvg <vg_name> <raid1/dev1>,...<raidn/devn>                 ; create vg on raid/dev"
	print "vginfo <vg_name>                                                ; show vg info"
	print "listvg                                                          ; show all vg info"
	print color_purple
	print "removevg <vg_name>                                              ; remove vg"
	print "createvol <vg_name> <vol_name> <vol_size>                       ; create vol on vg"
	print "volinfo <vg_name> <vol_name>                                    ; show vol info"
	print "listvol <vg_name>                                               ; show all vol info of one vg"
	print "removevol <vg_name> <vol_name>                                  ; remove vol"
	print "createfs <raid/dev/vol> <dir_name> <fs_type>                    ; create fs on raid/dev/vol"
	print color_cyan
	print "fsinfo <dir_name>                                               ; show fs info(dev fs_type etc.)"
	print "listfs                                                          ; show all fs info"
	print "removefs <dir_name>                                             ; remove fs (umount and delete dir)"
	print "list_remain_dev                                                 ; list remain dev to create vg"
	print "list_remain_raid                                                ; list remain raid to create vg"
	print "listvols                                                        ; list all vols"
	print color_reset

def listdev_arg(arg):
	err,output = dev_list();
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def createraid_arg(arg):
	if(len(arg)<4):
		return WRONG_ARG,"";
	raid_name = arg[2]
	err,output = raid_create(arg[0], arg[1], arg[2], arg[3])
	return 0,json_dump(err,output)

def raidinfo_arg(arg):
	err,output = raidinfo_by_raidname(arg[0]);
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def listraid_arg(arg):
	err,output = md_info()
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)
	
def scanraid_arg(arg):
	err,output = scan_raid()
	return 0,json_dump(err,output)

def regroupraid_arg(arg):
	err,output = regroupraid(arg[0])
	return 0,json_dump(err,output)

def removeraid_arg(arg):
	err,output = md_del(arg[0])
	return 0,json_dump(err,output)

def stopraid_arg(arg):
	err,output = stopraid(arg[0])
	return 0,json_dump(err,output)

def sethotspare_arg(arg):
	err,output = sethotspare(arg[0],arg[1])
	return 0,json_dump(err,output)

def createvg_arg(arg):
	raid_or_dev = arg[1].split(",")
	if len(raid_or_dev) == 0:
		return False, "没有指定磁盘"
	mddev_list = []
	raid_dic = json.loads(commands.getoutput("cat /opt/raid/raid_file"))
	for x in raid_or_dev:
		if x in raid_dic.keys():
			mddev_list.append(raid_dic[x])
		else:
			mddev_list.append(x)

	raid_or_dev_list = " ".join(mddev_list)
	cmd = "vgcreate %s %s "%(arg[0],raid_or_dev_list)
	sts,out = commands.getstatusoutput(cmd)
	out = out.replace('\"','')
	if sts != 0:
		return 0,json_dump(False,out)
	return 0,json_dump(True,"创建卷组成功")

def vginfo_arg(arg):
	err,output = vginfo_by_vgname(arg[0]);
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def removevg_arg(arg):	
	cmd = "vgremove %s " % arg[0]
	sts,out = commands.getstatusoutput(cmd)
	out = out.replace('\"','')
	if sts != 0:
		return 0,json_dump(False,out)
	return 0,json_dump(True,"删除卷组 %s 成功") % arg[0]

def scanvg_arg(arg):
	pass

def listvg_arg(arg):
	err,output = vg_info()
	# return 0,json_dump(err,output)
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def createvol_arg(arg):
	# <vg_name> <vol_name> <vol_size> 
	# cmd = "vgs|awk '{print $1}'"
	# out = commands.getoutput(cmd)
	# if arg[1] in out.split('\n'):
	# 	return 0,json_dump(False,"vol already exist")
	
	vol_name = arg[1]
	cmd = "lvcreate -L %s -n %s %s" %(arg[2], vol_name, arg[0])
	sts,out = commands.getstatusoutput(cmd)	
	out = out.replace('\"','')
	if sts != 0:
		return 0,json_dump(False,out)
	return 0,json_dump(True,out)

def volinfo_arg(arg):
	err,output = volinfo_by_volname(arg[0], arg[1]);
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def listvol_arg(arg):
	err,output = vol_info(arg[0])
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def removevol_arg(arg):
	# cmd = "lvs|grep '%s '|awk '{print $2}'" %arg[0]
	# vg_name = commands.getoutput(cmd)
	cmd = "lvremove /dev/%s/%s -f" % (arg[0],arg[1])
	sts,out = commands.getstatusoutput(cmd)
	out = out.replace('\"','')
	if sts != 0:
		return 0,json_dump(False,out)
	return 0,json_dump(True,"删除数据卷 %s 成功") % arg[0]

def createfs_arg(arg):
	#<raid/dev/vol> <dir_name> <fs_type>
	name = arg[0]
	raid_dic = json.loads(commands.getoutput("cat /opt/raid/raid_file"))
	if name in raid_dic.keys():
		name = raid_dic[name]
	if not os.path.exists(arg[0]):
		return 0,json_dump(False,"磁盘 %s 不存在") % arg[0]
	
	cmd = "echo y |mkfs -t %s %s" % (arg[2],name)
	sts,out = commands.getstatusoutput(cmd)
	if sts != 0 : 
		return 0,json_dump(False,"设备 %s 正在被占用，无法创建文件系统!") % arg[0]

	cmd = 'mount | grep "%s "' % arg[1]
	err,out = commands.getstatusoutput(cmd)
	if err == 0:
		return 0,json_dump(False,"目录 %s 已挂载文件系统") % arg[1]
	
	if not os.path.exists(arg[1]):
		cmd = "mkdir -p %s" % arg[1]
		err,out = commands.getstatusoutput(cmd)

	cmd = "mount %s %s" % (name,arg[1])
	err,out = commands.getstatusoutput(cmd)
	return 0,json_dump(True,"%s is mounted") % arg[0]

def fsinfo_arg(arg):
	#<dir_name>
	err,output = fsinfo_by_mountname(arg[0]);
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def listfs_arg(arg):
	err,output = fd_info()
	out = json_dump(err,output)
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def removefs_arg(arg):
	#<dir_name>
	if arg[0] not in listfs():
		return 0,json_dump(False,"文件系统不存在")

	cmd = "df -hT |grep %s|grep -v %s/|awk '{print $1}'" %(arg[0],arg[0])
	dev = commands.getoutput(cmd)
	cmd = "umount %s" % dev
	sts,out = commands.getstatusoutput(cmd)
	if sts != 0:
		return 0,json_dump(False,out)
	cmd = "rm -rf %s" %arg[0]
	os.system(cmd)
	return 0,json_dump(True,"%s is umounted, %s is deleted") %(dev,arg[0])

def list_remain_dev_arg(arg):
	dev_list = list_devs()
	mddev_list = md_list_mddevs()
		
	for md in mddev_list:
		cmd = 'mdadm -D %s 2>/dev/null' % md
		sts,output = commands.getstatusoutput(cmd)
		disk_list = find_attr(output, "([0-9]+\s*){4}.*(/dev/.+)", disk_post)
		for i in disk_list:
			dev_list.remove(i)
			
	for i in dev_list:
		cmd = "pvs |grep %s" %i
		sts,out = commands.getstatusoutput(cmd)
		if sts == 0:
			dev_list.remove(i)


	dev_attrs = []
	for dev in dev_list:    
		dev_attrs.append(get_dev_attr(dev))
	return 0,json_dump(True,{"total": len(dev_attrs), "devs": dev_attrs})


def list_remain_raid_arg(arg):
	mddev_list = md_list_mddevs()
	for i in mddev_list:
		cmd = "pvs |grep %s" %i
		sts,out = commands.getstatusoutput(cmd)
		if sts == 0:
			mddev_list.remove(i)

	md_attrs = []
	for mddev in mddev_list:
		attr = mddev_get_attr(mddev, True)
		if (attr):
			md_attrs.append(attr)
	return 0,json_dump(True,{"total": len(md_attrs), "raids": md_attrs})

def listvols_arg(arg):
	cmd = "lvscan|awk '{print $2}'"
	sts,out = commands.getstatusoutput(cmd)
	all_vols = []
	if out != '':
		all_vols = out.split("\n")
	vol_attrs = []
	for i in all_vols:
		vol_attrs.append(get_vol_attr(i.split("/")[2],i.split("/")[3][:-1]))
	out = json_dump(True,{"total": len(vol_attrs), "vols": vol_attrs})
	return 0,json.dumps(json.loads(out), ensure_ascii=False, sort_keys=True, indent = 4)

def json_dump(ret, msg): 
	ret_msg = {'status':True, 'msg':''}
	ret_msg['status'] = ret
	ret_msg['msg'] = msg
	#return  ret_msg
	return json.dumps(ret_msg, encoding="UTF-8", ensure_ascii=False)
	
cmd_tab = {"help":help,
		"listdev":listdev_arg,
		"createraid":createraid_arg,
		"raidinfo":raidinfo_arg,
		"listraid":listraid_arg,
		"scanraid":scanraid_arg,
		"regroupraid":regroupraid_arg,
		"removeraid":removeraid_arg,
		"stopraid":stopraid_arg,
		"sethotspare":sethotspare_arg,
		"createvg":createvg_arg,                                   
		"vginfo":vginfo_arg,                                                
		"listvg":listvg_arg,                                                    
		"scanvg":scanvg_arg,                                               
		"removevg":removevg_arg,                                            
		"createvol":createvol_arg,                       
		"volinfo":volinfo_arg,                                   
		"listvol":listvol_arg,                                               
		"removevol":removevol_arg,                                            
		"createfs":createfs_arg,                   
		"fsinfo":fsinfo_arg,                                              
		"listfs":listfs_arg,                                                      
		"removefs":removefs_arg,
		"list_remain_dev":list_remain_dev_arg,
		"list_remain_raid":list_remain_raid_arg,
		"listvols":listvols_arg,
		}

if __name__ == '__main__':
	if(len(argv) < 2):
		help(argv)
		sys.exit(0)	

	# raid_dic = load_raid_dic("/opt/raid/raid_file")
	try:
		err,output=cmd_tab.get(argv[1],help)(argv[2:])
		if err : print Err.err_json(err)
		else: print output	
		# save_raid_dic("/opt/raid/raid_file", raid_dic)
	except Exception:
		err = Err.ENVIRONMENT
		print Err.err_json(err)
	finally:
		exit(err);	
