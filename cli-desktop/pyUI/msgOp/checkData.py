# 检查相关数据是否正确
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/11
#  @File    : checkData.py
#


import json
import grpc
from pyUI.connectSvc.userOption import UserOp


def CheckLoginData(userName, passwd, rmbNameSts, rmbPasswdSts):
    # 登录验证

    # print("Check data: ", userName, passwd, rmbNameSts, rmbPasswdSts)
    UserOp.setUserLoginRequest(1, userName, passwd, rmbNameSts, rmbPasswdSts)
    # print(UserOp.loginRequest)
    res, err = UserOp.UserLogin()
    if None != err:
        return False

    print("login msg: ", res.LoginMgs, "\n--------------------\n")

    if res.LoginSts:
        return True
    return False


def CheckRegisterData(userName, passwd, email):
    # 注册验证
    print("reg msg: ", userName, passwd, email)
    UserOp.setUserRegisterRequest(1, userName, passwd, email)
    # print(UserOp.setUserLoginRequest)
    res, err = UserOp.UserRegister()
    if None != err:
        return False

    print("Register msg: ", res.RegisterMesg, "\n--------------------\n")

    if res.RegisterSts:
        return True
    return False
