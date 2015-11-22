---
author: admin
comments: true
date: 2010-12-15 03:57:27+00:00
excerpt: Could not generate WSDL! There is no SOAP service at this location
layout: post
slug: axis-error-could-not-generate-wsdl-there-is-no-soap-service-at-this-location
title: Axis error - Could not generate WSDL! There is no SOAP service at this location
wordpress_id: 38
categories:
- Experience
---

AXIS error

Could not generate WSDL! There is no SOAP service at this location

请求的service name出错，或者Axis的wsdd文件没有找到或发布出错

可能问题是service-config.wsdd配置不正确，注意以下红色字体的配置：

<?xml version="1.0" encoding="UTF-8"?>
<deployment xmlns="http://xml.apache.org/axis/wsdd/" xmlns:java="http://xml.apache.org/axis/wsdd/providers/java">
**<handler type="java:org.apache.axis.handlers.http.URLMapper" name="URLMapper" />**
<service name="hello" provider="java:RPC">
<parameter name="className" value="c4j.haa.ws.HelloService" />
<parameter name="allowedMethods" value="getHello" />
<!-- Application,Session,Scope -->
<parameter name="scope" value="Application" />
</service>

**<transport name="http">
<requestFlow>
<handler type="URLMapper" />
</requestFlow>
</transport>**
</deployment>
