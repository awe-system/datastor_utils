#!/usr/bin/env bash

cd $(dirname $0)
curdir=$(pwd)
cd -
bin_dir=$curdir"/conf/bin"
conf_dir=$curdir"/conf/conf"

get_env_name()
{
    pre=$(echo $1 | cut -d '.' -f1 | cut -d '-' -f2)
	echo $pre"_"$2
}

rm -f $curdir"/awe_conf_profile.sh"

files=$(ls $conf_dir/*.conf)
for file in $files;do
        for env_item in $(cat $file | grep = | grep -v \#);
        do
            key=$(echo $env_item | cut -d '=' -f1)
            val=$(echo $env_item | cut -d '=' -f2)
            filename=$(basename $file)
            export_key=$(get_env_name $filename $key)
            echo "export "$export_key"="$val >> awe_conf_profile.sh
        done
done
