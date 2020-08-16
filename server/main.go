package main

import (
    "fmt"
    "server/public"
)

func init() {
    fmt.Println("Server start init...")

    public.InitLogger()

}

func main() {
    public.LoggerTest()

}
