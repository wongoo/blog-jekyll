---
author: admin
comments: true
date: 2012-10-19 08:28:36+00:00
excerpt: 解決WEB应用客户端缓存问题
layout: post
slug: web-client-cache-problem
title: 解決WEB应用客户端缓存问题
wordpress_id: 330
categories:
- Experience
tags:
- cache
---

WEB客户端缓存对某些内容不常更新的网站来说是个好事，但对实时性要求高的WEB应用则显得不友好了，这个时候就要去掉客户端缓存。

**1. 标记html Meta无需缓存：**

    
    
    
    

**2.标记Ajax 请求无需缓存：**
在XMLHttpRequest请求中加入以下头部：

    Pragma: no-cache
    Cache-Control: no-cache 

jQuey设置无需缓存：

    $.ajaxSetup({ cache: false });
    $.ajax({ cache: false });
    


**3. url添加动态日期参数“欺骗”browser认定为新连接而重新发送请求**
在ajax请求和location.href请求的的时候，url添加参数nowtime=new Date().getTime() .

**4. 更改browser缓存设定：**
参考：[http://www.wikihow.com/Clear-Your-Browser's-Cache](http://www.wikihow.com/Clear-Your-Browser's-Cache)
