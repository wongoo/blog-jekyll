---
author: admin
comments: true
date: 2014-09-24 15:59:28+00:00
excerpt: Shell用管道符组建功能强大的指令，批量kill进程，批量打包等
layout: post
slug: using-pipe-operator-to-impl-powful-commands
title: 使用管道符组建功能强大的指令
wordpress_id: 715
categories:
- Knowledge
- Experience
post_format:
- 日志
tags:
- linux
- shell
- 管道符
---


Shell语法中的管道符命令“|”,用于将前一个命令的输出作为紧接其后命令的输入。如果将其用好了，一行指令可以实现一些很强大的功能。

**1. 查找指定内容：**
   于管道符命令“|”后接grep命令，在前一个命令的输出中查找指定内容。

在所有进程中查找是否存在httpd进程：

    > ps -ef | grep myapp
    65534     4587  4586  1 22:41 pts/2    00:00:00 /opt/myapp/bin/myapp -config /opt/myapp/conf/myapp.conf
    root      4612  4470  0 22:41 pts/2    00:00:00 grep myapp


在log日志中查找是否有ERROR信息(-n参数添加行号，这样可明确知道那一行有错误）

    > cat -n myapp.log | grep ERROR


在log日志最开始10行内容查找是否有ERROR信息

    > head -n 10 myapp.log | grep ERROR


在log日志最后10行内容查找是否有ERROR信息

    > tail -n 10 myapp.log | grep ERROR


在log日志第10行之后的内容中查找是否有ERROR信息

    > tail -n +10 myapp.log | grep ERROR


获得log中ERROR、WARN类型（除去INFO类型）的日志：

    > cat myapp.log | grep -v ' INFO ' #INFO标记前后有空格，此命令也可以直接使用grep命令：grep -v ' INFO ' myapp.log


有时候一个目录下的文件太多，只想查看今天有更新异动的文件：

    > ls -l |grep ' '$(date +%m-%d)
    -rw-r--r-- 1 root root       5 09-24 22:59 myapp_out.log
    -rw------- 1 root root    2211 09-24 22:41 myapp_err.log
    


**2. 对内容进行操作：**
获得所有进程的进程编号：

    > ps aux | cut -c 9-15


获得当前目录文件的文件名列表：

    > ls -l |cut -c 42-255


**3. xarg命令**
   该命令功能有点类似管道符命令，它将多行内容转为一行内容，行与行之间用空格间隔，并此行内容作为参数传递给其他命令。

获得所有进程的进程编号并一行显示：

    > ps aux | cut -c 9-15 | xargs echo


**4. 经典应用案例：**
将/opt/myapp目录下的程序的进程kill掉：

    > ps aux | grep '/opt/myapp' | cut -c 9-15 | xargs kill -9


将今日有异动的log日志打包为一个zip档：

    > ls -l |grep ' '$(date +%m-%d) | cut -c 43-255 | xargs zip log_$(date +%Y%m%d).zip


