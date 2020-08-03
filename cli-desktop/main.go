package main

import "cli-desktop/ui"

func main() {
    app := new(ui.WinApp)
    app.Init()
    app.StartApp()

}
