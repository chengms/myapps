import time
import grpc
from concurrent import futures

import compute_pb2, compute_pb2_grpc  # 刚刚生产的两个文件


class ComputeServicer(compute_pb2_grpc.ComputeServicer):
    def SayHello(self, request, ctx):
        max_len = str(len(request.helloworld))
        return compute_pb2.HelloReply(result=max_len)


def main():
    # 多线程服务器
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    # 实例化 计算len的类
    servicer = ComputeServicer()
    # 注册本地服务,方法ComputeServicer只有这个是变的
    compute_pb2_grpc.add_ComputeServicer_to_server(servicer, server)
    # 监听端口
    server.add_insecure_port('127.0.0.1:19999')
    # 开始接收请求进行服务
    server.start()
    # 使用 ctrl+c 可以退出服务
    try:
        print("running...")
        time.sleep(1000)
    except KeyboardInterrupt:
        print("stopping...")
        server.stop(0)


if __name__ == '__main__':
    main()
