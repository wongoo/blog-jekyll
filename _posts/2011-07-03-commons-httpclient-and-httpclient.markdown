---
author: admin
comments: true
date: 2011-07-03 02:51:06+00:00
layout: post
slug: commons-httpclient-and-httpclient
title: commons-httpclient 和 httpclient的不同之处
wordpress_id: 133
categories:
- Experience
tags:
- apache
- commons
- http
- httpclient
- java
---

commons-httpclient是apache的老项目，最后版本是3.1，不再建议使用。

最新的httpclient是属于[Apache HttpComponents](http://hc.apache.org/) 下面的一个子项目，分为[HttpClient](http://hc.apache.org/httpcomponents-client-ga) 和[HttpCore](http://hc.apache.org/httpcomponents-core-ga/)两个部分，API库更丰富，同时包含server和client端的API， 其版本号从4.0开始。新项目应该使用该版本。

参考：
[http://stackoverflow.com/questions/5153434/what-the-relationship-between-commons-httpclient-and-httpclient-both-from-apache](http://stackoverflow.com/questions/5153434/what-the-relationship-between-commons-httpclient-and-httpclient-both-from-apache) , about the question
[http://hc.apache.org/](http://hc.apache.org/), Apache HttpComponents
[http://hc.apache.org/httpclient-legacy/index.html ](http://hc.apache.org/httpclient-legacy/index.html )， httpclient history
[http://hc.apache.org/httpcomponents-client-ga/index.html ](http://hc.apache.org/httpcomponents-client-ga/index.html )， new httpclient features
