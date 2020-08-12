
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/11
#  @File    : checkData.py
#

# 检查相关数据是否正确
from pyUI.connectSvc.userOption import UserOp
from pyUI.proto.userOp_pb2_grpc import *


def CheckLoginData(userName, passwd, rmbNameSts, rmbPasswdSts):
    UserOp.setUserLoginRequest(1, userName, passwd, rmbNameSts, rmbPasswdSts)
    res = UserOp.UserLogin()

    print(res)

    # print("login msg: ", res.RegisterMesg)
    if res.RegisterSts:
        return True
    return False
