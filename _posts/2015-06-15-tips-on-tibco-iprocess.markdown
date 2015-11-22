---
author: admin
comments: true
date: 2015-06-15 03:02:41+00:00
layout: post
slug: tips-on-tibco-iprocess
title: Tips on TIBCO iProcess
wordpress_id: 841
categories:
- Experience
tags:
- iProcess
- Tibco
- tips
- webservice
---

From: http://it.toolbox.com/blogs/steel/tips-on-tibco-iprocess-34932

**1. iPE can’t started after installing iProcess Technology Plug-ins.**


Problem:

When I installed iPE, it started OK. But after I installed iProcess Technology Plug-ins and started iPE, some of the process always SUSPENDED status.

Reason:

Check the $SWDIR/logs/sw_warn, found following information:

2009/09/24 01:56:20.972188(BGPREDICT:1:10808:2::::pro:/proeai.c:80293:2531): 1867-'EAIJAVA' EAI Run-Time Plug-In failed to initialize, error SW_SYSCLASSPATH is not set!

Seems when we start iPE with iProcess Technology Plug-ins, the environment variable SW_SYSCLASSPATH is needed.

Resolve:

Change the folder to $SWDIR/eaijava/scripts, a shell file named env.sh can be found. Its content is like:

#!/bin/ksh

SW_SYSCLASSPATH=/data/iprocess/staffwar/eaijava/libs/bootstrap/log4j-1.2.13.jar:/data/iprocess/staffwar/eaijava/libs/bootstrap/bootstrap.jar

export SW_SYSCLASSPATH

#SWJVM_OPTIONS="-Xrs"

#export SWJVM_OPTIONS

After executing this shell file, the SW_SYSCLASSPATH variable will be set up. Or you can add this script into the .bash_profile of the OS user that started the iPE.

Restart the iPE, it can be started and all processes are RUNNING status again.

**2. Web Services Plug-ins registers failure.**



Problem:

After installing Web Services server Plug-ins on the iPE server, following error had been got when try to deploy BPM process:

ERROR : EAI Error - Step SERVICET: There is no runtime plugin of type EAI_WEBSERVICES installed on this system.

Reinstall the Web Services server Plug-ins, the error still occurred. Seems the plug-in can’t work in the iPE server.

Reason:

Check the $SWDIR/logs, found there where error information in the eairegerror.log.

/data/iprocess/staffwar/util/sweaireg: error while loading shared libraries: sal.so: cannot open shared object file: No such file or directory

This is the reason caused the WEB Service Plug-ins register failure.

Resolve:

In the Linux system, add $SWDIR/libs into LD_LIBRARY_PATH environment variable. And change the folder to $TIBCO_HOME/iPEWebServicesPlugIn/eaiwebsvcs/scripts, execute register.sh.

Check the $SWDIR/logs, there is no information in eairegerror.log, and in the eaireg.log, following information would be found:

EAI Run-Time Plug-In Registration Successful:

EAI Type: EAI_WEBSERVICES

Machine: 1

Version: 10.6.1

Copyright: Copyright (c) 2002-2009 TIBCO Software Inc.

Library: /data/iprocess/staffwar/lib/EAIWEBSVCS_10_6_1.EAR

Init Params: /data/iprocess/TIBCO/iPEWebServicesPlugIn/eaiwebsvcs

This means the EAI_WEBSERVICES registered successfully.

Caution: To register the EAI_WEBSERVICES, you must let the iPE server started.


**3. iProcess Service Task EAI call-out failed**



Problem:

Deploy a BPM process that contained a BW Service Task on the iPE server, start a case of this process. When the BW Service Task step executed, following error had been got:

2009/10/02 15:36:08.369724(BG:3:3402:2:STARTEXA:352:SERVICET:p ro:/proeai.c:80293:3601): 1869-Procedure:STARTEXA EAI Step:SERVICET Case:352 EAI plug-in EAIRun_Call() failed, error: Error inside execute method of Java object - com.staffware.eaijava.FatalPluginException: Execution failed

at com.staffware.eaijava.impl.SWGenericPlugin.execute( SWGenericPlugin.java:597)

Caused by: com.staffware.eaijava.FatalPluginException: Execution failed

at com.staffware.eaiframework.runtime.library.engine.S WFramework.executeSync(SWFramework.java:471)

at com.staffware.eaijava.impl.SWGenericPlugin.execute( SWGenericPlugin.java:542)

Caused by: com.staffware.eaijava.FatalPluginException: Unable to execut

2009/10/02 15:36:08.373083(BG:3:3402:0:STARTEXA:352:SERVICET:p ro:/cplmain.c:80415:2381): 1631-WARNING:  <-2>  

2009/10/02 15:36:08.373130(BG:3:3402:2:STARTEXA:352:SERVICET:p ro:/swbgmbd.c:81039:1100): 1864-Transaction failure : Reason code = -2

On the EMS server, can’t find any messages. That means, the BW Service Task can’t send the message to the EMS server.

Reason:

Check the eaijava.log found following information:

com.staffware.eaijava.FatalPluginException: Execution failed

... 1 more

Caused by: com.staffware.integration.db.library.impl.UnknownFi eldException: Object does not exist in database

... 3 more

2009-10-02 15:36:08,360 [ERROR] SWAliasControlDAO - Unable to find destination

com.staffware.integration.db.library.impl.UnknownFi eldException: Object does not exist in database

Caused by: com.staffware.integration.db.library.impl.UnknownFi eldException: Object does not exist in database

2009-10-02 15:36:08,363 [FATAL] SWFramework - Execution failed

com.staffware.eaijava.FatalPluginException: Unable to execute handler

... 1 more

Caused by: com.staffware.integration.db.library.impl.UnknownFi eldException: Object does not exist in database

... 3 more

2009-10-02 15:36:08,365 [ERROR] SWGenericPlugin - execute() - SWParserException

com.staffware.eaijava.FatalPluginException: Execution failed

Caused by: com.staffware.eaijava.FatalPluginException: Unable to execute handler

... 1 more

Caused by: com.staffware.eaiframework.runtime.library.handler. SWHandlerException: Unable to lookup JMS destination

... 1 more

Caused by: com.staffware.integration.db.library.impl.UnknownFi eldException: Object does not exist in database

Resolve:

Seems the jmsadmin didn't create the correct jndi in the EMS server. Create the jndiname using

create jndiname QUEUE_NAME queue QUEUE_NAME

Restart the procedure, the target BW process can be triggered.

**4. Error when deploy forms on WebDAV Server**


Problem:

Create a iProcess procedure in BS3.2 and generate some forms. In the deployment server, I had configured the WebDAV server. When I try to deploy module on this server, follow error had been got:

Deploying '\E-Commerce\Workspace Lite Forms\ProcessPackage'

Path is now ready on the server.

-------------

Deploying '\E-Commerce\Workspace Lite Forms\ProcessPackage\ECommerceProcess'

Unable to create collection '[Locator http://vm-linuxel4:18080/webdav/Forms/ProcessPackag e/ECommerceProcess/]' in the server. Server is not configured for the write access

Resolve:

Uncomment following lines in web.xml under  installation directory>/WEB-INF to enable write access .



readonly

false





**5. Configure iProcess Workspace (Browser) for Tomcat**


To configure the iProcess Workspace (Browser) for Tomcat, TIBCO iProcess Objects (Java) Software must be installed. All the jar files of TIBCO iProcess Objects (Java) Software should be copied to CATALINA_HOME/lib, libssoJNI.so file must be add to LD_LIBRARY_PAT variable.


6. iProcess Engine Pre Installation Configuration on Windows


Before installing iPE on Windows, following steps must be executed:

1. Control Panel->Administrative Tools->Component Services.

Select Component Services->Computers->My Computer, right key pop Properties->COM Security->Access Permissions->Edit Default, add current user and allow all

2. Control Panel->Administrative Tools->Local Security Policy->Local policies->User Rights Assignment.

Add current user and Administrators Group to follow items:

(1) Act as part of operating system

(2) Logon as a service

(3) Logon as a batch job

3. If your iPE is SQL Server version, you must do the following steps:

(1) Create new database, for example, [iprocessdb]

(2) Create new user [swpro] and [swuser], both of them add db_owner roles on iprocessdb.

(3) Create ODBC for iprocessdb.

During the installation you must select BASIC, if it’s your first installation.


**7. Unlock the User or Procedure in iPE**



Occasionally, Users or Procedures may be locked in the iPE. For example, the iPE crashed when a user login in it or Modeler crashed during the user open a procedure. After the iPE or Modeler restart, the locked users or procedures can’t be used. To unlock the user and procedure, you should modify the data in iPE database, all the tables belongs to swpro.

1. Unlock User

Modify the data in FLAT_TABLE, clean the USER_NAME field and set the AREA_LOCKED field as 0 if its value is 1.

2. Unlock Procedure

Following SQL script can be used to unlock the procedures:

delete swpro.procedure_lock where proc_id = (select proc_id from swpro.proc_index where proc_name like '%1')

delete swpro.pm_objects_lock where object_guid = (select object_guid from swpro.pm_objects where object_name like '%1')

‘%1’ is the procedure name.


**8. How to resolve the issue after changing hostname**


1. Run swconfig -v and save the output (licence period and serial number required for a new licence)

2. Backup the current licence file (%SWDIR%\staff.dat) (ex. copy staff.dat staff.dat.old)

3. Run the following command to create a new licence file with the correct hostname (this is a demonstration licence)

%SWDIR%\util\swconfig -n

4. Reset the database passwords for user swpro (background user) and swuser (foreground user)

%SWDIR%\util\swconfig -u

HINT: the passwords will be reset to the default one when you create a new licence

5. Start the services

If the server is now running you need a new full licence. To get one send an eMail to fulfillment@tibco.com with the new output of swconfig -v and the old information (serial number, time period).

