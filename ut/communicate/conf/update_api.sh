#!/bin/bash

cd $(dirname $0)
cur_path=$(pwd)
cd -
src_dir=$cur_path/../../
echo $cur_path

rm -f /opt/generate_cmd/remote/output/*
cd /opt/generate_cmd/remote/
python generate_all.py $cur_path/test_comm.xml
cd -
cp /opt/generate_cmd/remote/output/*.h $cur_path
cp /opt/generate_cmd/remote/output/*.cpp $cur_path

rm -f /opt/generate_cmd/remote/output/*
cd /opt/generate_cmd/remote/
python generate_all.py $cur_path/test_comm_async.xml
cd -
cp /opt/generate_cmd/remote/output/*.h $cur_path
cp /opt/generate_cmd/remote/output/*.cpp $cur_path

rm -f /opt/generate_cmd/remote/output/*
cd /opt/generate_cmd/remote/
python generate_all.py $cur_path/test_comm_sync.xml
cd -
cp /opt/generate_cmd/remote/output/*.h $cur_path
cp /opt/generate_cmd/remote/output/*.cpp $cur_path
