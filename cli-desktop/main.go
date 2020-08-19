package main

/*
*  -*- coding:utf8 -*-
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/8 下午10:26
*  @File    : main.go
 */

import (
    "cli-desktop/app"
    "cli-desktop/public"
    "cli-desktop/service"
    "fmt"
    //"golang.org/x/net/context"
    //"google.golang.org/grpc"
    //"log"

)

func init()  {
   fmt.Println("start...")

   // 初始化配置文件
   public.InitCfg()

    // 初始化日志组件
    public.InitLogger()
   // 初始化 三方模块


}


func main()  {

    service.StartServer()
    _ = app.UserLogin("zhangsan", "123456", "sds@asd.com")

    //  program don't eixt
    <-(chan int)(nil)
}

