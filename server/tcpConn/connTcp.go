package tcpConn

import (
    "context"
    "fmt"
    "io"
    "net"
    "server/public"
    "strings"
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
*  @File    : connTcp.go
 */

const ClosedConnectionMsg = "use of closed network connection"

type ClientConn struct {
    Conn net.Conn
    UserId int
    UserName string
    Writer *CommandWriter
    Reader *CommandReader
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

    public.Loggerf.Infof("Listening on: %s", address)

    return err
}

func (t *TcpServer) Close(ctx context.Context) error {
    return t.Listener.Close()
}

func (t *TcpServer) Start(ctx context.Context, address string) error {
    if err := t.Listen(ctx, address); err != nil{
        public.Loggerf.Errorf("Server listen addr %s, err: %v", address, err)
        return err
    }

Loop:
    for {
        select {
        case <-ctx.Done():
            public.Loggerf.Info("Chat server is shutting down...")
            //TODO: 停止服务的具体操作
            public.Loggerf.Info("shutdown successfully")
            break Loop
        default:
        }

        conn, err := t.Listener.Accept()
        if err != nil {
            fmt.Println(err)
            continue
        }

        clientConn := t.ClientAccept(conn)
        go t.serve(clientConn)
    }

    return nil
}

func (t *TcpServer) ClientAccept(conn net.Conn) *ClientConn {
     public.Loggerf.Infof("Accepting connection from %s", conn.RemoteAddr().String())

    t.mu.Lock()
    defer t.mu.Unlock()

    cc := &ClientConn{
        Conn: conn,
        Writer: NewCommandWriter(conn),
    }

    t.ClientConnSet[cc] = struct {}{}
    return cc
}

func (t *TcpServer) Remove(cc *ClientConn) {
    t.mu.Lock()
    defer t.mu.Unlock()

    delete(t.ClientConnSet, cc)
}


func (t *TcpServer) serve(cc *ClientConn) {
    cc.Reader = NewCommandReader(cc.Conn)
    defer t.Remove(cc)

    for {
        var err error

         cmd, err := cc.Reader.Read()

         if err == io.EOF {
             break
         }

         if err != nil {
             public.Loggerf.Infof("read message err:%v", err)
             if strings.Contains(err.Error(), ClosedConnectionMsg) {
                 break
             }
             //break
             continue
         }

        if cmd != nil {
            switch v := cmd.(type) {
            //case *protocol.SendCommand:
            //    err = s.handleSend(cc, cmd.(*protocol.SendCommand))
            //case *protocol.BroadCastCommand:
            //    err = s.handleBroadcast(cc, cmd.(*protocol.BroadCastCommand))
            //case *protocol.LoginCommand:
            //    err = s.handleLogin(cc, cmd.(*protocol.LoginCommand))
            //case *protocol.LogoutCommand:
            //    err = s.handleLogout(cc, cmd.(*protocol.LogoutCommand))
            //case *protocol.GroupCommand:
            //    err = s.handleGroup(cc, cmd.(*protocol.GroupCommand))
            //case *protocol.LeaveCommand:
            //    err = s.handleLeave(cc, cmd.(*protocol.LeaveCommand))
            default:
                public.Loggerf.Info("cmd:%T %v not supported", v, v)
            }

            //if err != nil {
            //    public.Loggerf.Info("handle cmd err:%v", err)
            //}
        }
    }
}
