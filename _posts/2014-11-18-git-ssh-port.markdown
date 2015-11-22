---
author: admin
comments: true
date: 2014-11-18 05:56:27+00:00
excerpt: Git SSH连接端口
layout: post
slug: git-ssh-port
title: Git SSH连接端口
wordpress_id: 744
categories:
- Knowledge
tags:
- git
- port
- ssh
---

如果git SSH的默认端口是22，那么可以使用以下方式clone 一个git项目：

    git clone git@mydomain.com:gitolite-admin


如果git SSH的默认端口不是是22，比如10022，有两种连接方式：
第一种，按照如下方式修改链接地址：

    git clone ssh://git@mydomain.com:[port]/gitolite-admin

注意：不能缺少协议“ssh://”声明，否则不能连接。

第二种，修改 ~/.ssh/config配置文件，增加domain映射，用映射的别名进行替换：

    # 映射一个别名
    host mydomain_alias
    hostname mydomain.com
    port 11022

连接的时候使用别名连接：

    git clone git@mydomain_alias:gitolite-admin


**参考：**
1. Git On Custom SSH Port, [http://stackoverflow.com/questions/5767850/git-on-custom-ssh-port](http://stackoverflow.com/questions/5767850/git-on-custom-ssh-port)



