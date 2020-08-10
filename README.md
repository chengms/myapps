# myapps
myapps， desktop app， web app， server，and mobile app

## 目录结构
```
myapps
├─cli-desktop         # 桌面客户端
│  ├─bin                # 
│  ├─data               # 
│  ├─err                # 
│  ├─img                # 
│  ├─tools              # 
│  ├─ui                 # 客户端 UI，使用 PyQt 编写，Qt designer 画界面 
│  │  └─__pycache__
│  └─window
└─server              # 服务端
    ├─appControl        # 
    ├─conf              # 存放配置文件, 采用 yaml 格式
    └─service           # 

```

# PyQt画界面注意事项
## 保存的 .ui 文件转换成 python 文件命令
```shell
pyuic5 -o 目标文件.py 源文件.ui
pyuic5 -o LoginUI.py LoginUI.ui
```






