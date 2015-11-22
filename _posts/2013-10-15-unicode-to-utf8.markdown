---
author: admin
comments: true
date: 2013-10-15 09:34:44+00:00
excerpt: 將包含Unicode(\uXXXX)編碼格式的國際化文件轉爲UTF8編碼
layout: post
slug: unicode-to-utf8
title: 將包含Unicode(\uXXXX)編碼格式的國際化文件轉爲UTF8編碼
wordpress_id: 524
categories:
- Experience
tags:
- i18n
- native2ascii
- unicode
- utf8
---

Java JDK中提供了一個很實用的工具native2ascii可以實現此操作：


    native2ascii -encoding UTF-8 -reverse package_zh_CN.properties package_en.properties


在將產出的package_en.properties翻譯為英文，即可得到一份英文國際化文件！

參考命令説明:[http://docs.oracle.com/javase/1.5.0/docs/tooldocs/windows/native2ascii.html](http://docs.oracle.com/javase/1.5.0/docs/tooldocs/windows/native2ascii.html)

