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
    "cli-desktop/public"
    "fmt"
    "strconv"

    // "golang.org/x/net/context"
    // "google.golang.org/grpc"
    // "log"

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

    //service.StartServer()

    fmt.Println("======")
    //public.TestJson()
    //app.LoginTest()
    test11()

    //  program don't eixt
    //<-(chan int)(nil)

}

func test11()  {
    var n1 int
    n1 = 1024
    //hexstr := "400"
    //s, _ := strconv.ParseInt(hexstr, 16, 32)
    //fmt.Printf("hexstr: %d\n", s)
    //fmt.Printf("hexstr: %x\n", s)
    ss := fmt.Sprintf("%08x", n1)
    fmt.Println(len(ss))
    fmt.Println(ss)
    nn, _ := strconv.ParseInt(ss, 16, 32)
    fmt.Println("int:", nn)
    fmt.Printf("hex: %8x\n", nn)
    str := "asdalllaaaa"
    buf := ss + str

    fmt.Println(len(buf),"buf:", buf)

}
