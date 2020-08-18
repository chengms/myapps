package tcpConn

import (
    "bufio"
    "cli-desktop/tools"
    "fmt"
    "net"
    "os"
    "strings"
)

/*
*  -*- coding:utf8 -*-
*
*  TCP 通信相关实现
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/17 16:20
*  @File    : connTcp.go
 */

type ConnServer struct {
    SvcConn   net.Conn
    Writer    *CommandWriter
    Reader    *CommandReader
}

func (c *ConnServer)ConnectServer(addr string) error {
    var err error
    c.SvcConn, err = net.Dial("tcp", addr)
    if err != nil {
        return err
    }

    c.Writer = NewCommandWriter(c.SvcConn)
    c.Reader = NewCommandReader(c.SvcConn)

    return nil
}

var ConnSvc ConnServer

func StartTcpConn() error {
    return ConnSvc.ConnectServer(tools.Cfg.GetTcpAddr())
}






func test() {
    //打开连接:
    conn, err := net.Dial("tcp", "localhost:40051")
    if err != nil {
        //由于目标计算机积极拒绝而无法创建连接
        fmt.Println("Error dialing", err.Error())
        return // 终止程序
    }

    inputReader := bufio.NewReader(os.Stdin)
    fmt.Println("First, what is your name?")
    clientName, _ := inputReader.ReadString('\n')
    // fmt.Printf("CLIENTNAME %s", clientName)
    trimmedClient := strings.Trim(clientName, "\r\n") // Windows 平台下用 "\r\n"，Linux平台下使用 "\n"
    // 给服务器发送信息直到程序退出：
    for {
        fmt.Println("What to send to the server? Type Q to quit.")
        input, _ := inputReader.ReadString('\n')
        trimmedInput := strings.Trim(input, "\r\n")
        // fmt.Printf("input:--%s--", input)
        // fmt.Printf("trimmedInput:--%s--", trimmedInput)
        if trimmedInput == "Q" {
            return
        }
        _, err = conn.Write([]byte(trimmedClient + " says: " + trimmedInput + "\n"))
    }
}
