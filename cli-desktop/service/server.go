package service

import (
    "cli-desktop/public"
    "cli-desktop/tcpConn"
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


func StartServer()  {
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

    }()

    //

}


