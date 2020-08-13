# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import userOp_pb2 as userOp__pb2


class userOpStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.UserLogin = channel.unary_unary(
                '/userOp/UserLogin',
                request_serializer=userOp__pb2.LoginRequest.SerializeToString,
                response_deserializer=userOp__pb2.LoginReply.FromString,
                )
        self.UserRegister = channel.unary_unary(
                '/userOp/UserRegister',
                request_serializer=userOp__pb2.UserRegisterRequest.SerializeToString,
                response_deserializer=userOp__pb2.UserRegisterReply.FromString,
                )


class userOpServicer(object):
    """Missing associated documentation comment in .proto file."""

    def UserLogin(self, request, context):
        """登录
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def UserRegister(self, request, context):
        """注册
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_userOpServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'UserLogin': grpc.unary_unary_rpc_method_handler(
                    servicer.UserLogin,
                    request_deserializer=userOp__pb2.LoginRequest.FromString,
                    response_serializer=userOp__pb2.LoginReply.SerializeToString,
            ),
            'UserRegister': grpc.unary_unary_rpc_method_handler(
                    servicer.UserRegister,
                    request_deserializer=userOp__pb2.UserRegisterRequest.FromString,
                    response_serializer=userOp__pb2.UserRegisterReply.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'userOp', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class userOp(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def UserLogin(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/userOp/UserLogin',
            userOp__pb2.LoginRequest.SerializeToString,
            userOp__pb2.LoginReply.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def UserRegister(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/userOp/UserRegister',
            userOp__pb2.UserRegisterRequest.SerializeToString,
            userOp__pb2.UserRegisterReply.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
