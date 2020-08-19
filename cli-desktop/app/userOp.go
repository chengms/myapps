package app

import (
    "cli-desktop/public"
    "cli-desktop/tcpConn"
)

/*
*  -*- coding:utf8 -*-
*
*  用户操作相关
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/19 11:47
*  @File    : userOp.go
 */

type UserInfo struct {
    tcpConn.CommandHeader
    UserId      int
    UserName    string
    UserPasswd  string
    Email       string
    UserStatus  string
}


type UserOption struct {
    UserInfo
}

func (u *UserOption)SetUserMsg(id int, name, passwd string)  {
    u.Protocol = tcpConn.ProtocolName
    u.Vserion = tcpConn.ProtocolVersion
    u.UserId = id
    u.UserName = name
    u.UserPasswd = passwd
    u.Email = "Unknown"
    u.UserStatus = "unknown"
}

func (u *UserOption)LoginMsg(id int, name, passwd string)  {
    u.SetUserMsg(id, name, passwd)
    u.CmdType = tcpConn.CmdLogin
    //sendBuf := tcpConn.NewSendCommand()
    //sendBuf.Data = u.UserInfo
}

func (u *UserOption)RegisterMsg(id int, name, passwd, email string) {
    u.SetUserMsg(id, name, passwd)
    u.Email = email
    u.CmdType = tcpConn.CmdRegister
}

var UserOp UserOption

func UserLogin(id int, name, passwd string) error {
    UserOp.LoginMsg(id, name, passwd)
    // length := unsafe.Sizeof(UserOp)

    sendCmd := tcpConn.NewSendCommand()
    err := sendCmd.SendCmd(UserOp.CmdType, UserOp)
    if err != nil {
        public.Loggerf.Error(err)
        return err
    }

    return nil
}

func UserRegister(id int, name, passwd, email string) error {
    UserOp.RegisterMsg(id, name, passwd, email)
    // length := unsafe.Sizeof(UserOp)

    sendCmd := tcpConn.NewSendCommand()
    err := sendCmd.SendCmd(UserOp.CmdType, UserOp)
    if err != nil {
        public.Loggerf.Error(err)
        return err
    }

    return nil
}

func LoginTest()  {
    _ = UserRegister(11,"zhangsan", "123456", "sds@asd.com")
}


