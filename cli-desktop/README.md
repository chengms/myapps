## 目录结构
```
myapps
└─cli-desktop         # 桌面客户端
  ├─bin                # 
  ├─data               # 
  ├─conf               # 配置文件 
  ├─err                # 相关错误类
  ├─tools              # 
  ├─proto              # protocol 协议文件
  ├─pyUI               # 客户端 UI，使用 PyQt 编写，Qt designer 画界面 
  │  ├─proto               # 
  │  └─pyWindow            # 
  │      └─ui              # 
  └─resource           # 相关资源操作，如本地数据库
  └─service            # 客户端开启的相关服务
```


# 组件版本：
## golang
- go 1.14
- grpc3

## python
- python3.8/3.7
- PyQt5

# go protocol 文件编译命令
```shell script
protoc -I . --go_out=plugins=grpc:. ./userOp.proto
```
运行这个命令可以在当前目录中生成下面的文件：
userOp.pb.go


# 客户端简介
## 连接服务器
往服务端发送消息的格式：
```
    protocolName + version + messageType + data
```






