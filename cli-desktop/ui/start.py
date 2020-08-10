# PyQt window start
#
#

import sys

from ui.pyWindow.LoginWin import *


def main():
    print("start...")

    app = QApplication(sys.argv)

    showLoginWindow()

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
