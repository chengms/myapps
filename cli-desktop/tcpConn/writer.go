package tcpConn

import (
    "bufio"
    "fmt"
    "io"
)

/*
*  -*- coding:utf8 -*-
*
*  写数据流
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/18 下午8:26
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

