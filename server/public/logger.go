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
    //"log"

    "github.com/natefinch/lumberjack"
    //rotatelogs "github.com/lestrrat-go/file-rotatelogs"
    "go.uber.org/zap"
    "go.uber.org/zap/zapcore"
    "io"
    "os"
    "time"
)

//只能输出结构化日志，但是性能要高于 SugaredLogger
var logger *zap.Logger
//可以输出 结构化日志、非结构化日志。性能差于 zap.Logger，具体可见上面的的单元测试
var Loggerf *zap.SugaredLogger


func InitLogger() {
    logLevel := zapcore.DebugLevel
    switch Cfg.LoggerInfo.LoggerLevel {
    case "DEBUG":
        logLevel = zap.DebugLevel
    case "INFO":
        logLevel = zapcore.InfoLevel
    case "WARN":
        logLevel = zapcore.WarnLevel
    case "ERROR":
        logLevel = zapcore.ErrorLevel
    case "DPANIC":
        logLevel = zapcore.DPanicLevel
    default:
        logLevel = zapcore.InfoLevel
    }

    InitLog(
        Cfg.LoggerInfo.InfoPath,
        Cfg.LoggerInfo.ErrPath,
        logLevel,
    )
    Loggerf.Debug("Logger module init successfully")
    Loggerf.Info("----------------------------------------------------------------------------------------")
}

func CloseLogger()  {
    _ = Loggerf.Sync()
    _ = logger.Sync()
}

func LoggerTest() {

    Loggerf.Infof("sugarLogger name:%s", "meng")
    Loggerf.Infow("sugarLogger", zap.String("name", "meng"))
    Loggerf.Errorf("sugarLogger name:%s", "meng")
    Loggerf.Debugf("sugarLogger name:%s", "meng")
    Loggerf.Warnf("sugarLogger name:%s", "meng")

    logger.Info("logger", zap.String("name", "meng"))
    logger.Error("logger", zap.String("name", "meng"))
    logger.Debug("logger", zap.String("name", "meng"))

}

// 初始化日志 logger
func InitLog(logPath, errPath string, logLevel zapcore.Level) {
    config := zapcore.EncoderConfig{
        MessageKey:   "msg",  //结构化（json）输出：msg的key
        LevelKey:     "level",//结构化（json）输出：日志级别的key（INFO，WARN，ERROR等）
        TimeKey:      "time",   //结构化（json）输出：时间的key（INFO，WARN，ERROR等）
        NameKey:      "logger",
        CallerKey:    "file", //结构化（json）输出：打印日志的文件对应的Key
        EncodeLevel:  zapcore.CapitalLevelEncoder, //将日志级别转换成大写（INFO，WARN，ERROR等）
        EncodeCaller: zapcore.ShortCallerEncoder, //采用短文件路径编码输出
        EncodeTime: func(t time.Time, enc zapcore.PrimitiveArrayEncoder) {
            enc.AppendString(t.Format("2006-01-02 15:04:05"))
        },//输出的时间格式
        EncodeDuration: func(d time.Duration, enc zapcore.PrimitiveArrayEncoder) {
            enc.AppendInt64(int64(d) / 1000000)
        },//
    }

    //自定义日志级别：自定义Info级别
    infoLevel := zap.LevelEnablerFunc(func(lvl zapcore.Level) bool {
        //return lvl < zapcore.WarnLevel && lvl >= logLevel      // 输出info级别及以下，若设置debug，也会输出至日志
        return lvl < zapcore.WarnLevel && lvl >= zap.InfoLevel   // 自定义Info级别输出至文件
    })

    //自定义日志级别：自定义Warn级别
    warnLevel := zap.LevelEnablerFunc(func(lvl zapcore.Level) bool {
        return lvl >= zapcore.WarnLevel && lvl >= logLevel
    })

    // 获取io.Writer的实现
    infoWriter := getWriter(logPath)
    warnWriter := getWriter(errPath)

    //将info及以下写入logPath，NewConsoleEncoder 是非结构化输出
    infoCore := zapcore.NewCore(zapcore.NewConsoleEncoder(config), zapcore.AddSync(infoWriter), infoLevel)

    //warn及以上写入errPath
    warnCore := zapcore.NewCore(zapcore.NewConsoleEncoder(config), zapcore.AddSync(warnWriter), warnLevel)

    //同时将日志输出到控制台，NewJSONEncoder 是结构化输出
    //stdoutCore := zapcore.NewCore(zapcore.NewJSONEncoder(config), zapcore.NewMultiWriteSyncer(zapcore.AddSync(os.Stdout)), logLevel)
    //同时将日志输出到控制台, 输出字符串
    stdoutCore := zapcore.NewCore(zapcore.NewConsoleEncoder(config), zapcore.NewMultiWriteSyncer(zapcore.AddSync(os.Stdout)), logLevel)

    // 实现多个输出
    core := zapcore.NewTee(infoCore, warnCore, stdoutCore)

    logger = zap.New(core, zap.AddCaller(), zap.AddStacktrace(zap.WarnLevel))
    Loggerf = logger.Sugar()

}

// 使用 "github.com/natefinch/lumberjack" 框架
func getWriter(filename string) io.Writer {
 return &lumberjack.Logger{
     Filename:   filename,
     MaxSize:    10,  //最大M数，超过则切割
     MaxBackups: 10,   //最大文件保留数，超过就删除最老的日志文件
     MaxAge:     30,  //保存30天
     Compress:   false,//是否压缩
 }
}

// 使用 rotatelogs "github.com/lestrrat-go/file-rotatelogs"
//func getWriter(filename string) io.Writer {
//   // 生成rotatelogs的Logger 实际生成的文件名 filename.YYmmddHH
//   // filename是指向最新日志的链接
//   hook, err := rotatelogs.New(
//       filename+".%Y%m%d%H"+".log",
//       rotatelogs.WithLinkName(filename),
//       rotatelogs.WithMaxAge(time.Hour*24*30),    // 保存30天
//       rotatelogs.WithRotationTime(time.Hour*24), //切割频率 24小时
//   )
//   if err != nil {
//       log.Println("日志启动异常")
//       panic(err)
//   }
//   return hook
//}



