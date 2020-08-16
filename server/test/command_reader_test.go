package test

import (
    "bytes"
    "testing"

    "server/service"
)

/*
*  -*- coding:utf8 -*-
*
*
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/16 下午7:16
*  @File    : command_reader_test.go
 */

// func TestSendMessage(t *testing.T)  {
//     case := []struct{
//         message     string
//         expectedErr  error
//         expectedName string
//         expectedData []byte
//     }{
//         {
//             "CHAT/1.0 SEND zhenghe hello world\n",
//             nil,
//             "zhenghe",
//             []byte("hello world"),
//         },
//     }
//
//     for i, c := range cases {
//         mr := NewCommandReader(strings.NewReader(c.message))
//
//         cmd, err := mr.Read()
//         if err != c.expectedErr {
//             t.Errorf("case %d: should have err:%v got:%v",
//             i, c.expectedErr, err)
//         }
//
//         if err == nil {
//         sendCmd := cmd.(*SendCommand)
//         if sendCmd.Name != c.expectedName {
//         t.Errorf("case %d: should have name:%s got:%s",
//         i, c.expectedName, sendCmd.Name)
//         }
//
//         if !reflect.DeepEqual(sendCmd.Data, c.expectedData) {
//         t.Errorf("case %d: should have data:%v got:%v",
//         i, c.expectedData, sendCmd.Data)
//         }
//         }
//     }
// }

func TestWriteSendMessage(t *testing.T) {
    cases := []struct {
        cmd         *service.SendCommand
        expectedMessage string
    }{
        {
            &service.SendCommand{
                BaseCmd: service.BaseCmd{service.ProtocolName, service.ProtocolVersion},
                Name:        "zhenghe",
                Data:        []byte("hello world"),
            },
            "CHAT/1.0 SEND zhenghe hello world\n",
        },
    }

    for i, c := range cases {
        buf := bytes.NewBuffer([]byte{})
        mw := service.NewCommandWriter(buf)

        _ = mw.Write(c.cmd)

        if buf.String() != c.expectedMessage {
            t.Errorf("Case %d: expect message:%s got:%s",
                i, c.expectedMessage, buf.String())
        }
    }
}

