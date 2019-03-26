#!/usr/bin/env bash

cd $(dirname $0)
cur_path=$(pwd)
cd -
top_dir=$cur_path/../../..

cd $top_dir/cmake-build-debug/ut/algo
pwd
lcov -c -d . -o res.info
genhtml res.info -o html
cd html
open file://$(pwd)/index.html
echo -e "\033[34;4mfile://$(pwd)/index.html\033[0m"

cd $cur_path