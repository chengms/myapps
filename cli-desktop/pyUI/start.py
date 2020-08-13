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

from PyQt5.QtWidgets import QApplication

from pyUI.pyWindow.LoginWin import *
from pyUI.connectSvc.connSvc import *


def WindowStart():
    # 界面程序开始
    app = QApplication(sys.argv)

    login = LoginWin()
    login.run()

    sys.exit(app.exec_())
