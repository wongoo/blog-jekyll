---
author: admin
comments: true
date: 2013-07-16 14:23:26+00:00
excerpt: 如何解决Windows Server 2008 服务启动不能调用Office DCOM转Word的问题
layout: post
slug: windows2008-call-dcom-problem
title: 如何解决Windows Server 2008 服务启动不能调用Office DCOM转Word的问题
wordpress_id: 456
categories:
- Experience
---

项目中产生word的方案是调用Microsoft DCOM API动态产生，在Windows Server 2008下测试，用Jboss standalone console模式启动会成功产生Word，但是将jboss注册为service启动则会失败，出现接口没方法或者卡死（cpu 100%）的错误。

经google之后了解到是因为Windows Vista/2008改变了COM对象默认的交互方式为“非交互”型的。Console启动本身支持应用交互，但service模式下就不行了。所以需要修改word DCOM默认的标识，改为“交互式用户”模式，即可正常调用了。

**按照以下步骤修改后再测service模式下试转Word即可成功：**
1） 运行命令： mmc comexp.msc -32
2） 找到：组建服务>计算器>我的计算机>DCOM组件>Microsoft Word 97-2003 文檔；
3） 右键点击，选择属性，修改标识为“交互式用户”，点击“确定”；

**参考文档：**
1）[Setting the Identity for an Application](http://msdn.microsoft.com/en-us/library/ms678426%5C#setting_the_identity_for_an_application)
2）[Find Microsoft Word Application in DCOM](http://forums.asp.net/t/1110648.aspx/1)
3) [Jacob is not working when running as service on Windows Vista, Windows 7 and Windows 2008 server](http://sourceforge.net/p/jacob-project/bugs/86/)
