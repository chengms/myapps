#!/bin/python3

# PyQt window start
#
#

import sys
import os
from pyUI.pyWindow.LoginWin import *
from pyUI.connectSvc.connSvc import *


def main():
    print("start...")
    os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
    # app = QApplication(sys.argv)

    showLoginWindow()

    # sys.exit(app.exec_())
    # UserLoginOp()


if __name__ == '__main__':
    main()
