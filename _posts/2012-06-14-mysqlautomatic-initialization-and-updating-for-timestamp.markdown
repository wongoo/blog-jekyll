---
author: admin
comments: true
date: 2012-06-14 06:57:42+00:00
excerpt: Automatic Initialization and Updating for TIMESTAMP in mysql
layout: post
slug: mysqlautomatic-initialization-and-updating-for-timestamp
title: '[MySql]Automatic Initialization and Updating for TIMESTAMP'
wordpress_id: 290
categories:
- Experience
tags:
- MSSQL
- mysql
- timestamp
---

It's found that Mysql can automatic initialize and update a TIMESTAMP field just using creating table declaration. MsSql can also finish this job through a trigger, but it's boring.




    CREATE TABLE t1 (
      f1 TIMESTAMP DEFAULT CURRENT_TIMESTAMP
                    ON UPDATE CURRENT_TIMESTAMP);




**Reference:**
[http://dev.mysql.com/doc/refman/5.0/en/timestamp-initialization.html](http://dev.mysql.com/doc/refman/5.0/en/timestamp-initialization.html)
[http://stackoverflow.com/questions/36001/sql-server-2005-auto-updated-datetime-column-lastupdated](http://stackoverflow.com/questions/36001/sql-server-2005-auto-updated-datetime-column-lastupdated)
