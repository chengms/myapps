#!/usr/bin/env python3

#
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/12 下午10:24
#  @File    : RegisterWin.py
#

import sys
from PyQt5.QtWidgets import QApplication, QLineEdit, QWidget
from PyQt5.QtGui import QIcon, QPalette, QColor, QPixmap

from pyUI.msgOp.checkData import *
from pyUI.pyWindow.ui.RegisterUI import *
from pyUI.tools.common import *


class RegisterWin(QWidget):
    userNameLength =2
    passwdLength = 2

    def __init__(self):
        super().__init__()

        self.RegisterUi = Ui_RegisterWd()
        self.RegisterUi.setupUi(self)
        self.initUI()

    def initUI(self):
        # the register ui init

        self.setWindowTitle("注册")
        # 设置窗口的图标
        self.setWindowIcon(QIcon("img/icon.png"))
        # 设置窗口背景
        palette = QPalette()
        palette.setColor(self.backgroundRole(), QColor(201, 241, 209))  # 设置背景颜色
        # palette.setBrush(self.backgroundRole(), QBrush(QPixmap('云.png')))   # 设置背景图片
        self.setPalette(palette)

        # 输入框设置
        # 设置默认显示
        self.RegisterUi.UserNameLineEdit.setPlaceholderText("用户名为数字、字母或特殊字符")
        self.RegisterUi.EmailLineEdit.setPlaceholderText("如：12345@qq.com")

        # 密码输入设置
        self.RegisterUi.passwdLineEdit.setPlaceholderText("密码可以为大小写字母，数字，特殊字符")
        # 设置密码输入显示为 圆点
        self.RegisterUi.passwdLineEdit.setEchoMode(QLineEdit.Password)
        self.RegisterUi.passwd2LineEdit.setPlaceholderText("请再次输入密码")
        self.RegisterUi.passwd2LineEdit.setEchoMode(QLineEdit.Password)

        # 设置按钮默认状态
        self.RegisterUi.RegisterPushButton.setDefault(True)
        self.RegisterUi.RegisterPushButton.clicked.connect(self.doRegister)

        self.setHideMsgLabel()

    def setHideMsgLabel(self):
        self.RegisterUi.userNameMsg.setVisible(False)
        self.RegisterUi.emalMsg.setVisible(False)
        self.RegisterUi.passwdMsg.setVisible(False)
        self.RegisterUi.passwd2Msg.setVisible(False)

    def checkUserName(self):
        # 检查用户名合法性
        userName = self.RegisterUi.UserNameLineEdit.text()
        if len(userName) < self.userNameLength:
            self.RegisterUi.userNameMsg.setText("请输入正确用户名！")
            # 设置提示字体为红色
            self.RegisterUi.userNameMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.userNameMsg.setVisible(True)
            return userName

    def checkEmail(self):
        # 检查邮箱
        email = self.RegisterUi.EmailLineEdit.text()
        if not VerifyEmailFormat(email):
            return email

    def checkPasswd(self):
        # 检查密码
        passwd = self.RegisterUi.passwdLineEdit.text()
        if len(passwd) < self.passwdLength:
            self.RegisterUi.passwdMsg.setText("密码长度太短！")
            # 设置提示字体为红色
            self.RegisterUi.passwdMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.passwdMsg.setVisible(True)
            return passwd

        passwd2 = self.RegisterUi.passwd2LineEdit.text()
        if passwd != passwd2:
            self.RegisterUi.passwd2Msg.setText("两次输入密码不一致！")
            # 设置提示字体为红色
            self.RegisterUi.passwd2Msg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.passwd2Msg.setVisible(True)
            return passwd

    def doRegister(self):
        # 提交前检查

        userName = self.checkUserName()
        email = self.checkEmail()
        passwd = self.checkPasswd()

        if not CheckRegisterData(userName, passwd, email):
            self.setHideMsgLabel()
            self.RegisterUi.userNameMsg.setText("用户名已被使用！")
            # 设置提示字体为红色
            self.RegisterUi.userNameMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.userNameMsg.setVisible(True)
            return

    def run(self):
        self.run()


def showRegisterWindow():
    app = QApplication(sys.argv)

    register = RegisterWin()
    register.show()

    sys.exit(app.exec_())

