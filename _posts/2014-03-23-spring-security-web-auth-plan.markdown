---
author: admin
comments: true
date: 2014-03-23 06:21:00+00:00
excerpt: 一种Spring Security Web认证实现方案及其核心组件及认证过程说明
layout: post
slug: spring-security-web-auth-plan
title: 一种Spring Security Web认证实现方案
wordpress_id: 595
categories:
- Experience
tags:
- architecture
- authentication
- security
- spring
---

整理一种Spring Security Web认证实现方案，其核心组件及认证过程说明如下：

**实体设计说明：**
1. Function，权限对象，和Role对象为多对多关系；
2. Role，角色对象，实现GrantedAuthority和ConfigAttribute接口；Autority和Attribute都返回RoleId；
3. User，用户对象，实现UserDetails接口，和Role对象为多对多关系；

**服务设计说明：**
1. FilterSecurityInterceptor继承AbstractSecurityInterceptor，需注入AccessDecisionManager、AuthenticationManager、SecurityMetadataSource；
2. SecurityMetadataSource需根据请求信息获得ConfigAttribute信息；具体实现类根据请求URI获得Function，再根据Functon获得ConfigAttribute(Role)列表；
3. AuthenticationManager需注入UserDetailsService，对提交信息进行认证，并获得UserDetails(User)的GrantedAuthority(Role)列表；
4. AccessDecisionManager实现类中decide方法，对以上获得的ConfigAttribute(Role)列表和GrantedAuthority(Role)列表进行对比，判断是否包含相同的项，若有则有权限，否则无权限；


