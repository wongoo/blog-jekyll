---
author: admin
comments: true
date: 2011-09-01 13:59:54+00:00
layout: post
slug: replace-char-with-ufffd-when-decode-failed
title: Replace special BIG5 character
wordpress_id: 148
categories:
- Experience
tags:
- Java JDK BIG5 decode
---

In Java JDK, the BIG5$Coder class can't decode the byte array [-7][-38] , the  bytes is of the character '恒' , then replace it with the char "\uFFFD".

Don't know why . Check the source code of BIG5$Coder when having free time .



Test code:


	byte[] bs = new byte[2];
	bs[0] = -7;
	bs[1] = -38;
	String str = new String(bs, "BIG5");
	System.out.println(str);

	bs = new byte[2];
	bs[0] = -92;
	bs[1] = -92;
	str = new String(bs, "BIG5");
	System.out.println(str);

	bs = new byte[2];
	bs[0] = -80;
	bs[1] = -22;
	str = new String(bs, "BIG5");
	System.out.println(str);

