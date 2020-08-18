package tcpConn

import (
    "bufio"
    "fmt"
    "io"
)

/*
*  -*- coding:utf8 -*-
*
*  TCP 通信相关实现
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/17 下午6:13
*  @File    : writer.go
 */



// chat/protocol/writer.go
type CommandWriter struct {
    writer *bufio.Writer
}

func NewCommandWriter(writer io.Writer) *CommandWriter {
    return &CommandWriter{
        writer: bufio.NewWriter(writer),
    }
}

func (w *CommandWriter) Write(cmd interface{}) (err error) {
    _, err = w.writer.WriteString(fmt.Sprintf("%v", cmd))
    return w.writer.Flush()
}

