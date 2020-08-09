import sys

from PyQt5.QtWidgets import QApplication, QMainWindow
from Login import *

if __name__ == '__main__':

    app = QApplication(sys.argv)

    MainWindow = QMainWindow()

    ui = Ui_Form()

    ui.setupUi(MainWindow)

    MainWindow.show()

    sys.exit(app.exec_())
