---
author: admin
comments: true
date: 2015-06-15 03:49:52+00:00
layout: post
slug: tibco-iprocess-activity-monitoring-and-work-queue-delta-publication
title: Tibco iProcess Activity Monitoring and Work Queue Delta Publication
wordpress_id: 855
categories:
- Experience
tags:
- iProcess
- monitor
- Tibco
---

The TIBCO iProcess Engine can be enabled to publish both iProcess Engine activity information and iProcess Engine Work Queue Deltas to external applications, using JMS queues.

An activity is any instruction in the iProcess Engine that creates an audit trail entry, for example, Case started or Event Issued. You can configure any combination of step and/or activity to be monitored. This enables an external application to monitor important business events during the processing of cases.

A BG process can identify if a step is being processed and if activity monitoring has been configured for it. The BG process then sends details of the configured activities in XML format to the IAPJMS process.

Work Queue Delta publication enables an external application (iProcess Server Objects (Java) or iProcess Server Objects (.NET)) to monitor a queue and to retrieve only those work items in a given work queue that have changed since the subscription started. The WIS process sends messages reporting Work Queue changes to the IAPJMS process.

For both activity monitoring and Work Queue Delta publication, the IAPJMS process sends the XML message to a specified JMS topic, from which an external application (for example, iProcess Objects, iProcess Analytics or an external application that you have written yourself) can receive the JMS messages.

**首次部署**
	1.将文件PSBC_IPE_MONITOR.xml放置到/opt/bpm/swnod1/util
	2.创建pro用户
	3.执行脚本
		su - pro
		cd /opt/bpm/swnod1/util
		./swadm set_attribute 0 ALL 0 IAPJMS_PUBLISH 1
		./swadm set_attribute 0 ALL 0 IAPJMS_SYNCHRONOUS 0
		./swadm set_attribute 0 ALL 0 IAPJMS_ROLLBACK 0

	4.修改文件$SWDIR/etc/staffcfg
		STAFF/IAPSCHEMA==>1

	5.注册监听步骤
		cd /opt/bpm/swnod1/bin
		./swutil IMPMONITOR PSBC_IPE_MONITOR.xml

	6.在EMS创建TOPIC
		cd /opt/tibco/ems/8.0/bin
		./tibemsadmin
		create topic IAPTOPIC
		
		
**后续部署-需监听其他流程**
	1.修改PSBC_IPE_MONITOR.xml文件并将该文件放置到/opt/bpm/swnod1/util
	2.注册监听步骤
		su - pro
		cd /opt/bpm/swnod1/bin
		./swutil IMPMONITOR PSBC_IPE_MONITOR.xml
		
