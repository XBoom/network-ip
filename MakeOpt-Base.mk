# 编译选项，不同的运行环境使用不同的 MakeOption_xx.mk
# MakeOption-Base.mk 默认
# 项目基本框架的Make选项，只编译基本框架内容

## 依赖库
OPTIONS_LIB_LOG=y
OPTIONS_LIB_UEV=y
OPTIONS_LIB_CONF=y

## 进程
OPTIONS_APP_RCU=y
OPTIONS_APP_UEV=y

## proto
OPTIONS_INCLUDE_PROTO=y


## 内核版本
KERNEL_VERSION=linux-6.1.0
## 内核驱动
OPTIONS_DRIVER_WATCH_DOG=y

