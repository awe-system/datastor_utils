#!/bin/bash

bash generate_profile.sh

conf_dest=$(cat yunke_serversan.conf | grep conf_dest | cut -d '=' -f2)
tmp_dest=$(cat yunke_serversan.conf | grep tmp_dest | cut -d '=' -f2)

echo y | cp yunke_serversan.conf /etc
rm -rf $conf_dest/yunke_serversan
mkdir $conf_dest/yunke_serversan
rm -rf $tmp_dest/yunke_serversan
mkdir $tmp_dest/yunke_serversan

cp ss_conf_profile.sh /etc/profile.d/ -f
cp yunke_serversan/* -rf $conf_dest/yunke_serversan
chmod a+x $conf_dest/yunke_serversan/bin/*

names=$(ls $conf_dest/yunke_serversan/bin/)

for name in $names;
do
	rm -f /usr/bin/$name
	ln -s $conf_dest/yunke_serversan/bin/$name /usr/bin/$name
done

echo "安装后 请执行source /etc/profile 让结果生效"
