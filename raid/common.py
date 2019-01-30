#!/usr/bin/python
# -*- coding:utf-8 -*-

import os
import json

def load_raid_dic(file_name):
    try:
        global file1
        os.system("touch %s" %file_name)
        file1 = open(file_name,"r+")
        #fcntl.flock(file,fcntl.LOCK_EX)
        data = file1.read()
        res = json.loads(data)
        #res = eval(str)
        file1.close()
        return res

    except Exception, e:
        return {}

def save_raid_dic(file_name,tree):
    global file1
    file1 = open(file_name,"w+")
    file1.seek(0,0)
    file1.write(json.dumps(tree))
    file1.close()