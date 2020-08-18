package service

import (
    "context"
    "server/public"
    "server/tcpConn"
)

type ServerInfo struct {
    ServerPort      string
    ServerHost      string
}


type AppsServer interface {
    Start(ctx context.Context, address string) error
    Close(ctx context.Context) error
}

func RunTcpServer()  {
    tcpCtx := context.TODO()
    var tcpSvc AppsServer
    tcpSvc = tcpConn.NewTcpChatServer()
    _ = tcpSvc.Start(
        tcpCtx,
        public.Cfg.GetTcpAddr(),
    )

}

