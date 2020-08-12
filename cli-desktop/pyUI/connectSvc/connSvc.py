# 链接本地服务端
# 数据转发处理
#

import grpc
from pyUI.proto import userOp_pb2, userOp_pb2_grpc

SvcAddr = '127.0.0.1:30011'


# def UserLoginOp():
    # conn = grpc.insecure_channel(SvcAddr)
    # client = userOp_pb2_grpc.userOpStub(channel=conn)
    # res = client.UserLogin(userOp_pb2.LoginRequest(Id=1, userName="lisi", passwd="123", rmbusrChk=True, rmbpasswdChk=False))
    # print("Revice: " + res.LoginMgs)
