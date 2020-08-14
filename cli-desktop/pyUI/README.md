# ui 目录
此目录下为 `pyqt5` 写的窗口代码

```bash
pyUI
├─img               # ui 相关图片
├─proto             # protocol 协议文件以及编译成 .py 的文件
└─pyWindow          # 界面
    └─ui            # QT ui 文件，及对应编译成 .py 的 ui 类文件
```

# PyQt 用 designer 画界面注意事项
## 保存的 .ui 文件转换成 python 文件命令
```shell
pyuic5 -o 目标文件.py 源文件.ui
pyuic5 -o LoginUI.py LoginUI.ui
pyuic5 -o RegisterUI.py RegisterUI.ui
pyuic5 -o MainWindow.py MainWindow.ui
pyuic5 -o RegisterSuccessUI.py RegisterSuccessUI.ui
```

# proto 文件转换为 python 文件
```shell script
python -m grpc_tools.protoc -I ./ --python_out=./ --grpc_python_out=./ userOp.proto

# python_out目录指定 xxxx_pb2.py的输出路径，我们指定为./ 当前路径
# grpc_python_out指定xxxx_pb2_grpc.py文件的输出路径,我们指定为./ 当前路径
# grpc_tools.protoc 这是我们的工具包，刚刚安装的
# -I参数指定协议文件的查找目录，我们都将它们设置为当前目录./
# userOp.proto 我们的协议文件

```



# 删除 `__pycache__` 预编译文件

删除缓存
```shell script
git rm -r --cached .
```
在 .gitignore 文件中添加对应文件夹
执行以下命令，在git中删除 
```shell script
git rm -rf cli-desktop/ui/pyWindow/__pycache__
```
然后重新提交
```shell script
git add .
```


# 安装依赖
```shell script
pip install grpcio            # gRPC
pip install protobuf          # ProtoBuf 相关的 python 依赖库
pip install grpcio-tools      # python grpc 的 protobuf 编译工具

# 安装时可以用豆瓣源加速或其他源加速， 如下
pip install grpcio -i https://pypi.douban.com/simple
```

