---
author: admin
comments: true
date: 2015-06-15 03:11:15+00:00
layout: post
slug: single-tibco-iprocess-node-installation-steps
title: Single Tibco iProcess Node Installation Steps
wordpress_id: 843
categories:
- Experience
tags:
- installation
- iProcess
- Tibco
---

================
**PREVIOUS INSTALLATION**
================
1. install oracle client;
2. copy tibco EMS library to a directory ;

================
**Environment**
================
192.168.5.224 CWVM224
192.168.5.225 CWVM225

192.168.5.220 bpmdb
192.168.5.221 emsserver

================
**INSTALLATION STEPS**
================
1. create user and group:	
	groupadd -g 502 staffwar
	adduser pro -u 502 -g staffwar

2. configure /etc/profile or /home/pro/.bash_profile:
	ORACLE_BASE=/u01/app/oracle; export ORACLE_BASE
	ORACLE_HOME=$ORACLE_BASE/oracle/11.2.0.2/db_01; export ORACLE_HOME
	ORACLE_SID=ORCL; export ORACLE_SID
	PATH=/usr/sbin:$PATH; export PATH
	PATH=$ORACLE_HOME/bin:$PATH; export PATH
	LD_LIBRARY_PATH=$ORACLE_HOME/lib:/lib:/usr/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH
	CLASSPATH=$CLASSPATH:$ORACLE_HOME/JRE:$ORACLE_HOME/jlib:$ORACLE_HOME/rdbms/jlib; export CLASSPATH 

	ulimit -n 65535
	NLS_LANG=AMERICAN_AMERICA.AL32UTF8; export NLS_LANG
	TISOUnicodeConverterName=UTF-8; export TISOUnicodeConverterName
	TISOMultiChar=1; export TISOMultiChar
	
	JAVA_HOME=/opt/bpm/swnod1/java; export JAVA_HOME
	PATH=$PATH:$JAVA_HOME/bin; export PATH

	SWDIR=/opt/bpm/swnod1; export SWDIR
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SWDIR/libs; export LD_LIBRARY_PATH

3. Execute the installer (./swinstall) as the user pro.
	su - pro
	./swinstall
	
4. installation detail:
	1) agree License
	2) choose the installation directory /opt/bpm/swnod1, and ignore version warning
	3) chose install files and schema:		
	4) configure location, OS accounts menu; 
		pro as background user
		swadmin as administration user
		staffwar as user group
	5) configuration options menu
		iProcess Engine Client Password Required? (Y/N - default Y): N
		Enable Prediction? (Y/N - default Y) : Y
		Enable Activity Publication? (Y/N - default N) : Y
		Enable iProcess Objects Director? (Y/N - default N) : Y
	6) configure IAP using tibco EMS;
	7) skip email plugin configuration
	8) configure oracle db connection  and account detail;
		tns: ORCL
		db admin: system
		db schema: swpro3
		db user:swuser3
		support unicode encoding:Y
		table space: STAFFWAR3 , size: 2048M, path:""
		temporary table space: TEMP
	9) configuration check
	10) installation processing ...
	11) chose Y to verify installation
	12) finish 
	
================
**POST CHECK STEPS**
================
1. configure /etc/profile or /home/pro/.bash_profile:
	SW_SYSCLASSPATH=$SWDIR/eaijava/libs/bootstrap/log4j-1.2.13.jar:$SWDIR/eaijava/libs/bootstrap/bootstrap.jar;export SW_SYSCLASSPATH
	
2. configure /opt/bpm/swnod1/seo/data/swentobjsv.cfg:
	SEOPasswordRequired = 0
	TCPServiceName = 1145
	DBConnectionAccess = 1
	IAPConfigAccess = 1

3. configure db pool in file /opt/bpm/swnod1/etc/staffcfg:
	POOLSIZE,20
	POOLGROWSIZE,10
	MAXPOOLSIZE,100
	
4. test start and stop:
	su - pro
	$SWDIR/bin/swstart –p
	$SWDIR/util/swsvrmgr status -v
	$SWDIR/bin/swstop –p

