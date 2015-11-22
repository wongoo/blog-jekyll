---
author: admin
comments: true
date: 2014-12-04 02:21:56+00:00
excerpt: Jboss应用管理命令，部署/卸载/启动/停止
layout: post
slug: jboss-commands
title: Jboss应用管理命令
wordpress_id: 769
categories:
- Experience
tags:
- cli
- deploy
- jboss
---

**1. 获得命令帮助**
_jboss-cli.bat help
jboss-cli.bat --command="deploy --help"
jboss-cli.bat --command="undeploy --help"_

**2. 完整命令格式**
_jboss-cli.bat -c --controller=[JBOSS_SERVER]:[SERVER_PORT] --user=[DEPLOY_USER] --password=[DEPLOY_PASSWORD] --command="[COMMAND]"_

**说明：**



	
  * [JBOSS_SERVER]：jboss server的ip或机器名

	
  * [SERVER_PORT]：Jboss native管理端口,默认9999

	
  * [DEPLOY_USER]：管理帐号

	
  * [DEPLOY_PASSWORD]：管理帐号密码

	
  * [COMMAND]：具体执行指令（用“指令”表述来区别于“命令”）


**3. 部署war并制定context-path名称的指令:**
_deploy f:\maven_repository\com\example\myapp\1.0.0\myapp-1.0.0.war --name=myapp.war_

如果只是部署但不启动，可以加'--disabled'参数指定disabled状态：
_deploy f:\maven_repository\com\example\myapp\1.0.0\myapp-1.0.0.war --name=myapp.war --disabled_
	
注意：



	
  * 如果没有提供'--name'参数声明部署应用名称（默认也会被用于context path）, 则使用war文件的名称.

	
  * 部署应用名称'--name'参数需以".war"结尾


	
**4. 卸载应用的指令:**
_undeploy myapp.war_

**5. 停止(Stop/Disable)应用的指令:**
_undeploy myapp.war --keep-content_
	
**6. 启动(Start/Enable)应用的指令:**
_deploy --name=myapp.war_
	
