---
author: admin
comments: true
date: 2014-11-30 06:24:12+00:00
layout: post
slug: annotation-auth
title: 基于注解的权限验证方案
wordpress_id: 756
categories:
- Experience
tags:
- annotation
- architecture
- authentication
- authorization
- spring
---

基本实体设计：用户（user）、角色（Role）、功能（Function）。 一个用户可以有多个角色，一个角色可以有多个功能。

功能的可以通过url地址来进行定义，一个要访问的url定义一个功能（function）。但实际项目中一个页面功能可能对应多个url，比如ajax加载、验证、提交等功能的url，那么就需要配置多个Function。而一般情况要给角色分配的是一个页面所有的访问功能，而不是某一个url的访问功能。 故用统一的功能代码来表示页面权限更为恰当。只要将这个页面相关的url的权限都配置为这个功能代码即可。系统管理要做的就是维护角色和功能代码之间的关系。

另外，考虑到多个功能代码对应功能都涉及到对某一个url的访问，故一个url需要能够配置多个功能代码。

那么是否要在db配置url和功能代码的关系呢？在开发代码的时候，这层关系已经决定，配置也是定死的，配置在db中没有意义。直接在代码中写死，需要做的是定义好功能代码的格式和标准，或者在一开始项目架构的时候定义好，让攻城师遵守按照规定代码配置即可。而注解是一个绝好的配置方案。


* * *


**以下以Spring MVC为例进行说明，首先设计登陆验证：**
1. 定义@RequireLogin注解：

    @Target({ElementType.METHOD, ElementType.TYPE})
    @Retention(RetentionPolicy.RUNTIME)
    public @interface RequireLogin {
    }

2. 对需要登录访问的Controller类或者方法添加此注解；

    @Controller
    @RequireLogin
    @RequestMapping(value = "/role")
    public class RoleController {
    }

3. 增加Spring HandlerInterceptor拦截器，对要访问的Controller类或方法判断其有无添加@RequireLogin注解，如果有则进行登陆判断；

**设计权限验证：**
1. 定义@RequirePermission注解，注解包含一个功能代码数组变量，只要用户角色拥有其中任何一个功能代码权限即拥有权限访问；

    @Target({ElementType.METHOD})
    @Retention(RetentionPolicy.RUNTIME)
    public @interface RequirePermission {
    
        /**
         * Provide permission codes
         */
        String[] value();
    
    }

2. 对所有Controller中所有@RequestMapping方法添加此注解：

    @RequirePermission(value = "ROLE_MANAGMENT")
    @RequestMapping(value = "/viewRoleList", method = RequestMethod.GET)
    public String viewRoleList(){
    }

3. 增加Spring HandlerInterceptor拦截器，获得访问的Controller方法的@RequirePermission注解，获得所需权限功能代码列表，再检查当前登录用户角色对应的功能代码中是否包含其中一个要求的权限功能代码，如果包含则权限验证通过；


* * *


**注：**如果一个页面功能要访问的url都在同一个Controller中，则可考虑将@RequirePermission的定义范围扩展到类层，在Controller类层定义此注解，声明共通的功能代码，而方法层则声明共通之外特殊的功能代码，这样还可以减少配置。
