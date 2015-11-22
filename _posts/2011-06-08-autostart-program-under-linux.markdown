---
author: admin
comments: true
date: 2011-06-08 04:02:10+00:00
excerpt: Autostart Program under Linux
layout: post
slug: autostart-program-under-linux
title: Autostart Program under Linux
wordpress_id: 116
categories:
- Experience
tags:
- autostart
- linux
---





* * *



/etc中所有的启动运行脚本简要说明:
**init inittab rc0 rc1 rc2 rc3 rc5 rc6 rcS init.d**
init 系统启动超级进程
inittab 进程启动配置文件
rc0 - rc6 各启动级别的启动脚本
rcS 单用户模式启动脚本
init.d 启动脚本存放目录

rc0.d: 停机模式，（千万不要把initdefault设置为0 ）
rc1.d: 单用户模式
rc2.d: 多用户模式，但是没有NFS支持。
rc3.d: 网络多用户文本模式，为完整的多用户模式，是标准的运行级。
rc4.d:一般不用，在一些特殊情况下可以用它来做一些事情。
rc5.d: 网络多用户图形模式，也就是X11，进到X Window系统了。
rc6.d:为重启，运行init 6机器就会重启。

**上面所有文件夹下，以S开头的表示要启动, 以K开头的表示不启动**



* * *



Autostart on Boot

ANSWER:
hello, I'm hoping (with your help ) I can get a program to run from boot time in debian. Basically I  want  the system to boot and launch straight into the program without  going  into x or asking for a passwords etc.  Can anyone advise how I do  this?  Thanks

REPLY:
This sounds like two distinct requests.** ****If you don't want to start X  when the system boots, go into your /etc/inittab and change the default  runlevel to 3**.
To start a program automatically,
**1. go into /etc/init.d and create a  script that starts the program,
**create a script like _/etc/init.d/Smyserver_：

    #!/bin/bash
    /opt/myserver/bin/start_


    And change run mode:
    _chmod 755 /etc/init.d/Smyserver_

**
2. then make a link in /etc/rc3.d that  points to that script. **

    ln -s /etc/init.d/__Smyserver Smyserver_

Follow the conventions used by other contents of  those folders, and check the man pages for specific guidance.



* * *



Autostart on X Server Start
Reference: [http://en.gentoo-wiki.com/wiki/HOWTO_Autostart_Programs](http://en.gentoo-wiki.com/wiki/HOWTO_Autostart_Programs)


