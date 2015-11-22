---
author: admin
comments: false
date: 2011-11-30 08:25:39+00:00
excerpt: MSSQL Export/Import Table Data with image type field
layout: post
slug: mssql-exportimport-table-data-with-image-type-field
title: MSSQL Export/Import Table Data with image type field
wordpress_id: 172
categories:
- Experience
tags:
- Export
- Import
- MSSQL
---

1. export table data to file:


    EXEC master..xp_cmdshell 'BCP "SELECT * FROM mydb.odb.BASE_ATTACHMENT" queryout "c:/base_attach.dat" -c -U"sa" -P"sa"'


2. import file data to table:


    EXEC master..xp_cmdshell 'BCP mydb.dbo.BASE_ATTACHMENT in c:/base_attach.dat -c -U"sa" -P"sa"'

or

    EXEC master..xp_cmdshell 'BCP mydb.dbo.BASE_ATTACHMENT in d:/base_attach.dat -c -S"192.168.1.111" -U"user" -P"password"'






* * *



NOTE: if get the following error:

SQL Server 阻止了对组件 'xp_cmdshell' 的 过程 'sys.xp_cmdshell' 的访问，因为此组件已作为此服务器安全配置的一部分而被关闭。系统管理员可以通过使用 sp_configure 启用 'xp_cmdshell'。有关启用 'xp_cmdshell' 的详细信息，请参阅 SQL Server 联机丛书中的 "外围应用配置器"。

Please execute:



* * *




    EXEC sp_configure 'show advanced options', 1   -- 允许配置高级选项
    GO
    RECONFIGURE   -- 重新配置
    GO
    EXEC sp_configure 'xp_cmdshell', 1  -- 启用xp_cmdshell
    GO
    RECONFIGURE  --重新配置
    GO




* * *



Reference:
Importing and Exporting Data , [http://technet.microsoft.com/en-us/library/cc917564.aspx](http://technet.microsoft.com/en-us/library/cc917564.aspx)
bcp Utility, [http://technet.microsoft.com/en-us/library/ms162802.aspx](http://technet.microsoft.com/en-us/library/ms162802.aspx)
