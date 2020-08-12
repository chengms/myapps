
from pyUI.connectSvc.connSvc import *


class UserOption(object):
    def __init__(self):
        super().__init__()

        self.conn = grpc.insecure_channel(SvcAddr)
        self.client = userOp_pb2_grpc.userOpStub(channel=self.conn)
        self.loginRequest = userOp_pb2.LoginRequest(Id=0,
                                                    userName="unknown",
                                                    passwd="unknown",
                                                    rmbusrChk=False,
                                                    rmbpasswdChk=False)
        self.loginResponse = userOp_pb2.LoginResponse()
        self.registerRequset = userOp_pb2.UserRegisterRequest(Id=0,
                                                              userName="unknown",
                                                              passwd="unknown",
                                                              Eamil='unknown')
        self.registerResponse = userOp_pb2.UserRegisterResponse()

    def setUserLoginRequest(self, Id, userName, passwd, rmbusrChk, rmbpasswdChk):
        self.registerRequset = userOp_pb2.LoginRequest(Id=Id,
                                                       userName=userName,
                                                       passwd=passwd,
                                                       rmbusrChk=rmbusrChk,
                                                       rmbpasswdChk=rmbpasswdChk)

    def UserLogin(self):
        self.loginResponse = self.client.UserLogin(self.loginRequest)
        return self.loginResponse

    def setUserRegisterRequest(self, Id, userName, passwd, Eamil):
        self.registerRequset = userOp_pb2.UserRegisterRequest(Id=Id,
                                                              userName=userName,
                                                              passwd=passwd,
                                                              Eamil=Eamil)

    def UserRegister(self):
        self.registerResponse = userOp_pb2.UserRegisterRequest(self.registerRequset)
        return self.registerResponse


UserOp = UserOption()
