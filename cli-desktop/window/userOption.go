package window

/*
* 用户相关操作
* 登录，注册，登录验证等
*
* chengms.com
* auther: chengms2012@163.com
* date: 2020.8.11
*
*/

import (
    pb "cli-desktop/proto"
    "fmt"
    "golang.org/x/net/context"
)



// 定义操作类
type UserOpSvc struct {}


// 实现接口服务
func (usrOp UserOpSvc) UserLogin (ctx context.Context, loginReq *pb.LoginRequest) (*pb.LoginResponse, error) {
    resp := new(pb.LoginResponse)

    if loginReq.RmbusrChk == true {
        resp.LoginSts = true
    } else {
        resp.LoginSts = false
    }
    resp.LoginMgs = fmt.Sprintf("Login user: %s", loginReq.UserName)

    return resp, nil
}

func (usrOp UserOpSvc) UserRegister(ctx context.Context, regReq *pb.UserRegisterRequest) (*pb.UserRegisterResponse, error) {
    resp := new(pb.UserRegisterResponse)
    resp.RegisterMesg = fmt.Sprintf("reg user: %s", regReq.UserName)

    return resp, nil
}




