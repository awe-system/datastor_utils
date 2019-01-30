#!/usr/bin/python
import sys
from sys import argv

def help(arg):
  print "----------menu--------------------"
  print "help                                                            ; show this"
  print "listdev                                                         ; list all dev info"
  print "createraid <trunk_sec> <raid_level> <raid_name> <dev1>...[devn] ; create raid"
  print "raidinfo <raid_name>                                            ; show raid info by raid_name"
  print "listraid                                                        ; show all raid info"
  print "scanraid                                                        ; scan disk and regroup all raid"
  print "removeraid <raid_name>                                          ; remove raid by raid name"
  print "sethotspare <raid_name> <dev>                                   ; set disk into some raid"
  print "createvg <raid/dev> <vg_name>                                   ; create vg on raid/dev"
  print "vginfo <vg_name>                                                ; show vg info"
  print "listvg                                                          ; show all vg info"
  print "scanvg                                                          ; scan dev/raid and regroup all vg"
  print "removevg <vg_name>                                              ; remove vg"
  print "createvol <vg_name> <vol_name> <vol_size>                       ; create vol on vg"
  print "vginfo <vg_name> <vol_name>                                     ; show vol info"
  print "listvol <vg_name>                                               ; show all vol info of one vg"
  print "removevol <vol_name>                                            ; remove vol"
  print "createfs <raid/dev/vol> <dir_name> <fs_type>                    ; create fs on raid/dev/vol"
  print "fsinfo <dir_name>                                               ; show fs info(dev fs_type etc.)"
  print "listfs                                                          ; show all fs info"
  print "removefs <dir_name>                                             ; remove fs (umount and delete dir)"

cmd_tab = {"help":help
}


if __name__ == '__main__':
	if(len(argv) == 0):
		cmd_tab["help"](argv)
		sys.exit(0)	
	cmd_tab.get(argv[0],help)(argv[1:])



