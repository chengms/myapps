package tcpConn

import (
    "fmt"
    "strings"
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

const (
    ProtocolName    = "MENG"
    ProtocolVersion	= "v1.0"
    ProtocolSep     = " "				// 分隔字符

    CmdSend         = "SEND"
    CmdBroadCase    = "BROADCAST"
    CmdLogin        = "LOGIN"
    CmdLogOut       = "LOGOUT"
    CmdRegister     = "REGISTER"
    CmdReceive      = "RECEIVE"
    CmdGroup        = "GROUP"
    CmdLeave        = "LEAVE"			// 离开群聊
    CmdStatus       = "STATUS"
)

type BaseCmd struct {
    Protocol string
    Version  string
}

func (b *BaseCmd) String() string {
    return fmt.Sprintf("%s%s", b.Protocol, b.Version)
}

type SendCommand struct {
    BaseCmd
    CmdType string
    Data string
}

func (s *SendCommand) String() string {
    return strings.Join([]string{
        s.BaseCmd.String(),
        s.CmdType,
        s.Data,
    }, ProtocolSep) + "\n"
}





