---
author: admin
comments: true
date: 2011-07-01 07:45:45+00:00
layout: post
slug: icalendar_mass_code
title: iCalendar乱码问题
wordpress_id: 126
categories:
- Experience
tags:
- encode
- icalendar
- mail
- rfc5545
---

可能的问题：

1. 为iCalendar附件设置错误的编码，iCalendar的编码规定需设置为UTF-8或者US-ASCII编码[[http://tools.ietf.org/html/rfc5545#section-8.1](http://tools.ietf.org/html/rfc5545#section-8.1)]。

2. iCalendar的ContentType没有设置，或者设置错误，正确的contentType为“text/calendar";
错误的设置有：
text/icalendar
text/calendar;charset=UTF-8

3. 未声明LANGUAGE参数[[http://tools.ietf.org/html/rfc5545#section-3.2.10](http://tools.ietf.org/html/rfc5545#section-3.2.10)]，如：
LOCATION;LANGUAGE=zh-tw:七樓會議室
DESCRIPTION;LANGUAGE=zh-tw:會議描述
SUMMARY;LANGUAGE=zh-tw:我的會議






