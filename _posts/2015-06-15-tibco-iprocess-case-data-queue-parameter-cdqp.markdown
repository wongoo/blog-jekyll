---
author: admin
comments: true
date: 2015-06-15 03:28:06+00:00
layout: post
slug: tibco-iprocess-case-data-queue-parameter-cdqp
title: Tibco iProcess Case Data Queue Parameter (CDQP)
wordpress_id: 848
categories:
- Experience
tags:
- CDQP
- iProcess
- Tibco
---

Tibco iProcess Case Data Queue Parameter (CDQP) fields provide an efficient method of filtering on the value of fields in your work items. To make use of this functionality, you must first pre-designate the fields you want to filter on as CDQP fields. Fields are designated as CDQP fields with the utility, swutil. This utility is used to create a list, on the TIBCO iProcess Engine, of the case data fields that are available to use for filtering. See the TIBCO iProcess Engine Administrator's Guide for information about using swutil.
=======================================================================================
To enter CDQP commands, use the following command:
./swutil QINFO 

To add a field as a CDQP, or to change the definition of an existing CDQP, use the command:
	F+FieldName,FieldSize,WQMDescription[,P]
where:
	>> FieldName is the name of the iProcess field to be defined as a CDQP. FieldName must be less than or equal to 15 characters. 
		If the field is a composite field of the form TagName:FieldName, TagName must be less than 15 characters and FieldName must be less than 15 characters.
	>> FieldSize is the size, in characters, of the CDQP. FieldSize must be less than or equal to 255.
	>> WQMDescription is the name that will be used to represent this field in Work Queue Manager dialogs. 
		WQMDescription must be unique, and must be less than or equal to 39 characters.

=======================================================================================
To validate a set of new or updated CDQP configuration data before importing it, use the following command:
./swutil QINFO C:\TEMP\MYPROC.XFR PARSEONLY

=======================================================================================
To import a set of new or updated CDQP configuration data to the server, use the following command:
./swutil QINFO C:\TEMP\MYPROC.XFR

=======================================================================================
To import a set of new or updated CDQP configuration data to the server and publish it immediately, use the following command:
./swutil QINFO C:\TEMP\MYPROC.XFR PUBLISH

--------------------------------------
[pro@IPESERVER bin]$ ./swutil QINFO /opt/swnod1/bin/cdqp.xfr PUBLISH
QINFO import: Started by 'swadmin' at 30/12/2014 15:42

QINFO import: Processing control file

lines processed successfully = 9
Control File Warnings:  = 0
QINFO import: Completed at 30/12/2014 15:42

./swutil QINFO /opt/swnod1/bin/cdqp1.xfr PUBLISH
=======================================================================================
You can export CDQP definitions and mappings - either for the entire system, or for specific queues - using the following command:
./swutil QINFO EXPORT [queuename [GLOBAL]]
./swutil QINFO EXPORT


--------------------------------------
[pro@IPESERVER bin]$ ./swutil QINFO EXPORT
R

;Global CDQPs
F+RELEASETYPE,1,releaseType
F+TELUSERTYPE,1,telUserType

;Default Group CDQPs
Q+SW_DEFAULT_CASEDATAQPARAM_GROUP
; d1
C+TELUSERTYPE
; d2
C+RELEASETYPE

;Default User CDQPs
Q+SW_DEFAULT_CASEDATAQPARAM_USER
; d1
C+TELUSERTYPE
; d2
C+RELEASETYPE
