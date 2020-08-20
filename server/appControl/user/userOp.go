package user

import "server/tcpConn"

/*
*  -*- coding:utf8 -*-
*
*  用户相关操作封装
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/16 下午6:14
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

func (u *UserInfo)CheckLogin() error{

    return nil
}

