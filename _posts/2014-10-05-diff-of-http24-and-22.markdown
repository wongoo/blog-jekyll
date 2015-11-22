---
author: admin
comments: true
date: 2014-10-05 14:37:32+00:00
excerpt: httpd2.4和2.2在访问控制配置的差异
layout: post
slug: diff-of-http24-and-22
title: httpd2.4和2.2在访问控制配置的差异
wordpress_id: 728
categories:
- Experience
tags:
- access-control
- apache
- httpd
---

2.2版本访问控制是通过Order, Allow, Deny, 和Satisfy这几个指令，对客户端访问hostname、IP以及其他requests信息进行限制.
2.4版本使用mod_authz_host模块以类似权限检查的方式进行访问控制。旧的访问控制方式应该改用新的机制。但2.4版本也提供一个新的mod_access_compat模块来适应旧的配置方式。

  

以下列出几个2.2和2.4配置差异例子：

**禁止所有请求：**
2.2 configuration:

    Order deny,allow
    Deny from all

2.4 configuration:

    Require all denied

  


**允许所有请求：**
2.2 configuration:

    Order allow,deny
    Allow from all

2.4 configuration:

    Require all granted

  


**仅允许example.org域可访问:**
2.2 configuration:

    Order Deny,Allow
    Deny from all
    Allow from example.org

2.4 configuration:

    Require host example.org

  


参考：[https://httpd.apache.org/docs/2.4/upgrading.html](https://httpd.apache.org/docs/2.4/upgrading.html)
