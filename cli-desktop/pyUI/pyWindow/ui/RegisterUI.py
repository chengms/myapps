# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'RegisterUI.ui'
#
# Created by: PyQt5 UI code generator 5.15.0
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_RegisterWd(object):
    def setupUi(self, RegisterWd):
        RegisterWd.setObjectName("RegisterWd")
        RegisterWd.resize(560, 600)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Maximum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(RegisterWd.sizePolicy().hasHeightForWidth())
        RegisterWd.setSizePolicy(sizePolicy)
        self.centralwidget = QtWidgets.QWidget(RegisterWd)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(60, 110, 82, 251))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setSizeConstraint(QtWidgets.QLayout.SetMaximumSize)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        spacerItem = QtWidgets.QSpacerItem(20, 10, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Maximum)
        self.verticalLayout.addItem(spacerItem)
        self.userNameLabel = QtWidgets.QLabel(self.verticalLayoutWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.userNameLabel.sizePolicy().hasHeightForWidth())
        self.userNameLabel.setSizePolicy(sizePolicy)
        self.userNameLabel.setMinimumSize(QtCore.QSize(0, 40))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.userNameLabel.setFont(font)
        self.userNameLabel.setObjectName("userNameLabel")
        self.verticalLayout.addWidget(self.userNameLabel)
        self.EmaiLlabel = QtWidgets.QLabel(self.verticalLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.EmaiLlabel.setFont(font)
        self.EmaiLlabel.setObjectName("EmaiLlabel")
        self.verticalLayout.addWidget(self.EmaiLlabel)
        self.passwdLabel = QtWidgets.QLabel(self.verticalLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.passwdLabel.setFont(font)
        self.passwdLabel.setObjectName("passwdLabel")
        self.verticalLayout.addWidget(self.passwdLabel)
        self.passwdLabel2 = QtWidgets.QLabel(self.verticalLayoutWidget)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.passwdLabel2.setFont(font)
        self.passwdLabel2.setObjectName("passwdLabel2")
        self.verticalLayout.addWidget(self.passwdLabel2)
        spacerItem1 = QtWidgets.QSpacerItem(20, 5, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Maximum)
        self.verticalLayout.addItem(spacerItem1)
        self.verticalLayoutWidget_2 = QtWidgets.QWidget(self.centralwidget)
        self.verticalLayoutWidget_2.setGeometry(QtCore.QRect(140, 110, 291, 251))
        self.verticalLayoutWidget_2.setObjectName("verticalLayoutWidget_2")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.verticalLayoutWidget_2)
        self.verticalLayout_2.setSizeConstraint(QtWidgets.QLayout.SetMaximumSize)
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.UserNameLineEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget_2)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.UserNameLineEdit.sizePolicy().hasHeightForWidth())
        self.UserNameLineEdit.setSizePolicy(sizePolicy)
        self.UserNameLineEdit.setMinimumSize(QtCore.QSize(279, 40))
        self.UserNameLineEdit.setMaximumSize(QtCore.QSize(16777215, 20))
        self.UserNameLineEdit.setObjectName("UserNameLineEdit")
        self.verticalLayout_2.addWidget(self.UserNameLineEdit)
        self.EmailLineEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget_2)
        self.EmailLineEdit.setMinimumSize(QtCore.QSize(0, 40))
        self.EmailLineEdit.setMaximumSize(QtCore.QSize(16777215, 20))
        self.EmailLineEdit.setObjectName("EmailLineEdit")
        self.verticalLayout_2.addWidget(self.EmailLineEdit)
        self.passwdLineEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget_2)
        self.passwdLineEdit.setMinimumSize(QtCore.QSize(0, 40))
        self.passwdLineEdit.setObjectName("passwdLineEdit")
        self.verticalLayout_2.addWidget(self.passwdLineEdit)
        self.passwd2LineEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget_2)
        self.passwd2LineEdit.setMinimumSize(QtCore.QSize(0, 40))
        self.passwd2LineEdit.setObjectName("passwd2LineEdit")
        self.verticalLayout_2.addWidget(self.passwd2LineEdit)
        self.RegisterPushButton = QtWidgets.QPushButton(self.centralwidget)
        self.RegisterPushButton.setGeometry(QtCore.QRect(130, 400, 201, 61))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.RegisterPushButton.setFont(font)
        self.RegisterPushButton.setObjectName("RegisterPushButton")
        self.ResetLabel = QtWidgets.QLabel(self.centralwidget)
        self.ResetLabel.setGeometry(QtCore.QRect(360, 390, 71, 41))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.ResetLabel.setFont(font)
        self.ResetLabel.setObjectName("ResetLabel")
        self.userNameMsg = QtWidgets.QLabel(self.centralwidget)
        self.userNameMsg.setGeometry(QtCore.QRect(440, 130, 111, 71))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.userNameMsg.setFont(font)
        self.userNameMsg.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.userNameMsg.setWordWrap(True)
        self.userNameMsg.setObjectName("userNameMsg")
        self.emalMsg = QtWidgets.QLabel(self.centralwidget)
        self.emalMsg.setGeometry(QtCore.QRect(440, 190, 111, 71))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.emalMsg.setFont(font)
        self.emalMsg.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.emalMsg.setWordWrap(True)
        self.emalMsg.setObjectName("emalMsg")
        self.passwdMsg = QtWidgets.QLabel(self.centralwidget)
        self.passwdMsg.setGeometry(QtCore.QRect(440, 240, 111, 71))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.passwdMsg.setFont(font)
        self.passwdMsg.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.passwdMsg.setWordWrap(True)
        self.passwdMsg.setObjectName("passwdMsg")
        self.passwd2Msg = QtWidgets.QLabel(self.centralwidget)
        self.passwd2Msg.setGeometry(QtCore.QRect(440, 300, 111, 71))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.passwd2Msg.setFont(font)
        self.passwd2Msg.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.passwd2Msg.setWordWrap(True)
        self.passwd2Msg.setObjectName("passwd2Msg")
        RegisterWd.setCentralWidget(self.centralwidget)

        self.retranslateUi(RegisterWd)
        QtCore.QMetaObject.connectSlotsByName(RegisterWd)

    def retranslateUi(self, RegisterWd):
        _translate = QtCore.QCoreApplication.translate
        RegisterWd.setWindowTitle(_translate("RegisterWd", "MainWindow"))
        self.userNameLabel.setText(_translate("RegisterWd", "用 户 名："))
        self.EmaiLlabel.setText(_translate("RegisterWd", "邮   箱 ："))
        self.passwdLabel.setText(_translate("RegisterWd", "密   码 ："))
        self.passwdLabel2.setText(_translate("RegisterWd", "确认密码："))
        self.RegisterPushButton.setText(_translate("RegisterWd", "注册"))
        self.ResetLabel.setText(_translate("RegisterWd", "重置信息"))
        self.userNameMsg.setText(_translate("RegisterWd", "userName"))
        self.emalMsg.setText(_translate("RegisterWd", "email"))
        self.passwdMsg.setText(_translate("RegisterWd", "passwd"))
        self.passwd2Msg.setText(_translate("RegisterWd", "passwd2"))
