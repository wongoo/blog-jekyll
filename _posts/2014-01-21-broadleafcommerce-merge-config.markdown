---
author: admin
comments: true
date: 2014-01-21 08:20:12+00:00
excerpt: BroadleafCommerce合并配置
layout: post
slug: broadleafcommerce-merge-config
title: BroadleafCommerce合并配置
wordpress_id: 558
categories:
- Knowledge
tags:
- broadleaf
- configuration
- ecommerce
- merge
---

Merge configuration是通过web.xml处理的，broadleaf的merge功能智能的合并一个或多个spring配置文件。合并的最终版本才被传递给spring处理。为了让broadleaf知道要merge那些文件，你必须在web.xml中声明。首先，我们提供一个patchConfigLocation的参数，表示你的spring context文件列表。你可以任意添加，它们将被合并到broadleaf配置中。

    
        patchConfigLocation
        
            /WEB-INF/applicationContext-custom.xml
            /WEB-INF/applicationContext-email.xml
            /WEB-INF/applicationContext-search.xml
            /WEB-INF/applicationContext-security.xml
        
    


其次，需声明org.broadleafcommerce.common.web.extensibility.MergeContextLoaderListener这个listener。它的职责是负责初始化broadleaf内部context信息和你声明的context文件直接的merge。

    
        org.broadleafcommerce.common.web.extensibility.MergeContextLoaderListener
    

当merge遇到有冲突的bean声明的时候，broadleaf知道如何处理合并或覆盖。
如，你声明了一个id为blOrderService的bean，broadleaf将使用你声明的bean。但是，某一些情况不会覆盖，而是会合并bean的内容。如以下blWebTemplateEngine声明：

    
        
            
                
                
            
        
     

如果你想添加你自己的dialect，你可以简单的在你的context文件中添加以下声明。

    
        
            
                
            
        
     

我们已经表明这个bean的属性为可merge的属性，而不是覆盖。所以最终，被spring处理的bean将是如下的样子：

    
        
            
                
                
                
            
        
     




### **参考**


1.[Broadleaf's Unique Application Context Merge Process Explained](http://www.broadleafcommerce.com/blog/broadleaf-spring-context-merge-process)
2.[Broadleaf Merge Process 2.0](http://www.broadleafcommerce.com/blog/broadleaf-spring-context-merge-2)
3.[Merge Configuration](http://docs.broadleafcommerce.org/core/current/broadleaf-concepts/key-aspects-and-configuration/merge-configuration)
