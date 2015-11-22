---
author: admin
comments: true
date: 2012-10-20 05:22:31+00:00
excerpt: 厦门国际马拉松网上报名遇到的页面问题
layout: post
slug: marathon-regist-probleam
title: 厦门国际马拉松网上报名遇到的问题
wordpress_id: 337
categories:
- Experience
tags:
- Javascript
- marathon
---

准备网上报名参加2013年厦门国际马拉松，但是填完表格 http://www.xmim.org/cn/register_form.asp ，准备提交的时候，却出现错误“无返回数据”。
直觉是服务器出问题了，难道是访问人太多了？ 在回头访问其他页面，可以正常访问。那是报名系统出了问题，我想应该有很多人有遇到同样的问题吧，但网上搜索了一下，却几乎没有这样的消息。
难得跟我报名填写的消息相关？我的直觉是对的，我在填写“家庭住址”的内容中有填写了一个“#”号，换掉这个“#”就正常了。
唉，系统太烂，太不友好了！

这让我想看看其页面源代码是怎么写的，发现html太乱，没有使用css，javascript有使用jquery但只是为了使用其事件处理，页面中还有一些质量低劣javascript原生代码，想必是没有经验的web工程师开发的结果。
