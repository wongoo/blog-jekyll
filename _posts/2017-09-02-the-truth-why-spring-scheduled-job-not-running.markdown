---
layout: post
author: gelnyang
comments: true
date: 2017-09-02
title: 程序会做正确的事——spring scheduled job未执行的事实
categories:
- experience
tags:
- spring
- scheduled
- analysis
---
---

昨天同事问了我一个问题，线上一个微服务有一个spring scheduled job一段时间都没有运行了，
这段时间都没有打日志，应该是排程服务死掉了。

直觉告诉我事情绝非表象这样，程序会做正确的事！

看了一下这个排程的定义是每隔10分钟执行一次，整个spring服务的其他接口都有正常工作，系统也没有记录schedule相关的错误日志。
这问题其他人应该也遇到过，google了一下发现有人也有这样的现象，原因是scheduled job中的http请求没有设置timeout，
而http请求阻塞了，导致整个scheduled job一直处于阻塞状态，job没有停止，也不会触发下一次的job启动，也就不会有新的日志产生。

建议的做法是增加http请求timeout设定。

分析一下我们自己的scheduled job处理的业务，其中有一个请求aliyun sts服务，服务的协议是http，查看了一下调用sts服务的代码，没有timeout相关设置。
大胆的猜想我们遇到的情况和网上其他人遇到的情况是一样的，那么现在服务的scheduled job应该是被挂起了。

使用kill -3 <PID> 产生线程信息，看了一下scheduled job果然是在请求aliyun服务的时候挂起了，一直在读socket `java.net.SocketInputStream.socketRead0(Native Method)`。

快速临时的解决办法是重启线上服务，最终解决办法是请求aliyun服务的时候都增加timeout设置。

另外查看了一下aliyun服务sdk，发现没有默认timeout设置。

原来我们初始oss client：

```
ossClient = new OSSClient(endpoint, accessKeyId, accessKeySecret);
```

没有传递config参数，就会默认new一个ClientConfiguration，但这个config的RequestTimeoutEnabled默认值是false:

```
public OSSClient(String endpoint, CredentialsProvider credsProvider, ClientConfiguration config) {
    this.credsProvider = credsProvider;
    config = config == null ? new ClientConfiguration() : config;
    if (config.isRequestTimeoutEnabled()) {
        this.serviceClient = new TimeoutServiceClient(config);
    } else {
        this.serviceClient = new DefaultServiceClient(config);
    }

    this.initOperations();
    this.setEndpoint(endpoint);
}
```

更改初始化方式如下:

```
ClientConfiguration config = new ClientConfiguration();
config.setRequestTimeoutEnabled(true);
ossClient = new OSSClient(endpoint, accessKeyId, accessKeySecret, config);
```

另外aliyun其他服务的sdk都没有默认timeout的设定，所以调用的时候要注意了！

{{ page.date | date_to_string }},{{ page.author }}

