package tools

import (
    "fmt"
    "os"
    "strings"

    "github.com/flopp/go-findfont"
)

func SetEnv()  {
    //设置中文字体
    fontPaths := findfont.List()
    for _, path := range fontPaths {
        //fmt.Println(path)
        //楷体:simkai.ttf
        //黑体:simhei.ttf
        if strings.Contains(path, "simkai.ttf") {
            os.Setenv("FYNE_FONT", path)
            fmt.Println("FYNE_FONT: ", path)
            break
        }
    }
}

func DisableEnv()  {

    _ = os.Unsetenv("FYNE_FONT")
}

