import commands
import os
import sys

base_dir="/tmp/globle_conf/"

def try_getenv_fromtmp(reponame,key):
    f = open(base_dir+reponame+"/"+key,"r")
    val=f.read()
    f.close()
    if val == "":
        raise
    return val

def write_env_intotmp(reponame,key,val):
    try:
        os.system("mkdir -p "+base_dir)
        os.system("mkdir -p "+base_dir+reponame)
        f = open(base_dir+reponame+"/"+key,"w")
        f.write(val)
        f.close()
    except:
        sys.stderr.write("write_env_intotmp faild\n")

def getenv_noss(reponame,key):
    try:
        return try_getenv_fromtmp(reponame,key)
    except:
        res = commands.getoutput("/usr/bin/serversan_list ss-"+reponame+" "+key)
        if res == "":return res
        write_env_intotmp(reponame,key,res)
        return res

def getenv(reponame,key):
    try:
        if(reponame[0:3] == "ss-"):
            return getenv_noss(reponame[3:],key)
    except:
        return getenv_noss(reponame,key)