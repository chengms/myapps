#!/usr/bin/env python3

#
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/14 10:24
#  @File    : messageBox.py
#


from PyQt5.QtWidgets import QDialog, QLabel, QMessageBox
from PyQt5.QtCore import QTimer
from pyUI.pyWindow.ui.RegisterSuccessUI import *


class RegisterSuccessWin(QDialog):
    # 注册成功提示框

    def __init__(self):
        super().__init__()

        self.RegisterSuccessUi = Ui_RegisterSuccess()
        self.RegisterSuccessUi.setupUi(self)

        self.timeNum = 5
        self.timer = QTimer(self)  # 初始化一个定时器

        # self.initUI()

    def initUI(self):
        # 关闭按钮
        self.RegisterSuccessUi.closeButton.clicked.connect(self.closeWin)

        self.RegisterSuccessUi.timeLabel.setText(str(5))
        self.timer.timeout.connect(self.timerOperate)  # 每次计时到时间时发出信号
        self.timer.start(1000)  # 设置计时间隔并启动；单位毫秒

    def timerOperate(self):
        self.timeNum -= 1
        self.RegisterSuccessUi.timeLabel.setText(str(self.timeNum))
        if 0 == self.timeNum:
            self.timer.killTimer(self.timer.timerId())
            self.close()

    def closeWin(self):
        self.close()

    def run(self):
        self.show()
        self.exec_()
