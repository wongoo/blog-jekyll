---
author: admin
comments: true
date: 2013-09-13 09:10:51+00:00
excerpt: Oracle expdp/impdp command sample
layout: post
slug: oracle-expdpimpdp-command-sample
title: Oracle expdp/impdp command sample
wordpress_id: 499
categories:
- Experience
tags:
- expdp
- impdp
- oracle
- sql
---

**Command Sample**

    
     create or replace directory DPUMP_DIR as '/oracle/dump';
     select directory_path from dba_directories where upper(directory_name) = 'DPUMP_DIR';
     grant all on directory DPUMP_DIR to user;
     expdp user/userpass@ORCL directory=DPUMP_DIR dumpfile=DATA_PUMP.dmp logfile=DATA_PUMP.log
     impdp user/userpass@ORCL directory=DPUMP_DIR dumpfile=DATA_PUMP.dmp logfile=DATA_PUMP.log
    


**Trouble Shooting**
1.impdp error:ORA-06512: at "SYS.UTL_FILE"

    
     sh>impdp ccas/password directory=dump_dir dumpfile=ccasdb%u.dmp parallel=5
     ORA-39002: invalid operation
     ORA-39070: Unable to open the log file.
     ORA-29283: invalid file operation
     ORA-06512: at "SYS.UTL_FILE", line 488
     ORA-29283: invalid file operation
     因/ccas/dmp目录的权限为drwxr-xr-x，其它用户没有写权限，而impdp运行时的权限是oracle用户的权限，
     而不是当前登录用户的权限，impdp不能写dmp目录，所以需将dmp目录加上o+w权限。
     sh>chmod 777 /ccas/dmp
     sh>ls –l /ccas
     drwxrwxrwx   2 ccas       ccas          1024 6月 4日 09:40 dmp
     重新运行impdp后成功
    


**Reference:**
1.impdp(Data Pump Import),[http://docs.oracle.com/cd/B19306_01/server.102/b14215/dp_import.htm](http://docs.oracle.com/cd/B19306_01/server.102/b14215/dp_import.htm)
2.expdp(Data Pump Export),[http://docs.oracle.com/cd/E11882_01/server.112/e22490/dp_export.htm](http://docs.oracle.com/cd/E11882_01/server.112/e22490/dp_export.htm)
