#!/bin/bash

cur_path=$(dirname $0)
cd $cur_path
src_dir=$(pwd)
cd -
echo $cur_path

rm -f /opt/generate_cmd/remote/output/*
cd /opt/generate_cmd/remote/
python generate_all.py $cur_path/test_comm.xml
cd -
cp /opt/generate_cmd/remote/output/*.h $cur_path
cp /opt/generate_cmd/remote/output/*.cpp $cur_path