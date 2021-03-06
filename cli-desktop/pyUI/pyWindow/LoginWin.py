# Show Login Window

from PyQt5.QtGui import QIcon, QPalette, QColor, QPixmap
from PyQt5.QtWidgets import QWidget, QLineEdit, QStyleFactory

from pyUI.pyWindow.ui.LoginUI import Ui_LoginWd
from pyUI.pyWindow.RegisterWin import RegisterWin
from pyUI.msgOp.checkData import *

# def doRegister():
#     print("Register")
#     showRegisterWindow()


class LoginWin(QWidget):

    def __init__(self):
        super().__init__()

        self.userNameMinLength = 2
        self.passwdMinLength = 2
        # 绘制界面
        # 设置居中显示
        # self.center()
        # 加载 绘图文件
        self.LoginUi = Ui_LoginWd()
        self.LoginUi.setupUi(self)
        # 初始化注册界面
        self.RegisterWin = RegisterWin()
        self.initUI()

    def initUI(self):
        # 设置 ui 相关属性

        self.setWindowTitle("Login")
        # 设置窗口的图标
        self.setWindowIcon(QIcon("img/icon.png"))
        # 设置窗口背景
        palette = QPalette()
        palette.setColor(self.backgroundRole(), QColor(201, 241, 209))  # 设置背景颜色
        # palette.setBrush(self.backgroundRole(), QBrush(QPixmap('云.png')))   # 设置背景图片
        self.setPalette(palette)

        # 设置 label 标签图片
        LabelPixmap = QPixmap("img/login.png")
        self.LoginUi.Imglabel.setPixmap(LabelPixmap)
        # 设置图片和标签大小相匹配
        self.LoginUi.Imglabel.setScaledContents(True)
        # 也可以这么设置
        # LabelPixmap = QPixmap("img/login.png").scaled(self.LoginUi.Imglabel.width(), self.LoginUi.Imglabel.height())

        # 输入框设置
        # 设置默认显示
        self.LoginUi.userNameInput.setPlaceholderText("用户名为数字、字母或特殊字符")
        # 输入框变化时绑定信号
        self.LoginUi.userNameInput.textChanged.connect(self.userNameChanged)
        self.LoginUi.passwdInput.setPlaceholderText("密码可以为大小写字母，数字，特殊字符")
        # 设置密码输入显示为 圆点
        self.LoginUi.passwdInput.setEchoMode(QLineEdit.Password)
        #  密码框输入时信号框
        self.LoginUi.passwdInput.textChanged.connect(self.passwdChanged)

        # 信息提示框默认隐藏
        self.LoginUi.messageLabel.setVisible(False)

        # 设置复选框默认选中状态
        self.LoginUi.rememberUserName.setChecked(True)

        # 设置登录按钮点击信号绑定
        self.LoginUi.LoginButton.clicked.connect(self.doLogin)
        # 设置按钮默认状态
        self.LoginUi.LoginButton.setDefault(True)

        # 注册按钮
        # 设置控件是否为突出效果
        self.LoginUi.RegisterButton.setFlat(True)
        # 设置自动填充背景
        self.LoginUi.RegisterButton.setAutoFillBackground(True)
        self.LoginUi.RegisterButton.clicked.connect(self.doOpenRegisterWin)

    def userNameChanged(self):
        self.LoginUi.messageLabel.setVisible(False)

    def passwdChanged(self):
        self.LoginUi.messageLabel.setVisible(False)

    def doLogin(self):
        # 点击登录后的动作
        print("login..")

        userName = self.LoginUi.userNameInput.text()
        passwd = self.LoginUi.passwdInput.text()
        # 检测输入框状态
        if userName.__len__() < self.userNameMinLength:
            self.LoginUi.messageLabel.setText("请输入完整用户名！")
            # 设置提示字体为红色
            self.LoginUi.messageLabel.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.LoginUi.messageLabel.setVisible(True)
            return

        if passwd.__len__() < self.passwdMinLength:
            self.LoginUi.messageLabel.setText("请输入密码！")
            # 设置提示字体为红色
            self.LoginUi.messageLabel.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.LoginUi.messageLabel.setVisible(True)
            return

        # 获取记住用户名选项
        saveUserNameSts = self.LoginUi.rememberUserName.isChecked()
        # 获取记住密码选项
        savePasswdSts = self.LoginUi.rememberPasswd.isChecked()

        # check username and password
        if not CheckLoginData(userName, passwd, saveUserNameSts, savePasswdSts):
            self.LoginUi.messageLabel.setText("请输入正确的用户名或密码")
            # 设置提示字体为红色
            self.LoginUi.messageLabel.setStyleSheet("QLabel{color:rgb(255,17,17,255);}")
            # 显示提示框
            self.LoginUi.messageLabel.setVisible(True)

        # 登录成功，隐藏登录界面，显示主界面
        # ShouMainWindow()

    def doOpenRegisterWin(self):
        # 打开注册界面

        print("Open Register...\n")
        # 隐藏自身
        self.hide()
        # 显示注册窗口
        self.RegisterWin.run()
        self.show()

    def run(self):
        # 显示界面
        self.show()
