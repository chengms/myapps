package app

import (
    "cli-desktop/public"
    "cli-desktop/tcpConn"
    "unsafe"
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
    u.UserId = id
    u.UserName = name
    u.UserPasswd = passwd
    u.Email = "Unknown"
    u.UserStatus = "unknown"
}

func (u *UserOption)LoginMsg(name, passwd string)  {
    u.SetUserMsg(11, name, passwd)
    //sendBuf := tcpConn.NewSendCommand()
    //sendBuf.Data = u.UserInfo
}

func (u *UserOption)RegisterMsg(name, passwd, email string) {
    u.SetUserMsg(11, name, passwd)
    u.Email = email

}

var UserOp UserOption

func UserLogin(name, passwd, email string) error {
    UserOp.RegisterMsg(name, passwd, email)
    length := unsafe.Sizeof(UserOp)

    sendCmd := tcpConn.NewSendCommand()
    err := sendCmd.SendCmd(int(length), UserOp)
    if err != nil {
        public.Loggerf.Error(err)
        return err
    }

    return nil
}
