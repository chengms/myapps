package main

import (
    pb "cli-desktop/proto"
    "context"
    "fmt"
    "google.golang.org/grpc"
    "google.golang.org/grpc/grpclog"
)

const (
    SvcAddr = "127.0.0.1:30011"
)

func main()  {
    // connection
    conn, err := grpc.Dial(SvcAddr, grpc.WithInsecure())
    if err != nil {
        grpclog.Fatal(err)
    }
    defer conn.Close()

    // init client
    c := pb.NewUserOpClient(conn)

    // 调用方法
    req := &pb.UserRegisterRequest{Id: 1, UserName: "zhangsan", Passwd: "123", Eamil: "1111"}
    res, err := c.UserRegister(context.Background(), req)
    if err != nil {
        grpclog.Fatal(err)
    }
    grpclog.Info(res.RegisterMesg)
    fmt.Println(res.RegisterMesg)
}
