---
author: admin
comments: true
date: 2014-03-25 01:50:06+00:00
excerpt: 'oracle create data and temporary  tablespace for new user '
layout: post
slug: oracle-create-data-and-temporary-tablespace-for-new-user
title: 'oracle create data and temporary tablespace for new user '
wordpress_id: 602
categories:
- Experience
tags:
- dataspace
- grant
- oracle
- privileges
- tablespace
---

创建表空间、临时表空间；创建账号并指定账号使用表空间；赋予新账号权限角色；

    -- tablespace
    CREATE tablespace MYPROJECT datafile 'D:\oracle\product\10.2.0\oratablespace\MYPROJECT.dbf'  SIZE 5m  autoextend ON NEXT  10m maxsize unlimited;
    
    -- temporary tablespace
    CREATE temporary tablespace MYPROJECT_TEMP tempfile 'D:\oracle\product\10.2.0\oratablespace\MYPROJECT_TEMP.dbf' SIZE 5m autoextend ON NEXT 10m maxsize unlimited;
    
    -- USER SQL
    CREATE USER MYUSER IDENTIFIED BY password
    DEFAULT TABLESPACE "MYPROJECT"
    TEMPORARY TABLESPACE "MYPROJECT_TEMP";
    
    -- ROLES
    GRANT "CONNECT","RESOURCE","DBA" TO MYUSER ;
    
    -- SYSTEM PRIVILEGES
    
    -- QUOTAS


如果要严格控制权限，不赋予DBA权限，权限赋予部分可以为赋予具体需要的权限，如：

    -- ROLES
    GRANT "CONNECT" TO MYUSER ;
    GRANT "RESOURCE" TO MYUSER ;
    
    -- SYSTEM PRIVILEGES
    GRANT EXECUTE ANY PROCEDURE TO MYUSER ;
    GRANT DROP ANY TRIGGER TO MYUSER ;
    GRANT DROP ANY VIEW TO MYUSER ;
    GRANT CREATE TRIGGER TO MYUSER ;
    GRANT ALTER ANY PROCEDURE TO MYUSER ;
    GRANT ALTER ANY TRIGGER TO MYUSER ;
    GRANT CREATE SYNONYM TO MYUSER ;
    GRANT ALTER ANY TABLE TO MYUSER ;
    GRANT CREATE TABLE TO MYUSER ;
    GRANT CREATE VIEW TO MYUSER ;
    GRANT CREATE ANY INDEX TO MYUSER ;
    GRANT DROP ANY INDEX TO MYUSER ;
    GRANT ALTER ANY INDEX TO MYUSER ;
    GRANT CREATE PUBLIC SYNONYM TO MYUSER ;
    GRANT CREATE ANY PROCEDURE TO MYUSER ;
    GRANT DROP ANY PROCEDURE TO MYUSER ;
    GRANT CREATE ANY SYNONYM TO MYUSER ;
    GRANT CREATE ANY TABLE TO MYUSER ;
    GRANT DROP PUBLIC SYNONYM TO MYUSER ;
    GRANT DROP ANY SYNONYM TO MYUSER ;
    GRANT DELETE ANY TABLE TO MYUSER ;
    GRANT CREATE ANY VIEW TO MYUSER ;
    GRANT CREATE PROCEDURE TO MYUSER ;
    GRANT CREATE ANY TRIGGER TO MYUSER ;
    GRANT COMMENT ANY TABLE TO MYUSER ;
    GRANT DROP ANY TABLE TO MYUSER ;
    
    
    grant select on v_$session to MYUSER ;
    grant select on v_$sesstat to MYUSER ;
    grant select on v_$statname to MYUSER ;


