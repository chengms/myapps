# 检查相关数据是否正确
from pyUI.connectSvc.userOption import UserOp


def CheckLoginData(usrName, passwd, rmbNameSts, rmbPasswdSts):
    UserOp.setUserLoginRequest(1, usrName, passwd, rmbNameSts, rmbPasswdSts)
    res = UserOp.UserLogin()

    print("login msg: ", res.RegisterMesg)
    if res.RegisterSts:
        return True
    return False
