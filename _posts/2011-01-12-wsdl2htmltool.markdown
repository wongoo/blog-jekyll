---
author: admin
comments: true
date: 2011-01-12 09:22:29+00:00
layout: post
slug: wsdl2htmltool
title: WSDL转HTML工具
wordpress_id: 59
categories:
- Experience
---

## 1. WSDL viewer - 方便易用


[http://tomi.vanek.sk/index.php?page=wsdl-viewer](http://tomi.vanek.sk/index.php?page=wsdl-viewer) 可在线进行转换

[http://code.google.com/p/wsdl-viewer/](http://code.google.com/p/wsdl-viewer/)

通过ANT执行：<xslt basedir="doc" destdir="wsdl/html" extension=".html"  style="wsdl_viewer.xsl"> </xslt>


## 2. Xydra


[http://www.extreme.indiana.edu/xgws/xydra/](http://www.extreme.indiana.edu/xgws/xydra/)

Xydra is a library that uses servlet to provide XHTML based WSDL invoker.      Xydra servlet takes WSDL with XML Schema complex types as input, generates      XHTML form to allow user to fill content of input message, gathers submitted      input values and converts form name-value pairs into XML message that is      sent it to Web Service and then finally displays result message.
