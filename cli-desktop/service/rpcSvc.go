package service

import (
    pb "cli-desktop/proto"
    "cli-desktop/window"
    "google.golang.org/grpc"
    "google.golang.org/grpc/grpclog"
    "net"
)

const (
    // grpc server addr
    UserOpAddr = "127.0.0.1:30011"
)

func UserOpService()  {
    listen, err := net.Listen("tcp", UserOpAddr)
    if err != nil {
        grpclog.Fatal("Failed to listen:%v", err)
    }
    // 实例化 grpc server
    svc := grpc.NewServer()

    // 注册server
    var UserOpSvc = window.UserOpSvc{}
    pb.RegisterUserOpServer(svc, UserOpSvc)

    grpclog.Info("Listen on: " + UserOpAddr)
    _ = svc.Serve(listen)

}
