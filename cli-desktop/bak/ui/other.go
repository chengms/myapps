package ui

import (
    "fyne.io/fyne"
    "fyne.io/fyne/widget"
)

type tappableIcon struct {
    widget.Icon
}

func newTappableIcon(res fyne.Resource) *tappableIcon {
    icon := &tappableIcon{}
    icon.ExtendBaseWidget(icon)
    icon.SetResource(res)

    return icon
}

