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

    //

    public.Loggerf.Info("Server init successfully")
}

func main() {
    //public.LoggerTest()
    //tcp_conn.TcpTest()
    //public.TestConf()
    //public.TestJson()
    // 开启 tcp 监听
    go service.RunTcpServer()

    // 退出时关闭logger
    defer public.CloseLogger()

    <-(chan int)(nil)
}

