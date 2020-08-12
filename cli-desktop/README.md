## 目录结构
```
myapps
├─cli-desktop         # 桌面客户端
  ├─bin                # 
  ├─data               # 
  ├─err                # 相关错误类
  ├─tools              # 
  ├─proto              # protocol 协议文件
  ├─pyUI               # 客户端 UI，使用 PyQt 编写，Qt designer 画界面 
  │  ├─proto               # 
  │  └─pyWindow            # 
  │      └─ui              # 
  └─window             # 
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


