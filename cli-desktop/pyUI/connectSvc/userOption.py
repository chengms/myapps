import logging
from pyUI.connectSvc.connSvc import *

_LOGGER = logging.getLogger(__name__)
_LOGGER.setLevel(logging.INFO)

class UserOption(object):
    def __init__(self):
        super().__init__()

        #  连接 server
        self.conn = grpc.insecure_channel(SvcAddr)
        self.client = userOp_pb2_grpc.userOpStub(channel=self.conn)
        # 声明请求结构默认值
        self.loginRequest = userOp_pb2.LoginRequest(Id=0,
                                                    userName="unknown",
                                                    passwd="unknown",
                                                    rmbusrChk=False,
                                                    rmbpasswdChk=False)
        self.loginResponse = userOp_pb2.LoginReply()
        self.registerRequset = userOp_pb2.UserRegisterRequest(Id=0,
                                                              userName="unknown",
                                                              passwd="unknown",
                                                              Eamil='unknown')

        self.registerResponse = userOp_pb2.UserRegisterReply()

    def setUserLoginRequest(self, Id, userName, passwd, rmbusrChk, rmbpasswdChk):
        # set login request struct
        self.loginRequest = userOp_pb2.LoginRequest(Id=Id,
                                                    userName=userName,
                                                    passwd=passwd,
                                                    rmbusrChk=rmbusrChk,
                                                    rmbpasswdChk=rmbpasswdChk)

    def UserLogin(self):
        # call user login
        try:
            self.loginResponse = self.client.UserLogin(self.loginRequest)
        except grpc.RpcError as rpc_error:
            _LOGGER.error('Received login error: %s', rpc_error)
            return rpc_error
        else:
            _LOGGER.info('Received login message: %s', self.loginResponse)
            return self.loginResponse

    def setUserRegisterRequest(self, Id, userName, passwd, Eamil):
        self.registerRequset = userOp_pb2.UserRegisterRequest(Id=Id,
                                                              userName=userName,
                                                              passwd=passwd,
                                                              Eamil=Eamil)

    def UserRegister(self):
        try:
            self.registerResponse = self.client.UserRegister(self.registerRequset)
        except grpc.RpcError as rpc_error:
            _LOGGER.error('Received register error: %s', rpc_error)
            return rpc_error
        else:
            _LOGGER.info('Received register message: %s', self.registerResponse)
            return self.registerResponse


UserOp = UserOption()
