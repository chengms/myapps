import sys

from PyQt5.QtWidgets import QApplication, QMainWindow, QLineEdit

from ui.pyWindow.LoginUI import Ui_LoginWd


def showLoginWindow():
    app = QApplication(sys.argv)

    LoginMainWindow = QMainWindow()

    LoginUi = Ui_LoginWd()

    LoginUi.setupUi(LoginMainWindow)
    LoginUi.passwdInput.setEchoMode(QLineEdit.Password)  # 设置密码输入显示为 圆点

    LoginMainWindow.show()

    sys.exit(app.exec_())





