package main

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


func main()  {
    service.UserOpService()
}


