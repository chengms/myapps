package tcpConn

import (
    "cli-desktop/public"
    "errors"
    "fmt"
)

/*
*  -*- coding:utf8 -*-
*
*  通信协议相关，通信格式指定等
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/17 16:21
*  @File    : command.go
 */

var (
    InvalidMessageErr = errors.New("invalid message")
)

const (
    CmdSend         = 0
    CmdBroadCase    = 1
    CmdLogin        = 2
    CmdLogOut       = 3
    CmdRegister     = 4
    CmdReceive      = 5
    CmdGroup        = 6
    CmdLeave        = 7	    // 离开群聊
    CmdStatus       = 8
)

const (
    ProtocolName    = "MENG"
    ProtocolVersion	= "v1.0"
    ProtocolSep     = " "				// 分隔字符
)

type BasrCmd struct {
    Protocol    string  `json:"Protocol"`
    Vserion     string  `json:"Version"`
}

type CommandHeader struct {
    BasrCmd             `json:"BasrCmd"`
    // CmdLength   int     `json:"CmdLength"`
    CmdType     int     `json:"CmdType"`
}

func NewCommandHeader(cmdType int) CommandHeader {
    return CommandHeader{
                    BasrCmd: BasrCmd{
                        Protocol: ProtocolName,
                        Vserion: ProtocolVersion,
                    },
                    // CmdLength: 0,
                    CmdType: cmdType,
        }
        // header.CmdLength = int(unsafe.Sizeof(header)) + length

    // return public.Jsonit.Marshal(header)
}

type SendCommand struct {}

func (s *SendCommand) SendCmd(CmdType int, msg interface{}) error {
    data, _ := public.Jsonit.MarshalToString(msg)

    // var buffer bytes.Buffer
    // buffer.Write(header)
    // buffer.Write(data)
    // buffer.Write([]byte("\n"))
    //fmt.Println(header, data)
    // CmdBuf := buffer.String()
    // CmdBuf := header + data + "\n"
    CmdBuf := data + "\n"
    length := fmt.Sprintf("%8d", len(CmdBuf))
    fmt.Println(length)
    CmdBuf = length + CmdBuf
    fmt.Println(CmdBuf)
    err := ConnSvc.SendMsg(CmdBuf)
    if err != nil {
        return err
    }

    return nil
}

func NewSendCommand() SendCommand {
    return SendCommand{}
}


