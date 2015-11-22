---
author: admin
comments: true
date: 2014-12-05 07:08:50+00:00
excerpt: Web应用异常日志记录优化方案
layout: post
slug: web-error-log-method
title: Web应用异常日志记录优化方案
wordpress_id: 784
categories:
- Experience
tags:
- java
- log
- web
---

Java Web应用常用的日志管理框架如log4j,logback等,一般生产环境不会设置为DEBUG模式，这样会导致日志量大增，影响系统性能，也影响正常日志的分析。正常设置为INFO级别，警告(WARN)或异常(ERROR)的日志也会输出，发生异常时，需要查看发生异常前后上下文的相关日志进行分析，比如分析一个HTTP请求异常，需要分析请求参数、发生异常前后的逻辑处理日志，但日志中记录了所有请求的日志信息，如果同一时间有多个请求，只能根据请求线程ID在日志文件中上下查找，很是麻烦！

另外，有时仅仅看INFO、WARN、ERROR的日志还不能分析问题，还需要一些DEBUG级别的日志。此时只能停止应用，修改LOG日志级别为DEBUG，再启动应用，重新模拟异常发生的情况。但这可能会遇到以下几个问题：





  1. 模拟发生异常的状况非常麻烦，费时费力!


  2. 异常发生时的状况无法知晓，难以模拟！


  3. 测试环境不会发生，但生产环境有不能模拟测试!



**为了解决以上问题，需要一个日志记录方案满足这样的功能：发生异常时将同一个线程内包括DEBUG级别的日志全部输出到单个请求异常日志文件中。**

**设计方案描述：**
1. 定义一个TraceLoggingEventProcessor类，此类中接收ILoggingEvent实例，但并不立即写到日志文件中，而是将event记录到一个list变量中；当遇到ERROR层级的日志时，才将list中的event都写到一个单独的日志文件中，而后续的日志也直接写到文件； 此线程从头到尾没有ERROR层级日志，则将event list清空即可。
2. 新增日志Appender类TraceAppender，在Root logger设置为DEBUG级别并添加TraceAppender。Root logger中同时包含正常日志Appender，但在正常日志Appender的配置中将其设置为INFO级别。
3. 覆盖TraceAppender的append方法，其从一个范型类型为TraceLoggingEventProcessor的ThreadLocal类变量中取出实例，将日志event交给此TraceLoggingEventProcessor进行记录。


