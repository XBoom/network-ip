#设置基础变量

#设置库路径
export LD_LIBRARY_PATH=/home/libs/:$LD_LIBRARY_PATH

# 设置编译信息
export BUILD_DATE_TIME=$(shell date '+%Y-%m-%d %H:%M:%S' )
export BUILD_DATE=$(shell date '+%y%m%d')
export BUILDER_NAME=$(shell whoami)
