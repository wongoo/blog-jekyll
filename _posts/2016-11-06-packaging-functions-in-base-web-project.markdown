---
layout: post
author: gelnyang
comments: true
date: 2016-11-06
title: 基础web项目封装的功能
categories:
- 经验
tags:
- 开发 经验
---
---

整理一下web基础项目目前封装的主要功能：


## 1. JPA访问
增对JPA访问封装了list，count方法，调用者只需传递sql语法及参数列表。

## 2. 分页对象
基于spring MVC分页对象进行了修改，增加了特定的初始化方法、添加查询结果内容方法，增加了分页列表加载方法。
 
## 3. LOG日志控制相关
包括日志全局变量初始化、日志格式化定义、日志追踪过滤器等

## 4. 访问权限认证
基于spring security框架，封装了基于LDAP和DB的相关认证功能。

## 5. MVC相关配置
基于spring MVC的配置，包括 formater,validator,messageSource,localResolver,viewResolver,Environment等。

## 6. 监控相关
提供系统状态、内存、线程、url映射信息的监控


{{ page.date | date_to_string }}
