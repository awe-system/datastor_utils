#!/bin/bash

cmd="cp -r ./include/* /usr/local/include"
echo ${cmd}
${cmd}

cmd="cp -d ./lib/* /usr/local/lib"
echo ${cmd}
${cmd}

cmd="cp ./etc/awe_env.conf /etc"
echo ${cmd}
${cmd}

cmd="cp ./usr/bin/* /usr/bin"
echo ${cmd}
${cmd}

cmd="cp ./usr/lib64/python2.7/env.py /usr/lib64/python2.7/"
echo ${cmd}
${cmd}

conf_dest=$(bash ../conf/conf)
echo "conf_dest=${conf_dest}"

cmd="cp -r ./opt/* ${conf_dest}/"
echo ${cmd}
${cmd}
