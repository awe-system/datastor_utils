# datastor_utils
数据存储中常用基本结构 包括
data_t 结构 \
json_obj结构 \
通信基础sesstion \
全局env配置等功能 \
日志通信等 \
该库依赖于boost，以及log4cpp，以及rapid_json

# Build & Install
执行cmake的路径固定为：{datastor_utils_dir}/build \
cd {datastor_utils_dir} \
mkdir build \
cmake .. \
make -j 8 \
make install 

# Change install path
默认安装到系统路径 \
如果需要制定安装路径则执行如下命令 \
cd {datastor_utils_dir} \
mkdir build \
cmake -DCMAKE_INSTALL_PREFIX={output} .. \
make -j 8 \
make install

如果想要将安装到指定目录的产出安装到系统目录 \
执行:
cd {output}
sh ./install_output.sh