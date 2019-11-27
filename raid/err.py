#!/usr/bin/python
import json
OK 			= 0
WRONG_ARG	= 1 
ENVIRONMENT	= 2 
METADATA	= 3 
WRONG_RAID	= 4
UNKNOWN		= 99 

err_tab = {
	OK:"OK",
	WRONG_ARG:"WRONG_ARG",
	ENVIRONMENT:"ENVIRONMENT",
	METADATA:"METADATA",
	WRONG_RAID:"WRONG_RAID",
	UNKNOWN:"UNKNOWN"
}


def err_json(err):
	dic = {}
	if err:
		dic.update({"status":False})
	else:
		dic.update({"status":True})
	dic.update({"msg":err_tab.get(err,err_tab[UNKNOWN])});
	return json.dumps(dic)

