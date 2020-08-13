#!/usr/bin/env python3

#
#  -*- coding:utf8 -*-
#  @Author  : chengms
#  @WebSite : chengms.com
#  @Email   : chengms2012@163.com
#  @TIME    : 2020/8/13 16:41
#  @File    : common.py
#


def VerifyEmailFormat(email):
    # 验证邮箱格式

    if 0 == len(email):
        return False

    # 格式必须为 xxx@xx.xx

    return True

