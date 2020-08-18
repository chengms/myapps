# server 

# 简介
本项目是一个即使通讯服务器，代码全部使用golang完成。主要功能：
- 支持 tcp 接入 
- 支持 webSocket 接入 (待实现)
- 支持离线消息
- 单聊，群聊
- 服务水平扩展 (待实现)
- 单用户多设备同时在线 (待实现)


# 使用技术
+ 数据库： Mysql (数据持久化) + Redis (缓存) 
+ ORM框架：GORM
+ 通讯框架：GRPC
+ 日志框架：Zap
+ JSON 解析器：jsoniter（json-iterator）

## 相关库安装
```shell script
#  日志框架安装
go get -u go.uber.org/zap
#  日志配套分割工具
go get github.com/natefinch/lumberjack
# jsoniter 安装
go get github.com/json-iterator/go



```


# 项目目录介绍
```
server
├── appControl              # 服务逻辑处理
│    └── user                   # 用户相关操作
├── conf                    # 配置文件
├── data                    # 数据相关接口处理
│    ├── cache                  # 缓存，操作redis封装
│    ├── dao                    # 操作数据库的封装
│    ├── db                     # 数据库实例
│    ├── module                 # 模型层
│    └── sql                    # 数据库建表语句
├── docs                    # 相关文档
├── proto                   # 相关协议文件
├── public                  # 连接层和逻辑层公共代码
├── service                 # 服务入口
├── tcp_conn                # TCP 连接层启动入口
├── test                    # 测试脚本
└── ws_conn                 # webSocket连接层启动入口
```


# 服务简介






# 


