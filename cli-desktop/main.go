package main

import (
    "cli-desktop/tools"
    "cli-desktop/ui"
)

func init()  {
    tools.SetEnv()
}

func main() {
    app := new(ui.WinApp)
    app.Init()
    app.StartApp()


    // 取消中文字体
    tools.DisableEnv()
}
