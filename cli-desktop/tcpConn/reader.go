package tcpConn

import (
    "bufio"
    "fmt"
    "io"
    "strings"
)

/*
*  -*- coding:utf8 -*-
*
*
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/18 下午8:27
*  @File    : reader.go
 */



type CommandReader struct {
    reader *bufio.Reader
}

func NewCommandReader(reader io.Reader) *CommandReader {
    return &CommandReader{
        reader: bufio.NewReader(reader),
    }
}

//
func (r *CommandReader) Read() (cmd interface{}, err error) {
    line, _, err := r.reader.ReadLine()
    if err != nil {
        return
    }

    fmt.Println("Revc: ", string(line))

    parts := strings.Split(string(line), ProtocolSep)

    if len(parts) < 2 {
        err = InvalidMessageErr
        return
    }

    proVer := parts[0]
    proVerParts := strings.Split(strings.TrimSpace(proVer), "/")
    if len(proVerParts) != 2 {
        err = InvalidMessageErr
        return
    }
    protocol, version := proVerParts[0], proVerParts[1]

    if protocol != ProtocolName || version != ProtocolVersion {
        err = InvalidMessageErr
        return
    }

    fmt.Println(proVerParts)


    //base := service.BaseCmd{Protocol: protocol, Version: version}
    //
    //cmdName := strings.TrimSpace(parts[1])

    //switch cmdName {
    //case service.CmdSend:
    //    if len(parts) < 4 {
    //        err = service.InvalidMessageErr
    //        return
    //    }
    //
    //    name := strings.TrimSpace(parts[2])
    //    message := strings.Join(parts[3:], service.ProtocolSep)
    //
    //    cmd = &SendCommand{base, name, []byte(message)}
    //    return
    //case service.CmdBroadCase:
    //    if len(parts) < 4 {
    //        err = service.InvalidMessageErr
    //        return
    //    }
    //
    //    groupName := strings.TrimSpace(parts[2])
    //    message := strings.Join(parts[3:], service.ProtocolSep)
    //    cmd =&BroadCastCommand{base, groupName, []byte(message)}
    //case service.CmdLogin:
    //    if len(parts) != 3 {
    //        err = service.InvalidMessageErr
    //        return
    //    }
    //
    //    username := strings.TrimSpace(parts[2])
    //
    //    cmd = &service.LoginCommand{base, username}
    //case service.CmdLogout:
    //    if len(parts) != 2 {
    //        err = InvalidMessageErr
    //        return
    //    }
    //
    //    cmd = &LogoutCommand{base}
    //case service.CmdReceive:
    //    if len(parts) < 4 {
    //        err = InvalidMessageErr
    //        return
    //    }
    //
    //    f := strings.TrimSpace(parts[2])
    //    message := strings.Join(parts[3:], ProtocolSep)
    //
    //    cmd = &ReceiveCommand{base, f, []byte(message)}
    //case service.CmdGroup:
    //    if len(parts) < 5 {
    //        err = InvalidMessageErr
    //        return
    //    }
    //
    //    groupName := strings.TrimSpace(parts[2])
    //    userNames := parts[3:]
    //
    //    cmd = &GroupCommand{base, groupName, userNames}
    //case service.CmdLeave:
    //    if len(parts) < 3 {
    //        err = InvalidMessageErr
    //        return
    //    }
    //
    //    groupName := strings.TrimSpace(parts[2])
    //    cmd = &LeaveCommand{base, groupName}
    //default:
    //    err = service.UnsupportedCmdErr
    //}

    return
}