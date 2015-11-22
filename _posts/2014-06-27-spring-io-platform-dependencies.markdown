---
author: admin
comments: true
date: 2014-06-27 02:22:59+00:00
excerpt: spring io platform 依赖版本管理
layout: post
slug: spring-io-platform-dependencies
title: spring io platform 依赖版本管理
wordpress_id: 618
categories:
- Experience
tags:
- dependency
- spring
- spring-io
---

今天收到spring推出spring io platform的邮件，它是一个基于maven（或Gradle）的依赖版本管理的定义，它定义了常用的企业开发所需的java库的依赖版本，使得开发者不再花心思研究各个java库相互依赖的版本，只需引用spring io platform的定义。特别是引用的java库要升级的时候，还需要所有相关的java库都要检查一下，往往需要漫长的验证过程才能最终确定一个可用的版本。 如果使用spring io platform，依赖的版本关系已经被验证测试过了，所以可以放心使用，升级也是简单升级spring io platform即可，它会升级所有相关依赖的java库。

公司中我也创建了一个类似spring io platform的版本依赖定义的项目，其他多个开发项目都在用。后续将考虑修改此依赖定义项目，先引入spring io platform，如果有特别需要的java库，再另外添加！ 以后java库升级就不那么麻烦了！


    
    
    
            
                
                    io.spring.platform
                    platform-bom
                    1.0.0.RELEASE
                    pom
                    import
                
            
        
    


参考：[Spring IO Platform Reference Guide](http://docs.spring.io/platform/docs/1.0.0.RELEASE/reference/htmlsingle/)
