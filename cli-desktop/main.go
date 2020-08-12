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
    "cli-desktop/service"

    "fmt"
    //"golang.org/x/net/context"
    //"google.golang.org/grpc"
    //"log"

)

func init()  {
   fmt.Println("start...")
}


func main()  {
    service.UserOpService()
}

//func test() {
//    conn, err := grpc.Dial("127.0.0.1:19999", grpc.WithInsecure())
//    if err != nil {
//        log.Fatal(err)
//    }
//    defer conn.Close()
//
//    client := pb.NewComputeClient(conn)
//    reply, err := client.SayHello(context.Background(), &pb.HelloRequest{Helloworld: "lalala"})
//    if err != nil {
//        log.Fatal(err)
//    }
//    fmt.Println(reply)
//}
