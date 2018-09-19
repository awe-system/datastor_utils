#!/bin/bash

bash generate_profile.sh

conf_dest=$(cat awe_env.conf | grep conf_dest | cut -d '=' -f2)
tmp_dest=$(cat awe_env.conf | grep tmp_dest | cut -d '=' -f2)

echo y | cp awe_env.conf /etc
rm -rf $conf_dest/awe_conf
mkdir $conf_dest/awe_conf
rm -rf $tmp_dest/awe_conf
mkdir $tmp_dest/awe_conf

cp awe_conf_profile.sh /etc/profile.d/ -f
cp awe_conf/* -rf $conf_dest/awe_conf
chmod a+x $conf_dest/awe_conf/bin/*

names=$(ls $conf_dest/awe_conf/bin/)

for name in $names;
do
	rm -f /usr/bin/$name
	ln -s $conf_dest/awe_conf/bin/$name /usr/bin/$name
done

echo "安装后 请执行source /etc/profile 让结果生效"
