package main

import (
    "fmt"
    "golang.org/x/net/context"
    "google.golang.org/grpc"
    "log"

    pb "cli-desktop/proto"
)

//func init()  {
//    fmt.Println("start...")
//}


func test() {
    conn, err := grpc.Dial("127.0.0.1:19999", grpc.WithInsecure())
    if err != nil {
        log.Fatal(err)
    }
    defer conn.Close()

    client := pb.NewComputeClient(conn)
    reply, err := client.SayHello(context.Background(), &pb.HelloRequest{Helloworld: "lalala"})
    if err != nil {
        log.Fatal(err)
    }
    fmt.Println(reply)
}


func main()  {
    test()
}


