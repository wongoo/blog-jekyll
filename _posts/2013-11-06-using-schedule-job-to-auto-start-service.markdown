---
author: admin
comments: true
date: 2013-11-06 06:41:37+00:00
excerpt: 利用Windows任务计划和shell脚本定时重启服务
layout: post
slug: using-schedule-job-to-auto-start-service
title: 利用Windows任务计划和shell脚本定时重启服务
wordpress_id: 535
categories:
- Experience
tags:
- schedule
- shell
- windows
---

客户传真接收程式莫名原因会卡住，需重启才会好。传真接收程式很旧，也暂无时间查原因，为快速解决，使用Windows任务计划和shell脚本，每隔一个小时重启一下传真接收程式。

**1. 准备shell脚本（以下以WORD进程为例）：**

    
    @echo off
    rem --------------------------------------
    rem author:geln_yang 
    rem date:2013-11-06
    rem --------------------------------------
    
    rem --------------------------------------
    rem configuration variables
    rem --------------------------------------
    set log_file_path=C:\schedule_command\restart.log
    set command_name=WINWORD.EXE
    set command_path="D:\App\Microsoft Office\Office15\WINWORD.EXE"
    set kill_start_interval=10
    
    rem --------------------------------------
    rem start to kill process
    rem --------------------------------------
    echo %date% %time%[INFO] start to kill process:%command_name% >> %log_file_path%
    taskkill /IM %command_name% /f >> %log_file_path%  2>&1
    
    rem --------------------------------------
    rem sleep for a while 
    rem --------------------------------------
    echo %date% %time%[INFO] sleep %kill_start_interval% seconds >> %log_file_path%
    timeout %kill_start_interval% >> %log_file_path%
    
    rem --------------------------------------
    rem try to start process
    rem --------------------------------------
    echo %date% %time%[INFO] try to start process:%command_path% >> %log_file_path%
    start "%command_name%" %command_path%
    

说明：
  1）此脚本需配置日志文件路径、服务进程名称、启动服务命令路径（注意命令路径有空格时需加引号）；
  2）启动服务进程时，通过start命令执行，避免cmd指令等待；
  3）输出命令结果到日志的时候，添加2>&1为将STDERR的输入也写到日志中；

**2.配置系统任务计划：**
  1）创建任务计划，使用管理员帐号执行，勾选“不管用户是否登录都要运行”，勾选“使用最高权限运行”；
  2）触发器,设置每天运行一次，开始时间为00:00，设置重复任务间隔时间为一小时，持续时间为“无限期”；
  3）操作，添加运行脚本路径，"起始于"设置为脚本所在目录；
  4）设置，勾选“如果过了计划开始时间，立即启动任务”；


**3.参考：**
1.Microsoft DOS start command, [http://www.computerhope.com/starthlp.htm](http://www.computerhope.com/starthlp.htm)
2.Using command redirection operators,[http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true)

