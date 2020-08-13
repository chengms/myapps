# 检查相关数据是否正确
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/11
#  @File    : checkData.py
#


import json
from pyUI.connectSvc.userOption import UserOp


def CheckLoginData(userName, passwd, rmbNameSts, rmbPasswdSts):
    # print("Check data: ", userName, passwd, rmbNameSts, rmbPasswdSts)
    UserOp.setUserLoginRequest(1, userName, passwd, rmbNameSts, rmbPasswdSts)
    # print(UserOp.loginRequest)
    res = UserOp.UserLogin()

    print("login msg: ", res.LoginMgs, "\n--------------------\n")

    if res.LoginSts:
        return True
    return False
