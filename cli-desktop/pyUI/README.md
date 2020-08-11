# ui 目录
此目录下为 `pyqt5` 写的窗口代码

```bash
pyUI
├─img               # ui 相关图片
├─proto             # protocol 协议文件以及编译成 .py 的文件
└─pyWindow          # 界面
    └─ui            # QT ui 文件，及对应编译成 .py 的 ui 类文件
```

# PyQt 画界面注意事项
## 保存的 .ui 文件转换成 python 文件命令
```shell
pyuic5 -o 目标文件.py 源文件.ui
pyuic5 -o LoginUI.py LoginUI.ui
pyuic5 -o LoginUI.py Main.ui
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


