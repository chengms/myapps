package ui

import (
    "fmt"
    "fyne.io/fyne"
    "fyne.io/fyne/app"
    "fyne.io/fyne/canvas"
    "fyne.io/fyne/layout"
    "fyne.io/fyne/theme"
    "fyne.io/fyne/widget"
    "image/color"


)

type WinApp struct {
    UiApp fyne.App
}


func (w *WinApp) Init(){
    w.UiApp = app.New()

    // resource
    w.UiApp.SetIcon(theme.FyneLogo())

}


func (w *WinApp) StartApp()  {
    w.StartLogin()

    myWindow := w.UiApp.NewWindow("Container")

    text1 := canvas.NewText("Hello", color.White)
    text2 := canvas.NewText("There", color.White)
    text2.Move(fyne.NewPos(20, 20))
    text3 := canvas.NewText("World", color.White)
    container := fyne.NewContainer(text1, text2, text3)
    //	container := fyne.NewContainerWithLayout(layout.NewGridLayout(2),
    //		text1, text2, text3)

    myWindow.SetContent(container)
    myWindow.ShowAndRun()

}

func (w *WinApp) StartLogin()  {
    LoginWin := w.UiApp.NewWindow("Login")

    // set windows size
    LoginWin.Resize(fyne.NewSize(500,400))
    // show center a window on the screen
    LoginWin.CenterOnScreen()

    // 创建一个盒子
    content := widget.NewHBox()
    content.CreateRenderer()

    loginImg := canvas.NewImageFromFile("./cli-desktop/img/login.png")
    loginImg.SetMinSize(fyne.NewSize(500, 200))
    // 创建水平布显示图片
    loginImgCnt := widget.NewVBox(loginImg)
    // 创建水平布局将图片部件显示在主框中
    containerImg := fyne.NewContainerWithLayout(layout.NewHBoxLayout(), loginImgCnt)

    // 文字，登录框
    // username
    userNameText := canvas.NewText("用户名: ", color.Black)
    // 输入框
    userNameInput := widget.NewEntry()
    userNameInput.SetPlaceHolder(" 请输入用户名")
    // 水平布局
    containerUsr := fyne.NewContainerWithLayout(layout.NewHBoxLayout(), layout.NewSpacer(), userNameText, userNameInput, layout.NewSpacer())

    // password
    PasswordText := canvas.NewText("密  码: ", color.Black)
    // 输入框
    passwordInput := widget.NewPasswordEntry()
    passwordInput.SetPlaceHolder(" 请输入密码")
    // 水平布局
    containerPassword := fyne.NewContainerWithLayout(layout.NewHBoxLayout(), layout.NewSpacer(), PasswordText, passwordInput, layout.NewSpacer())

    // 登录按键
    LoginBtn := widget.NewButton("登录", func() {
        err := usrLogin(userNameInput.Text)
        if err != nil {
            fmt.Println("Login err")
        }
    })

    // 注册
    RegisterBtn := widget.NewButton("注册", func() {
        err := userRegister()
        if err != nil {
            fmt.Println("Register err")
        }
    })

    ContainerUsrLogin := widget.NewGroup("",
        fyne.NewContainerWithLayout(layout.NewGridLayout(3),
            layout.NewSpacer(),
            LoginBtn,
            layout.NewSpacer(),
        ),
    )

    ContainerUsrReg := widget.NewGroup("or",
        fyne.NewContainerWithLayout(layout.NewGridLayout(3),
            layout.NewSpacer(),
            RegisterBtn,
            layout.NewSpacer(),
        ),
    )

    // 加载布局
    LoginWin.SetContent(fyne.NewContainerWithLayout(layout.NewVBoxLayout(), containerImg, containerUsr, containerPassword, ContainerUsrLogin, ContainerUsrReg))
    LoginWin.ShowAndRun()
}

//dialog.ShowCustomConfirm("Login...", "Log In", "Cancel", content, func(b bool) {
//    if !b {
//        return
//    }
//
//    log.Println("Please Authenticate", username.Text, password.Text)
//}, win)