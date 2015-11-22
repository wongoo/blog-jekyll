---
author: admin
comments: true
date: 2015-03-12 15:20:44+00:00
layout: post
slug: windows-config-for-pressure
title: 用Windows作为压力测试客户端需注意的环境设定
wordpress_id: 830
categories:
- Experience
tags:
- jmeter
- performance-test
---

使用jmeter进行压力测试，定义100个thread，每个thread会发起三个http请求，总共就是30000个请求。因client请求server时会随机创建port和server建立连接，但如果client允许创建的port不够，则会报错。Windows默认允许创建的port最大为5000，故30000个client请求则可能会发生端口不足报错的情况。可通过修改windows注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters目录下的名称为MaxUserPort的REG_DWORD值来调整允许创建的最大端口，建议将其设置为60000（十进制），这样client允许创建的port数就达到将近60000个。

大量压测会使client产生TCP/IP连接端口，关闭连接后，这些端口并不会立刻释放，而是出于TIME_WAIT状态，默认情况需要240秒后才会释放。
可以修改windows注册表HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/ Services/TCPIP/Parameters目录下的名称为TcpTimedWaitDelay的REG_DWORD值，将其改为最小值30，以加快端口的释放速度。其取之范围为0-300，默认240（4分钟）。

