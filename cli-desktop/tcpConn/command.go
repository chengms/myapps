package tcpConn

import (
    "cli-desktop/public"
    "errors"
    "unsafe"
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
    CmdLength   int     `json:"CmdLength"`
    CmdType     int     `json:"CmdType"`
}

func NewCommandHeader(length, cmdType int)  (string, error) {
    var header = CommandHeader{
                    BasrCmd: BasrCmd{
                        Protocol: ProtocolName,
                        Vserion: ProtocolVersion,
                    },
                    CmdLength: 0,
                    CmdType: cmdType,
        }
        header.CmdLength = int(unsafe.Sizeof(header)) + length
    return public.Jsonit.MarshalToString(header)
}

type SendCommand struct {}

func (s *SendCommand) SendCmd(length int, msg interface{}) error {
    header, _ := NewCommandHeader(int(length), CmdLogin)
    data, _ := public.Jsonit.MarshalToString(msg)

    //fmt.Println(header, data)

    err := ConnSvc.SendMsg(header + data)
    if err != nil {
        return err
    }

    return nil
}

func NewSendCommand() SendCommand {
    return SendCommand{}
}


