编译Proto命令

```shell
# --proto_path：指定导入其他.proto文件时的搜索路径。
# --cpp_out：指定生成C++代码的目标目录。
# --example.proto：你的.proto文件名称。
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR example.proto
protoc -I=. --c_out=. example.proto

# 安装 protoc
apt install protobuf-compiler
apt-get install protobuf-c-compiler
```


如果希望生成C语言代码，你可以用`--c_out`来替代`--cpp_out`，需要通过 `protobuf-c` 将`proto`与`bin`进行相互转换