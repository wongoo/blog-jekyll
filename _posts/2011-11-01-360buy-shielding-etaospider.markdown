---
author: admin
comments: false
date: 2011-11-01 04:33:27+00:00
layout: post
slug: 360buy-shielding-etaospider
title: 360buy Shielding EtaoSpider
wordpress_id: 159
categories:
- Inspiration
tags:
- 360buy
- etao
- robot
- taobao
---

看到一段[新闻](http://tech.sina.com.cn/i/2011-10-25/11546226678.shtml)：

_新浪科技讯 10月25日上午消息，京东商城[(微博)](http://weibo.com/jingdong?zw=tech)与阿里巴巴[(微博)](http://weibo.com/aligroup?zw=tech)集团旗下比价购物搜索一淘网的暗战出现最新进展，京东商城已通过技术手段在[www.360buy.com/robots.txt](http://www.360buy.com/robots.txt)页面中加入“User-agent: EtaoSpider Disallow: /”代码，屏蔽来自一淘网的内容抓取。_

但是这样技术上360buy就真的能够阻止Etao抓取网站的信息吗？

[Robot Exclusion Protocol ](http://www.robotstxt.org/orig.html)Says：

_It is not enforced by anybody, and there no guarantee that all current and future robots will use it. Consider it a common facility the majority of robot authors offer the WWW community to protect WWW server against unwanted accesses by their robots._

也就是说 robots.txt协议并不是一种强制性协议，只是用来告知遵循此协议的Spider( or robot) 那些内容可以抓取，那些不要抓取。如果EtaoSpider不遵循此协议，或者它改了一下资料的User-agent，那么360buy的这个动作丝毫没 有意义了。

比如有些Bad Spider就不遵循此协议，它们会抓取所有网页，分析网页中mailto的内容，获取mail地址，然后向这些地址发送广告信息。

GoogleBot 是Google的搜索引擎机器人，它抓取的所有网页信息进行索引提供搜索服务，它遵循robots.txt协议，只抓取允许抓取的内容，这样也是为了提高 搜索质量应该要做的事情。而EtaoSpider不同，它抓取的内容是网站商品的价格，就像抓mailto一样，它抓取的内容是有针对性的，在设计上去遵 循robots.txt协议显得并没有必要。

所以Etao想抓取360buy的价格信息，一定是有方法的。但是360buy以此新闻通告它屏蔽了Etao，如果在Etao还能找出360buy的价格信息，Etao的社会公关方面就会显得难堪了。



附注，
1. 一些遵循此协议的robot：[ http://www.robotstxt.org/db.html](http://www.robotstxt.org/db.html)
2. Robots.txt 配置说明：[http://en.wikipedia.org/wiki/Robots.txt#Examples](http://en.wikipedia.org/wiki/Robots.txt#Examples)
3. Google的robots.txt设置：[http://www.google.com/robots.txt](http://www.google.com/robots.txt)
4. List of Bad Bots：[http://www.kloth.net/internet/badbots.php](http://www.kloth.net/internet/badbots.php)


