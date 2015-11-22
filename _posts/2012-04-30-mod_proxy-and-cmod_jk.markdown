---
author: admin
comments: true
date: 2012-04-30 05:39:02+00:00
excerpt: apache模块mod_proxy和mod_jk优劣势比较
layout: post
slug: mod_proxy-and-cmod_jk
title: mod_proxy和mod_jk比较
wordpress_id: 258
categories:
- Knowledge
tags:
- AJP
- apache
- balance
- mod_jk
- mod_proxy
---

#### Comparing mod_proxy and mod_jk


Authur：Mladen Turk
Published at 2007-7-16 3:22:57
See source link [here](https://community.jboss.org/people/mladen.turk/blog/2007/07/16/comparing-modproxy-and-modjk)


# **介绍**


Apache2.2 本身拥有如mod_proxy这样一系列优秀的模块，它们拥有一部分和mod_jk一样的功能（AJP Protocol），也能整合实现负载均衡。

AJP (Apache Jserv Protocol) 当前版本是1.3，是一个持久性的二进制协议。持久性指web server 和application server 直接的连接一旦建立，在系统生命周期内一直保持open的状态。

从客户端看，web server 和application server本质上是一个单一的系统。

既然系统中的某个节点可能因为各种原因死掉，同application server 一样，mod_proxy和mod_jk需要能够发现各种连接和通道的错误并作出反应。

mod_jk在过去几年就已经拥有了这样的技术优势，现在有各种技术实现连接错误侦察和修复。从现在来看，mod_jk在这方面比mod_proxy有更多的优势。


# 协议


mod_jk是基于AJP协议的，这个协议也通常被认为是二进制http协议。基于二进制协议，主要考虑有两点，首先，客户请求信息已经在web server被解码，就不用再把完整的请求数据发送给application server了；其次，在web server 和 application server 之间，请求和返回 header信息不再是string形式，而是两字节序列的原子形式，这样也就降低了网络带宽要求。

但是，AJP协议有一个主要的限制，就是packet的大小不能超过8K。最新的mod_jk和Tomcat使得这个限制可以加大到64K，但是还是有限制。mod_proxy也有限制，最大的packet大小是8K.如果遇到较大的客户端请求就会出问题了，特别像有些客户SSO模块存储了大量的session信息在cookies和header中。如果在要支持大型客户端请求，唯一的解决方案是使用AJP http协议。


# 加密以及SSL支持


AJP协议没有加密，不能够用在开放外部网络环境中。在这样的环境中，web server和application server 直接的的通信可能会被监听，需要使用一定的SSL隧道技术保障数据安全。另外一个选择是使用https协议结合mod_proxy.但是https会稍微复杂一点，需要在application server中添加filter处理客户端传递过来的证书。AJP协议则会自动处理，它将加密数据通过序列的方式再web server 和application
server直接传递。可以说，AJP协议的行为像是一个缓存SSL的加速器（the AJP protocol behaves like caching SSL accelerator）。

这样的性能更高效，因为数据只需进行一次解密。但是使用不同的网卡进行通信、建设防火墙和路由才是保障web server和application server之间通信安全最好的解决方案。

另外有些观点认为，将web server 和application server 放置在同一台物理机器上，他们直接通过内存进行通信，这样可以加强整个系统的安全性。


# 负载均衡


最新的mod_jk比mod_proxy_balancer有更多的优势特性。mod_jk额外拥有一些“商业特性”方法，可以根据实际application server 反应时间设置负载均衡。
mod_jk的“负责均衡维护”（load balancer maintenance）功能，能够有效的处理突发爆炸性的请求；当一个节点在维护的时候，能够减少降低此间的的请求。这种情况需要大量的application server和[session replication](http://tomcat.apache.org/tomcat-6.0-doc/cluster-howto.html)（会话复制）。
mod_jk的Domain Model Clustering功能，支持最新Jboss Cache的[buddy replication](https://community.jboss.org/wiki/JBossCacheBuddyReplicationDesign)。它通过将集群中的节点进行分组来降低session replication 数据传输，而且replication动作只需在集群中的（相邻的）节点之间进行。


# Apache httpd 各个版本


新版mod_proxy只支持Apache httpd 2.2和更高版本，也就是说，如果web server升级需要同时升级新版的mod_proxy.

Apache httpd可以结合worker-mpm使用。使用worker mpm的时候，mod_proxy和mod_jk都有设置web和application server之间连接池大小的选项。这个选项在部署的时候很有必要，可以使用Apache httpd 在application server之前传送一些其他内容，例如传送递静态内容等。在这种情况下，时间发送到application server的请求比实际上web server接收到的客户端请求要少很多，它允许worker mpm的链接池大小设置比MaxThreadsPerChild更小。

Windows和Netware版本的Apache httpd是完全线程的，所有他们的mpm和连接池大小可以设置处理更大的范围。


# mod_proxy vs. mod_jk


那么什么时候使用哪一个呢？这依赖于你的架构。如果你已经有了或者需要apache 2.2的功能，那么你可以再mod_proxy和mod_jk直接选择。mod_jk在apache2.2上允许得很好。关键看你需要什么样的功能：

mod_proxy



	
  * 优势:


	
    * 不需要编译和维护一个对立的模块。mod_proxy,mod_proxy_http,mod_proxy_ajp,mod_proxy_balancer已经是apache 2.2+的标准集成部分；

	
    * 可以使用http、https和AJP协议，即便是在同一个balancer中。


	
  * 劣势:


	
    * mod_proxy_ajp不支持大于8k的数据包；

	
    * 只有最基本的负载均衡器；

	
    * 不支持域模型集群（domain model clustering）



mod_jk

	
  * 优势:


	
    * 先进的负载均衡器；

	
    * 先进的节点失败侦察功能；

	
    * 支持大型AJP 数据包


	
  * 劣势:


	
    * 需要单独维护一个独立的模块；





# 总结


我个人建议是如果有能力维护mod_jk模块的二进制版本，尽量使用mod_jk。mod_proxy一直在更新但还缺少一些mod_jk的功能。但是，如果你需要https和一个简单的负载均衡就是用mod_proxy.
