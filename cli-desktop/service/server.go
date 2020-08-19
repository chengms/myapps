package service

import (
    "cli-desktop/public"
    "cli-desktop/tcpConn"
    "fmt"
)

/*
*  -*- coding:utf8 -*-
*
*
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/19 17:34
*  @File    : server.go
 */

// var ServerReady chan int


func StartServer()  {
    ServerReady := make(chan int)

    // 开启和python界面互动 grpc 服务
    go UserOpService()

    // connect to server
    go func() {
        err := tcpConn.TcpConnectionToSvc()
        if err != nil {
            public.Loggerf.Errorf("%v", err)

            //TODO: 连接不上直接启动本地服务
            panic(err)
            return
        }
        fmt.Println("connect server...")

        ServerReady <- 1

    }()

    // 等待所有服务准备完成
    _ = <- ServerReady

}


