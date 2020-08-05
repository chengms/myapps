package ui

import (
    "fyne.io/fyne"
    "fyne.io/fyne/app"
    "fyne.io/fyne/canvas"
    "fyne.io/fyne/layout"
    "fyne.io/fyne/theme"
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
    //LoginWin := w.UiApp.NewWindow("Login")
    myWindow := w.UiApp.NewWindow("Box Layout")

    text1 := canvas.NewText("Hello", color.Black)
    text2 := canvas.NewText("There", color.Black)
    text3 := canvas.NewText("(right)", color.Black)
    container := fyne.NewContainerWithLayout(layout.NewHBoxLayout(),
        text1, text2, layout.NewSpacer(), text3)

    text4 := canvas.NewText("centered", color.Black)
    centered := fyne.NewContainerWithLayout(layout.NewHBoxLayout(),
        layout.NewSpacer(), text4, layout.NewSpacer())
    myWindow.SetContent(fyne.NewContainerWithLayout(layout.NewVBoxLayout(), container, centered))
    myWindow.ShowAndRun()
}
