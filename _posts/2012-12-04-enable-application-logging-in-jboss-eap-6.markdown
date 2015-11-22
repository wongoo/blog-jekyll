---
author: admin
comments: true
date: 2012-12-04 04:03:00+00:00
excerpt: Enable Application Logging in Jboss Enterprise Application Platform 6
layout: post
slug: enable-application-logging-in-jboss-eap-6
title: Enable Application Logging in Jboss EAP 6
wordpress_id: 368
categories:
- Experience
tags:
- jboss
- JCL
- log4j
- logging
- SLF4J
---

1. Create common-logging.properties with the following content and put it under class path:

    org.apache.commons.logging.Log=org.apache.commons.logging.impl.Log4JLogger
    log4j.configuration=log4j.properties


2.Create a jboss-deployment-structure.xml with the following content, and place the jboss-deployment-structure.xml file in either the META-INF/ directory or the WEB-INF/ directory if you are deploying a WAR, or in the META-INF/ directory if you are deploying an EAR.

    
    
    	
    		
    		
    			
    			
    			
    		
    	
    


3.Include the log4j.properties or log4j.xml file in the lib/ directory in your deployment.
4.Start the JBoss Enterprise Application Platform 6 server with the following runtime argument to prevent a ClassCastException from appearing in the console when you deploy the application: -Dorg.jboss.as.logging.per-deployment=false (modify the JAVA_OPTS in the file standalone.conf.bat for windows OR standalone.conf for linux)

5.Deploy your application.

Reference:
1. Jboss EAP Migration Guide, 3.1.4.2. Update Application Code for Third-party Logging Frameworks, [https://access.redhat.com/knowledge/docs/JBoss_Enterprise_Application_Platform/](https://access.redhat.com/knowledge/docs/JBoss_Enterprise_Application_Platform/)
