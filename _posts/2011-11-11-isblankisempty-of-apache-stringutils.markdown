---
author: admin
comments: false
date: 2011-11-11 08:41:19+00:00
layout: post
slug: isblankisempty-of-apache-stringutils
title: isBlank&isEmpty of Apache StringUtils
wordpress_id: 165
categories:
- Experience
tags:
- apache
- java
- string
---

StringUtils.isBlank(null); => true
StringUtils.isBlank("");   => true
StringUtils.isBlank("   ");  => true
StringUtils.isEmpty(null);  => true
StringUtils.isEmpty("");    => true
StringUtils.isEmpty("   ");   => false
