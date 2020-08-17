package service

import (
	"fmt"
	"strings"
	"errors"
)

/*
*  -*- coding:utf8 -*-
*
*	通信协议相关，通信格式指定等
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/16 下午6:11
*  @File    : command.go
 */

var (
	InvalidMessageErr = errors.New("invalid message")
	UnsupportedCmdErr = errors.New("unsupported cmd")
)

const (
    ProtocolName    = "MENG"
    ProtocolVersion	= "v1.0"
	ProtocolSep     = " "

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
	Name string
	Data []byte
}

func (s *SendCommand) String() string {
	return strings.Join([]string{
		s.BaseCmd.String(),
		CmdSend,
		s.Name,
		string(s.Data),
	}, ProtocolSep) + "\n"
}

