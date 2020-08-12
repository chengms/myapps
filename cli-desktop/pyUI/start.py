#!/bin/python3

#  PyQt window start
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/10
#  @File    : start.py
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
