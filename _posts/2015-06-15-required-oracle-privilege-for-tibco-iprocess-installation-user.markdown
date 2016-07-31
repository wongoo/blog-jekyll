---
author: admin
comments: true
date: 2015-06-15 04:11:25+00:00
layout: post
slug: required-oracle-privilege-for-tibco-iprocess-installation-user
title: Required Oracle Privilege for Tibco iProcess installation user
wordpress_id: 857
categories:
- Knowledge
tags:
- iProcess
- oracle
- privelge
- Tibco
---

Tibco iProcess安装db帐号创建有两种方式，一种是给管理员帐号，安装的时候自动创建；另一种是提前创建好，安装的时候选择已经创建好的db帐号。 生产环境db帐号管控严格，需要提前创建帐号，需要知道帐号都需要什么权限。测试环境安装是通过第一种方式自动创建的，通过以下两个语法可以查看帐号都需要那些权限。

select * from session_privs;
-------------------------------
	CREATE SESSION
	UNLIMITED TABLESPACE
	CREATE TABLE
	CREATE CLUSTER
	CREATE VIEW
	CREATE SEQUENCE
	CREATE PROCEDURE
	CREATE TRIGGER
	CREATE TYPE
	CREATE OPERATOR
	CREATE INDEXTYPE
	QUERY REWRITE
	MANAGE ANY QUEUE
	ENQUEUE ANY QUEUE
	DEQUEUE ANY QUEUE
	CREATE EVALUATION CONTEXT
	CREATE RULE SET
	CREATE RULE

select * from user_role_privs;
--------------------------------------------
	   	USERNAME	GRANTED_ROLE	ADMIN_OPTION	DEFAULT_ROLE	OS_GRANTED
	1	SWPRO	AQ_ADMINISTRATOR_ROLE	NO	YES	NO
	2	SWPRO	CONNECT	NO	YES	NO
	3	SWPRO	RESOURCE	NO	YES	NO

但安装的时候报执行初始化SQL语法错误，经查是对其中一个package body **SSOLITE_MSG**进行编译的时候报错，这个package body有引用DBMS_AQ,但报错DBMS_AQ没有定义，原因是没有DBMS_AQ执行权限，按照如下赋予权限重新安装即可：

	GRANT EXECUTE ON DBMS_AQ TO swpro;
	GRANT EXECUTE ON DBMS_AQ TO swuser;

