---
author: admin
comments: true
date: 2013-08-04 06:34:40+00:00
excerpt: SSH Project Performance Tuning Analysis
layout: post
slug: ssh-project-performance-tuning-analiysis
title: SSH Project Performance Tuning Analysis
wordpress_id: 459
categories:
- Experience
tags:
- performance
- struts2
---

Last month, we tested the performance for a SSH project before deploying to production. When the synchronized user thread size was less than 30, the time cost of a page with DB query was 3-4 seconds. But when the size was over 30, the time would extend to 20-30 seconds gradually. The usages of CPU of AP and DB server were mantain at 20%-30%, The usages of memoery were enough and didn't increase obviously. And the intranet network is fast and wouldn't be the performance neck. We tried to test a page without DB query and time cost was less than 1 second, which gave us a superficial result the DB was slow and it should add cache mechanism to reduce DB query. But the performance didn't improve much with cache. 

We reviewed the whole enviroment again! The slow cpu usages of DB server means the DB is not he perfromance neck. So is it the application server Jboss?
The performance of a page without DB query is nice, so the application server works fine. Is it the VIEW level of MVC AP?
After removing the  block in JSP which show db query result, the performance became very good and the time cost was less than 1 seconds. Is there some performance issue for the  of struts2? I found the OGNL slow issue for struts2 from google, but it has been fixed in new versions. 

Though, it's sure the struts2 is the the performance neck. [The struts2 performance tuning page](http://struts.apache.org/release/2.0.x/docs/performance-tuning.html) first tells that the "struts.devMode" SHOULD BE set to false and we already did that. But two properties "struts.i18n.reload" and "struts.configuration.xml.reload" were still set to true. Changed them to false and test again, the whole performance became very good finally!


