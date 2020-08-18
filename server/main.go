package main

import (
    "fmt"
    "server/public"
    "server/service"
)

func init() {
    fmt.Println("Server start init...")
    //  读配置文件
    public.InitCfg()

    // 初始化日志组件
    public.InitLogger()

    // 其他

    public.Loggerf.Info("Server init successfully")
}

func main() {
    //public.LoggerTest()
    //tcp_conn.TcpTest()
    //public.TestConf()
    service.RunTcpServer()

    defer public.CloseLogger()
}

