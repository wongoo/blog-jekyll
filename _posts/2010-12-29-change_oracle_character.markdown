---
author: admin
comments: true
date: 2010-12-29 14:31:14+00:00
layout: post
slug: change_oracle_character
title: 更改Oracle数据库编码
wordpress_id: 49
categories:
- Experience
---

SQL*Plus: Release 10.2.0.1.0 - Production on 星期三 12月 29 22:23:55 2010

Copyright (c) 1982, 2005, Oracle.  All rights reserved.

SQL> conn system/password as sysdba
已连接。
SQL> shutdown immediate;
数据库已经关闭。
已经卸载数据库。
ORACLE 例程已经关闭。
SQL> startup mount;
ORACLE 例程已经启动。

Total System Global Area  411041792 bytes
Fixed Size                  1287736 bytes
Variable Size             121637320 bytes
Database Buffers          285212672 bytes
Redo Buffers                2904064 bytes
数据库装载完毕。
SQL> ALTER SYSTEM ENABLE RESTRICTED SESSION;

系统已更改。

SQL>  ALTER SYSTEM SET JOB_QUEUE_PROCESSES=0;

系统已更改。

SQL> ALTER DATABASE OPEN;

数据库已更改。

SQL> ALTER DATABASE CHARACTER SET INTERNAL_USE** ZHS16GBK**;

数据库已更改。

SQL> SHUTDOWN IMMEDIATE;
数据库已经关闭。
已经卸载数据库。
ORACLE 例程已经关闭。
SQL>  STARTUP;
ORACLE 例程已经启动。

Total System Global Area  411041792 bytes
Fixed Size                  1287736 bytes
Variable Size             121637320 bytes
Database Buffers          285212672 bytes
Redo Buffers                2904064 bytes
数据库装载完毕。
数据库已经打开。
SQL> select userenv('language') from dual;

USERENV('LANGUAGE')
----------------------------------------------------
SIMPLIFIED CHINESE_CHINA.ZHS16GBK

SQL>
