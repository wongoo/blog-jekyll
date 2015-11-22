---
author: admin
comments: true
date: 2012-10-18 17:14:37+00:00
excerpt: WEB 应用 Session过期跳回登录页
layout: post
slug: session-timeout-and-return-to-login-page
title: Session过期跳回登录页
wordpress_id: 327
categories:
- Experience
tags:
- Javascript
- session-timeout
---

WEB应用一般在session过期的情况，将页面跳转到登录页面。但在过期的情况页面javascript仍然是可以执行的，并有可能弹出新窗口，此时新窗口会跳到登录页，原来那个页面还一直保留。
通过在登录页添加以下代码可以解决这个问题，主要的思路是：



1.如果当前页存在父窗口且父窗口未关闭的情况，父窗口跳到登录页，关闭当前窗口。
2.如果当前页不是顶层框架（frame），则顶层框架跳到登录页；




    
    if (window.opener && window.opener!=self && !window.opener.closed) {
    	window.opener.location.href = self.location.href;
    	window.close();
    }
    if (self != top)
    	top.location.href = self.location.href;

