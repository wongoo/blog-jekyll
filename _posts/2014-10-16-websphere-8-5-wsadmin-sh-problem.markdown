---
author: admin
comments: true
date: 2014-10-16 16:46:38+00:00
excerpt: websphere 8.5启动wsadmin.sh失败问题解决过程
layout: post
slug: websphere-8-5-wsadmin-sh-problem
title: websphere 8.5启动wsadmin.sh失败问题解决过程
wordpress_id: 738
categories:
- Experience
tags:
- IBM
- websphere
- wsadmin
---

第一次尝试启动 wsadmin.sh 失败，console提示错误:

    WASX7093I: Issuing message: "WASX7011E: Cannot find file "$(was.install.root)/bin/securityProcs.jacl""
    WASX7093I: Issuing message: "WASX7011E: Cannot find file "$(was.install.root)/bin/LTPA_LDAPSecurityProcs.jacl""
    WASX7093I: Issuing message: "WASX7023E: Error creating "SOAP" connection to host "localhost"; exception information: com.ibm.websphere.management.exception.ConnectorNotAvailableException: [SOAPException: faultCode=SOAP-ENV:Client; msg=Error parsing HTTP status line "": java.util.NoSuchElementException; targetException=java.lang.IllegalArgumentException: Error parsing HTTP status line "": java.util.NoSuchElementException]"


是路径无法转换的错误，故将其配置为固定路径，修改/opt/IBM/ODM85/WAS/AppServer/properties/wsadmin.properties文件，将以下配置：

    com.ibm.ws.scripting.traceFile=$(was.install.root)/logs/wsadmin.traceout 
    com.ibm.ws.scripting.profiles=$(was.install.root)/bin/securityProcs.jacl;$(was.install.root)/bin/LTPA_LDAPSecurityProcs.jacl

修改为：

    com.ibm.ws.scripting.traceFile=/opt/IBM/ODM85/WAS/AppServer/logs/wsadmin.traceout 
    com.ibm.ws.scripting.profiles=/opt/IBM/ODM85/WAS/AppServer/bin/securityProcs.jacl;/opt/IBM/ODM85/WAS/AppServer/bin/LTPA_LDAPSecurityProcs.jacl


再次执行swadmin.sh，还是提示Error creating "SOAP" connection to host "localhost"，查看wsadmin.traceout发现找不到ssl.client.props文件的错误：

    [10/16/14 22:58:22:452 CST] 00000001 SSLConfigMana E   CWPKI0019E: Error parsing the SSL client configuration file file:/opt/IBM/ODM85/WAS/AppServer/profiles/odm850/properties/ssl.client.props.  The error returned is java.io.FileNotFoundException:file:/opt/IBM/ODM85/WAS/AppServer/profiles/odm850/properties/ssl.client.props.

经查找（find -name ssl.client.props）WAS下有很多模版文件，拷贝一个ssl.client.props文件到对应目录：

    cp /opt/IBM/ODM85/WAS/AppServer/profiles/odm850/temp/ssl.client.props /opt/IBM/ODM85/WAS/AppServer/profiles/odm850/properties/


再次执行swadmin.sh，提示以下错误：

    WASX7246E: Cannot establish "SOAP" connection to host "localhost" because of an authentication failure. Ensure that user and password are correct on the command line or in a properties file.
    Exception message (if any): "ADMC0035E: No user name has been specified to create a secure SOAP administrative client."

查看wsadmin.traceout发现是因为找不到soap.client.props文件：

    [10/17/14 0:34:00:528 CST] 00000001 AdminClientFa W   ADMC0046W: The system failed to load properties from com.ibm.SOAP.configURL: file:/opt/IBM/ODM85/WAS/AppServer/profiles/odm850/properties/soap.client.props

同样查找（find -name soap.client.props）WAS下有很多模版文件，拷贝一个soap.client.props文件到对应目录：

    cp /opt/IBM/ODM85/WAS/AppServer/profiles/odm850/temp/soap.client.props /opt/IBM/ODM85/WAS/AppServer/profiles/odm850/properties/


再次执行swadmin.sh，提示以下权限不足的错误：

    Exception message (if any): "ADMN0022E: Access is denied for the getProcessType operation on Server MBean because of insufficient or empty credentials."

修改/opt/IBM/ODM85/WAS/AppServer/profiles/odm850/properties/soap.client.props文件，设置帐号密码：

    com.ibm.SOAP.loginUserid=admin
    com.ibm.SOAP.loginPassword=password


再次执行swadmin.sh，登录成功：

    WASX7209I: Connected to process "server1" on node localhostNode01 using SOAP connector;  The type of process is: UnManagedProcess
    WASX7029I: For help, enter: "$Help help"
    wsadmin>


Reference：
1. ssl.client.props client configuration file，[http://www-01.ibm.com/support/knowledgecenter/api/content/SSCKBL_8.5.5/com.ibm.websphere.nd.multiplatform.doc/ae/rsec_sslclientpropsfile.html](http://www-01.ibm.com/support/knowledgecenter/api/content/SSCKBL_8.5.5/com.ibm.websphere.nd.multiplatform.doc/ae/rsec_sslclientpropsfile.html)
2. Using the Administration Thin Client，[http://www-01.ibm.com/support/knowledgecenter/SSCKBL_8.5.5/com.ibm.websphere.nd.multiplatform.doc/ae/txml_adminclient.html](http://www-01.ibm.com/support/knowledgecenter/SSCKBL_8.5.5/com.ibm.websphere.nd.multiplatform.doc/ae/txml_adminclient.html)

