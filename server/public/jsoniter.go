package public

import (
    "fmt"
    jsoniter "github.com/json-iterator/go"
)

/*
*  -*- coding:utf8 -*-
*
*
*
*  @Author  : chengms
*  @WebSite : chengms.com
*  @Email   : chengms2012@163.com
*  @TIME    : 2020/8/19 11:22
*  @File    : jsoniter.go
 */

// 三方json组件
var Jsonit = jsoniter.ConfigCompatibleWithStandardLibrary


func TestJson() {
    type ColorGroup struct {
        ID  int
        Name string
        Colors []string
    }

    group := ColorGroup{
        ID:     1,
        Name:   "Reds",
        Colors: []string{"Crimson", "Red", "Ruby", "Maroon"},
    }
    //b, err := jsoniter.Marshal(group)
    //bb, err := jsoniter.MarshalIndent(group, "", " ")
    //if err != nil {
    //    fmt.Println("error: ", err)
    //}
    //
    ////os.Stdout.Write(b)
    //fmt.Println(string(b))
    ////os.Stdout.Write(bb)
    //fmt.Printf("%T", bb)

    // ===================  Deconde 解码 =================
    //jsoniter.NewDecoder(os.Stdin).Decode(&group)
    //fmt.Println(group)

    //reader := strings.NewReader(string(b))
    //decoder := json.NewDecoder(reader)
    params := ColorGroup{}
    //err = decoder.Decode(&params)
    cc, err := Jsonit.Marshal(group)
    fmt.Println(string(cc))
    err = Jsonit.Unmarshal(cc, &params)
    if err != nil {
        fmt.Println(err)
    } else {
        fmt.Printf("%+v\n", params)
    }
    fmt.Println(params.Colors)
}

