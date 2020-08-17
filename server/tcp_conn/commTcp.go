package tcp_conn

import (
    "context"
    "fmt"
    "go/ast"
    "log"
    "net"
    "sync"
)

/*
*  -*- coding:utf8 -*-
*
*  TCP 通信相关实现
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/16 下午6:13
*  @File    : commTcp.go
 */

type ClientConn struct {
    Conn net.Conn
    UserId int
    UserName string
}


type TcpServer struct {
    Listener        net.Listener
    ClientConnSet   map[*ClientConn]interface{}
    GroupToMembers  map[string][]string
    mu             *sync.RWMutex
}

func NewTcpChatServer() *TcpServer {
    return &TcpServer{
        mu:             &sync.RWMutex{},
        ClientConnSet:  make(map[*ClientConn]interface{}),
        GroupToMembers: make(map[string][]string),
    }
}

func (t *TcpServer)Listen(ctx context.Context, address string) error  {
    listener, err := net.Listen("tcp", address)
    if err != nil {
        return err
    }

    t.Listener = listener

    log.Printf("Listening on %s", address)

    return err
}

func (t *TcpServer) Close(ctx context.Context) error {
    return t.Listener.Close()
}

func (t *TcpServer) Start(ctx context.Context, address string) error {
    if err := t.Listen(ctx, address); err != nil{
        return err
    }

Loop:
    for {
        select {
        case <-ctx.Done():
            fmt.Println("Chat server is shutting down...")
            // TODO: 停止服务的具体操作
            fmt.Println("shutdown successfully")
            break Loop
        default:
        }

        conn, err := t.Listener.Accept()
        if err != nil {
            fmt.Println(err)
            continue
        }

        clientConn := t.ClientAccept(conn)

    }

    return nil
}

func (t *TcpServer) ClientAccept(conn net.Conn) *ClientConn {
    fmt.Println("Accepting connection from %s", conn.RemoteAddr().String())

    t.mu.Lock()
    defer t.mu.Unlock()

    cc := &ClientConn{
        Conn: conn,

    }

}
