package tools

import (
    yaml "gopkg.in/yaml.v2"
    "io/ioutil"
    "strconv"
)

/*
*  -*- coding:utf8 -*-
*
*
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/18 下午11:11
*  @File    : readConfig.go
 */

type Network struct {
    Host        string      `yaml:"host"`
    TcpPort     int         `yaml:"tpcPort"`
    //grpcPort    int
}

type LoggerInfo struct {
    InfoPath    string      `yaml:"infoPath"`
    ErrPath     string      `yaml:"errPath"`
    LoggerLevel string      `yaml:"loggerLevel"`
}

type Config struct {
    Name        string      `yaml:"name"`
    Network     Network     `yaml:"Network"`
    LoggerInfo  LoggerInfo  `yaml:"Logger"`
}

func (cfg *Config)GetConfig() {
    bytes, err := ioutil.ReadFile("conf/config.yaml")
    if err != nil {
        panic(err)
    }

    //err = yaml.Unmarshal(bytes, cfg)
    err = yaml.UnmarshalStrict(bytes, cfg)
    if err != nil {
        panic(err)
    }

}

func (cfg *Config)GetTcpAddr() string {
    return cfg.Network.Host + ":" + strconv.Itoa(cfg.Network.TcpPort)
}


var Cfg Config

func InitCfg()  {
    Cfg.GetConfig()
}
