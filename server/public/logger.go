package public

/*
*  -*- coding:utf8 -*-
*
*  日志操作设置封装
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/15 下午10:45
*  @File    : logger.go
 */

import (
    "fmt"
    "go.uber.org/zap"
)

func InitLogger() {
    fmt.Println("Init logger...")
}

func LoggerTest() {
    log := zap.NewExample()
    log.Debug("this is debug message")
    log.Info("this is info message")
    log.Info("this is info message with fileds",
        zap.Int("age", 24), zap.String("agender", "man"))
    log.Warn("this is warn message")
    log.Error("this is error message")
    log.Panic("this is panic message")

}

