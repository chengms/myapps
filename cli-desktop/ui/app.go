package ui

import (
    "fyne.io/fyne"
    "fyne.io/fyne/app"
    "fyne.io/fyne/layout"
    "fyne.io/fyne/theme"
    "fyne.io/fyne/widget"
)

type WinApp struct {
    UiApp fyne.App
}


func (a *WinApp) Init(){
    a.UiApp = app.New()

    // resource
    a.UiApp.SetIcon(theme.FyneLogo())

}


func (a *WinApp) StartApp()  {
    a.StartLogin()

    //myWindow := a.UiApp.NewWindow("Container")
    //
    //text1 := canvas.NewText("Hello", color.White)
    //text2 := canvas.NewText("There", color.White)
    //text2.Move(fyne.NewPos(20, 20))
    //text3 := canvas.NewText("World", color.White)
    //container := fyne.NewContainer(text1, text2, text3)
    ////	container := fyne.NewContainerWithLayout(layout.NewGridLayout(2),
    ////		text1, text2, text3)
    //
    //myWindow.SetContent(container)
    //myWindow.ShowAndRun()

}

func (a *WinApp) StartLogin()  {
    LoginWin := a.UiApp.NewWindow("Login")

    // set win title
    LoginWin.SetTitle("Login")

    widget.NewLabelWithStyle("Welcome to the Fyne toolkit demo app", fyne.TextAlignCenter, fyne.TextStyle{Bold: true}),
        widget.NewHBox(layout.NewSpacer(), logo, layout.NewSpacer()),

        widget.NewHBox(layout.NewSpacer(),
            widget.NewHyperlink("fyne.io", parseURL("https://fyne.io/")),
            widget.NewLabel("-"),
            widget.NewHyperlink("documentation", parseURL("https://fyne.io/develop/")),
            widget.NewLabel("-"),
            widget.NewHyperlink("sponsor", parseURL("https://github.com/sponsors/fyne-io")),
            layout.NewSpacer(),
        ),



    // show this window
    LoginWin.ShowAndRun()
}
