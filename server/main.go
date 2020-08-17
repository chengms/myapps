package main

import (
    "fmt"
    "server/public"
    "server/tcp_conn"
)

func init() {
    fmt.Println("Server start init...")

    public.InitLogger()

}

func main() {
    //public.LoggerTest()
    tcp_conn.TcpTest()

}

