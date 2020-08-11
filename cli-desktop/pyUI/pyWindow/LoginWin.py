# Show Login Window

from PyQt5.QtWidgets import QApplication, QMainWindow, QLineEdit

from pyUI.pyWindow.LoginUI import Ui_LoginWd


def showLoginWindow():
    LoginMainWindow = QMainWindow()   # 新建窗口

    LoginUi = Ui_LoginWd()   # 加载 绘图文件

    LoginUi.setupUi(LoginMainWindow)
    LoginUi.passwdInput.setEchoMode(QLineEdit.Password)  # 设置密码输入显示为 圆点

    # 设置点击信号绑定

    # 显示界面
    LoginMainWindow.show()
