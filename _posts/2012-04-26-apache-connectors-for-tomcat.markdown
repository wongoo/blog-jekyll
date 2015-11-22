---
author: admin
comments: true
date: 2012-04-26 02:33:03+00:00
excerpt: 什么是JK(或者AJP)? 为什么需要（或不需要）Apache和tomcat整合？Connector比较：mod_jserv,JK,JK2,mod_webapp,
  mod_proxy?
layout: post
slug: apache-connectors-for-tomcat
title: Apache Connectors for Tomcat FAQ
wordpress_id: 253
categories:
- Experience
tags:
- AJP
- apache
- mod_jk
- mod_proxy
- Tomcat
---

## 概览


请查看[其他资源连接](http://wiki.apache.org/tomcat/UsefulLinks)页面，了解他们如何使用connector连接tomcat。幸运的话，文档描述的环境和你的一样。
访问[JK Connectors](http://tomcat.apache.org/connectors-doc/)，了解更多配置和安装的信息。
注意：jk2已不再支持，请使用mod_jk。****
****


## 问题





	
  1. 什么是JK(或者AJP)?

	
  2. Connector比较：mod_jserv,JK,JK2,mod_webapp, mod_proxy

	
  3. 为什么需要（或不需要）Apache和tomcat整合？

	
  4. Apache和Tomcat整合，两者启动有顺序吗？

	
  5. 有什么方法控制自动产生的mod_jk.conf-auto文件的内容？

	
  6. 如何绑定一个特殊的ip地址？

	
  7. 哪里可以下载二进制版本的connector？

	
  8. 遇到一个奇怪关于请求参数的UFT-8编码问题

	
  9. 一个访问量很大的网站要如何配置tomcat connecotr？


****
****


## 回答


****
什么是JK(或者AJP)?
****AJP是一个[wire protocol](http://en.wikipedia.org/wiki/Wire_protocol). 它是HTTP协议的优化版本，支持单一web server，如apache整合tomcat。由于历史原因，apache在处理静态内容方面比tomcat要快很多。AJP的主要观点是让apache处理大部分静态内容请求，把其他请求转发给tomcat处理；********

Connector比较：mod_jserv,JK,JK2,mod_webapp, mod_proxy



	
  * 远离mod_webapp(及warp)。因缺少开发者支持，已被弃用。另外的原因是有了更好的选择如JK、mod_proxy. mode_webapp将不能再windows上运行。不要使用mod_webapp或者warp就是了。

	
  * mod_jserv将不再提供支持，tomcat5中也将弃用。mod_jserv是最原始支持AJP协议的connector。也不要使用mod_jserv。

	
  * jk2是mod_jk的一个变种，使用Apache Portable Runtime (apr)，但是并没有赢得开发者，不再提供支持。可以选择mod_jk或mod_proxy_ajp。也不用使用jk2.

	
  * 相比较而言，mod_jk成熟、稳定、非常灵活。有tomcat社区的成员在开发维护。

	
  * mod_proxy是一个轻量的代理方式，没有JK那么麻烦的配置。如何你不需要JK的一些特性，mod_proxy是一个简单的选择。

	
  * mod_proxy_ajp. Apache2.2开始，mod_proxy被重写，新增一个模块mod_proxy_ajp以支持负载均衡，它包含在apache http server发行包中。


****
为什么需要（或不需要）Apache和tomcat整合？****

有很多原因，但可能很多人并不同意。tomcat5和tomcat6在性能上的改善，使得出于提高性能的考虑而去整合apache和tomcat变得不让人信服了。那么可以讨论下面这些情况以决定是否需要整合apache和tomcat。



	
  * 集群。可将Apache作为一个前端入口，转发请求到多个tomcat实例。如果其中一个tomcat失败了，apache可以忽略它，系统管理员就可以不用半夜起床去维护了。但是如果你使用硬件负载均衡和Tomcat自身的集群功能，则可以不需要apache了。

	
  * 集群/安全：可以将apache作为一个前端入口，转发不同命名空间（如/app1/, /app2/, /app3/, 或者虚拟主机）到不同的tomcat server。每一个tomcat server都在一个安全区域，请求来源也都是安全的（From apache），只需要考虑apache的安全就可以了。这种情况，apache本质上变成了一个可靠的proxy server。

	
  * 安全：这一点让人为难。java本身有安全管理器，apache在安全上有更多的考虑（不过设定较多，这儿不展开，自行google）。需要看你具体的环境以判断对整合是否会更好。但要记住，如果整合apache和tomcat，则你需要维护两个系统。

	
  * 插件：apache可以轻松添加CGI,perl，PHP等插件，但tomcat要添加插件就比较麻烦了，而且速度也比apache慢。Apache有无数插件可以添加，tomcat虽然也可以，但是大多数都还未实现（应该也没人去做这个事情）。

	
  * 装饰器！将apache作为tomcat的前端入口，可以添加多个tomcat不支持或未实现的装饰器。例如mod_headers,mod_rewrite,mod_alias, 虽然这些功能可以为tomcat重写，但是apache已经做得很好了，又何必重新造轮子呢？

	
  * 速度：处理静态内容，apache比tomcat要快。但除非是访问量很大的网站，这一点也没多大用处。在某些环境下，tomcat可能比apache更快。所以还得根据你自己的网站情况来做决定。当Tomcat使用适当的connector（如开启sendFile的APR）也能够达到和httpd一样的速度。选择apache httpd还是tomcat，速度不应该作为一个关键因素。

	
  * 套接字处理/系统稳定性：apache套接字处理比tomcat更好，能够处理各种错误情况。主要原因是tomcat为了跨平台的需求都是用JVM来处理套接字。但套接字优化是有平台特殊性的。基于JVM的java程序大多时候都能很好的处理各种请求，但下面这些情况，apache能够处理得更好，如遇到中断的链接、非法的packets、来自非法的IP的非法请求，apache能够过滤丢弃这些错误。（YMMV）




Apache和Tomcat整合，两者启动有顺序吗？
没有。在任何时候都可以单独重启apache或tomcat。****

有什么方法控制自动产生的mod_jk.conf-auto文件的内容？我需要自己特殊的命令去添加。
并不需要这样。直接复制mod_jk.conf-auto 的内容，然后按照你的情况进行修改。没有一个产品环境在用mod_jk.conf-auto 。****

如何绑定一个特殊的ip地址？
每一个connector有一个地址属性配置。查看[HTTP Connector docs](http://tomcat.apache.org/tomcat-6.0-doc/config/http.html)和[AJP Connector docs](http://tomcat.apache.org/tomcat-6.0-doc/config/ajp.html).
**
哪里可以下载二进制版本的connector？
**没有（windows平台有）。你需要下源码在你的平台上编译。源码可以从[官方页面](http://tomcat.apache.org/download-connectors.cgi)获取。注意，[JPackage.org](http://www.jpackage.org/) 也有提供各种connector的RPM包。****

遇到一个奇怪关于请求参数的UFT-8编码问题
查看ee [Character Encoding](http://wiki.apache.org/tomcat/FAQ/CharacterEncoding)****

一个访问量很大的网站要如何配置tomcat connecotr？
查看ee [Performance and Monitoring](http://wiki.apache.org/tomcat/FAQ/Performance_and_Monitoring)****

原文链接：[http://wiki.apache.org/tomcat/FAQ/Connectors](http://wiki.apache.org/tomcat/FAQ/Connectors)
