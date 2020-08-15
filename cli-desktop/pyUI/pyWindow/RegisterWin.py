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
from PyQt5.QtWidgets import QApplication, QLineEdit, QWidget, QDialog
from PyQt5.QtGui import QIcon, QPalette, QColor, QPixmap

from pyUI.msgOp.checkData import *
from pyUI.pyWindow.messageBox import RegisterSuccessWin
from pyUI.pyWindow.ui.RegisterUI import *
from pyUI.tools.common import *


class RegisterWin(QDialog):

    def __init__(self):
        super().__init__()

        self.userNameLength = 2
        self.passwdLength = 2
        self.RegisterUi = Ui_RegisterWd()
        self.RegisterUi.setupUi(self)
        self.initUI()

        # 初始化注册成功提示界面
        self.RegisterSuccessWin = RegisterSuccessWin()

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
        self.RegisterUi.UserNameLineEdit.setPlaceholderText("用户名为6位以上数字、字母或特殊字符")
        # 设置输入时隐藏提示框
        self.RegisterUi.UserNameLineEdit.textChanged.connect(self.setHideUserMsg)
        self.RegisterUi.EmailLineEdit.setPlaceholderText("如：abc123@gmail.com")
        self.RegisterUi.EmailLineEdit.textChanged.connect(self.setHideEmalMsg)

        # 密码输入设置
        self.RegisterUi.passwdLineEdit.setPlaceholderText("密码可以为大小写字母，数字，特殊字符")
        # 设置密码输入显示为 圆点
        self.RegisterUi.passwdLineEdit.setEchoMode(QLineEdit.Password)
        self.RegisterUi.passwdLineEdit.textChanged.connect(self.setHidePasswdMsg)
        self.RegisterUi.passwd2LineEdit.setPlaceholderText("请再次输入密码")
        self.RegisterUi.passwd2LineEdit.setEchoMode(QLineEdit.Password)
        self.RegisterUi.passwd2LineEdit.textChanged.connect(self.setHidePasswd2Msg)

        # 设置按钮默认状态
        self.RegisterUi.RegisterPushButton.setDefault(True)
        self.RegisterUi.RegisterPushButton.clicked.connect(self.doRegister)

        # 重置按钮
        # 设置控件是否为突出效果
        self.RegisterUi.RestButton.setFlat(True)
        # 设置自动填充背景
        self.RegisterUi.RestButton.setAutoFillBackground(True)
        self.RegisterUi.RestButton.clicked.connect(self.reset)

        # 隐藏提示标签
        self.setHideMsgLabel()

        # test
        # self.RegisterUi.UserNameLineEdit.setText("111")
        # self.RegisterUi.EmailLineEdit.setText("111")
        # self.RegisterUi.passwdLineEdit.setText("111")
        # self.RegisterUi.passwd2LineEdit.setText("111")

    def setHideUserMsg(self):
        self.RegisterUi.userNameMsg.setVisible(False)

    def setHideEmalMsg(self):
        self.RegisterUi.emalMsg.setVisible(False)

    def setHidePasswdMsg(self):
        self.RegisterUi.passwdMsg.setVisible(False)

    def setHidePasswd2Msg(self):
        self.RegisterUi.passwd2Msg.setVisible(False)

    def setHideMsgLabel(self):
        self.RegisterUi.userNameMsg.setVisible(False)
        self.RegisterUi.emalMsg.setVisible(False)
        self.RegisterUi.passwdMsg.setVisible(False)
        self.RegisterUi.passwd2Msg.setVisible(False)

    def checkUserName(self):
        # 检查用户名合法性
        userName = self.RegisterUi.UserNameLineEdit.text()

        if len(userName) < self.userNameLength:
            self.RegisterUi.userNameMsg.setText("用户名过短！")
            # 设置提示字体为红色
            self.RegisterUi.userNameMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.userNameMsg.setVisible(True)
            return None, False

        return userName, True

    def checkEmail(self):
        # 检查邮箱
        email = self.RegisterUi.EmailLineEdit.text()
        if not VerifyEmailFormat(email):
            self.RegisterUi.emalMsg.setText("请输入正确的邮箱，如：abc123@gmail.com")
            # 设置提示字体为红色
            self.RegisterUi.emalMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.emalMsg.setVisible(True)
            return None, False

        return email, True

    def checkPasswd(self):
        # 检查密码
        passwd = self.RegisterUi.passwdLineEdit.text()
        if len(passwd) < self.passwdLength:
            self.RegisterUi.passwdMsg.setText("密码长度太短！ 长度 > 6")
            # 设置提示字体为红色
            self.RegisterUi.passwdMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.passwdMsg.setVisible(True)
            return None, False

        passwd2 = self.RegisterUi.passwd2LineEdit.text()
        if passwd != passwd2:
            self.RegisterUi.passwd2Msg.setText("两次输入密码不一致！")
            # 设置提示字体为红色
            self.RegisterUi.passwd2Msg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.passwd2Msg.setVisible(True)
            return None, False

        return passwd, True

    def doRegister(self):
        # 提交前检查

        userName, res = self.checkUserName()
        if not res:
            return
        email, res = self.checkEmail()
        if not res:
            return
        passwd, res = self.checkPasswd()
        if not res:
            return

        if not CheckRegisterData(userName, passwd, email):
            self.setHideMsgLabel()
            self.RegisterUi.userNameMsg.setText("用户名已被使用！")
            # 设置提示字体为红色
            self.RegisterUi.userNameMsg.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.RegisterUi.userNameMsg.setVisible(True)
            return

        # 注册成功，提示，并关闭
        self.RegisterSuccessWin.initUI()   # 设置定时器启动
        self.RegisterSuccessWin.run()
        self.close()

    def reset(self):
        print("reset...")
        # self.RegisterUi.UserNameLineEdit.setText("")
        self.RegisterUi.UserNameLineEdit.clear()
        self.RegisterUi.EmailLineEdit.clear()
        self.RegisterUi.passwdLineEdit.clear()
        self.RegisterUi.passwd2LineEdit.clear()

    def run(self):
        self.show()
        self.exec_()


# def showRegisterWindow():
#     register = RegisterWin()
#     register.show()

