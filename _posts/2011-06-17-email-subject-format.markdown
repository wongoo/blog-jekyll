---
author: admin
comments: true
date: 2011-06-17 06:54:16+00:00
layout: post
slug: email-subject-format
title: Email subject format
wordpress_id: 120
categories:
- Experience
tags:
- encode
- format
- mail
---

Email的标题源码格式是：

**"=?" + ENCODE + "?" + SUBJECT_DATA_TYPE+ "?" + ENCODED_SUBJECT_DATA + "?="**



	
  1. 以“=?”开头；

	
  2. 以 "?="结束；

	
  3. ENCODE:编码；

	
  4. SUBJECT_DATA_TYPE:指邮件标题数据类型，可选值有B或者Q，B指Base64，Q指[Quoted-printable](http://en.wikipedia.org/wiki/Quoted-printable)编码

	
  5. ENCODED_SUBJECT_DATA：编码过后的邮件标题内容;


reference：[http://www.realsoftware.com/listarchives/realbasic-nug/2003-04/msg02405.html](http://www.realsoftware.com/listarchives/realbasic-nug/2003-04/msg02405.html)


