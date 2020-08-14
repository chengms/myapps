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

//type LoginResponse struct{
//    UserId int
//    LoginMsg string     // 登录提示信息
//    LoginSts bool       // 登录状态，True 表示成功，False 表示登录失败
//}
//
//type UserRegisterResponse struct{
//    UserId int
//    RegisterMesg string     // 注册提示消息
//    RegisterSts bool        // 注册状态 True：成功， False：失败
//}

// 定义操作类
type UserOpSvc struct {}


// 实现接口服务
func (usrOp UserOpSvc) UserLogin (ctx context.Context, loginReq *pb.LoginRequest) (*pb.LoginReply, error) {
    resp := new(pb.LoginReply)

    fmt.Println(loginReq)

    if loginReq.RmbusrChk == true {
        resp.LoginSts = true
    } else {
        resp.LoginSts = false
    }

    resp.UserId = 1
    resp.LoginMgs = fmt.Sprintf("Login user: %s", loginReq.UserName)
    fmt.Println(resp)
    return resp, nil
}

func (usrOp UserOpSvc) UserRegister(ctx context.Context, regReq *pb.UserRegisterRequest) (*pb.UserRegisterReply, error) {
    resp := new(pb.UserRegisterReply)

    resp.RegisterMesg = fmt.Sprintf("reg user: %s", regReq.UserName)
    resp.UserId = 1
    resp.RegisterSts = true

    return resp, nil
}




