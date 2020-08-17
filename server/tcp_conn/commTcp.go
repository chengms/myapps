package tcp_conn

import (
    "fmt"
    "log"
    "net"
    "server/service"
    "sync"
    "time"
    "context"
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

func (t *TcpServer)Start(ctx context.Context, address string) error  {
    l, err := net.Listen("tcp", address)
    if err != nil {
        return err
    }

    t.Listener = l

    log.Printf("Listening on %s", address)

    return err
}

func (t *TcpServer) Close(ctx context.Context) error {
    return t.Listener.Close()
}



func TcpTest()  {
    listener, err := net.Listen("tcp", "127.0.0.1:30051")
    if err != nil {
        panic(err)
    }

    for {
        conn, err := listener.Accept()
        if err != nil {
            panic(err)
        }

        go doServer(conn)
    }
}

func doServer(conn net.Conn)  {
    go func() {
        i := 0
        for ;;i++ {
            fmt.Println("connect time: ", i)
            time.Sleep(1*time.Second)
        }

    }()
    for {
        buf := make([]byte, 1024)
        len, err := conn.Read(buf)
        if err != nil {
            return
            //panic(err)
        }
        fmt.Println("Received data: %v\n", string(buf[:len]))
    }

}