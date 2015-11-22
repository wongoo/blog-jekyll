---
author: admin
comments: true
date: 2012-07-09 04:24:41+00:00
layout: post
slug: compile-err-of-apache-module
title: 编译apache module遇到的错误
wordpress_id: 295
categories:
- Experience
tags:
- apache
- C
- GCC
- linux
---

编译module命令：gcc  -c -o mod.o mod_example_config.c -I/usr/apache/include -I/usr/local/apr-util/include/apr-1 -I/usr/local/apr/include/apr-1

遇到以下错误：
/usr/local/apr/include/apr-1/apr.h:358: 错误：expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘apr_off_t’

Google了一下，再加一个编译参数-D_LARGEFILE64_SOURCE 就可以编译成功了：
gcc -D_LARGEFILE64_SOURCE -c -o mod.o mod_example_config.c -I/usr/apache/include -I/usr/local/apr-util/include/apr-1 -I/usr/local/apr/include/apr-1

查看了一下预编译代码大小，才290K，好像也不是很大，求解啊？：
cpp mod_example_config.c >mod.i -I/usr/apache/include -I/usr/local/apr-util/include/apr-1 -I/usr/local/apr/include/apr-1
