# myapps
myapps， desktop app， web app， server，and mobile app

## 目录结构
```
myapps
├─cli-desktop         # 桌面客户端
│  ├─bin                # 
│  ├─data               # 
│  ├─err                # 相关错误类
│  ├─tools              # 
│  ├─proto              # protocol 协议文件
│  ├─pyUI               # 客户端 UI，使用 PyQt 编写，Qt designer 画界面 
│  │  ├─proto               # 
│  │  └─pyWindow            # 
│  │      └─ui              # 
│  └─window             # 
│  └─service            # 客户端开启的相关服务
└─server              # 服务端
    ├─appControl        # 
    ├─conf              # 存放配置文件, 采用 yaml 格式
    └─service           # 

```


# 组件版本：
## golang
- go 1.14
- grpc3

## python
- python3.8/3.7
- PyQt5









