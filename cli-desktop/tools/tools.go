package tools

import "os"

func SetEnv()  {
    //设置中文字体
    _ = os.Setenv("FYNE_FONT", "./cli-desktop/ttf/SourceHanSerifCN-Regular.ttf")
}

func DisableEnv()  {

    _ = os.Unsetenv("FYNE_FONT")
}

