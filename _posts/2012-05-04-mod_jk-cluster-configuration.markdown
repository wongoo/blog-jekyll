---
author: admin
comments: true
date: 2012-05-04 10:23:37+00:00
excerpt: mod_jk cluster configuration
layout: post
slug: mod_jk-cluster-configuration
title: mod_jk cluster configuration
wordpress_id: 263
categories:
- Experience
tags:
- AJP
- apache
- cluster
- mod_jk
- Tomcat
---

## Config Steps:


**1. [download](http://tomcat.apache.org/download-connectors.cgi) mod_jk module and put it under the module directory of apache.**




**2. add content in conf/http.conf**


Include conf/extra/httpd-mod_jk.conf




**3. create file conf/extra/httpd-mod_jk.conf:**


# Load mod_jk moduleLoadModule
jk_module  modules/mod_jk.so

# Where to find workers.propertiesJkWorkersFile
mod_jk/workers.properties

# Where to put jk shared memoryJkShmFile
mod_jk/mod_jk.shm

# Where to put jk logs
JkLogFile logs/mod_jk.log# Set the jk log level [debug/error/info]
JkLogLevel debug

# Select the timestamp log format
JkLogStampFormat "[%a %b %d %H:%M:%S %Y]"

#Rules defined in uriworkermap.properties come first, before rules defined by JkMount (Apache) and inside workers.properties using the mount attribute.

# When a request is being processed, tomcat connectors check the file modification time of the uriworkermap file.
JkMountFile mod_jk/uriworkermap.properties

# To keep the performance penalty low, this happens only, if the last check happened at least n seconds ago.
JkMountFileReload 1800

# If the Tomcat Host appBase (webapps) directory is accessible (on the same server) by the Apache web server,
# Apache can be configured to serve web application context directory static files instead of passing the request to Tomcat.






**4. create file mod_jk/workers.properties**




# The workers that jk should create and work withworker.list=wlb,jkstatus# SERVER 1
# -------------------------------------------------------------------------------------

# Type of the worker (can be one of ajp13, ajp14, jni, lb or status)
# AJP13 worker is the preferred worker type that JK uses for communication between web server and Tomcat.
# This type of worker uses sockets as communication channel.
# For detailed description of the AJP13 protocol stack browse to AJPv13 protocol specification

# Defining a worker named ajp13w and of type ajp13
# Note that the name and the type do not have to match.

worker.ajp13w206.type=ajp13
worker.ajp13w206.host=192.168.5.206
worker.ajp13w206.port=13771

# Socket timeout in seconds used for the communication channel between JK and remote host.
# If the remote host does not respond inside the timeout specified, JK will generate an error, and retry again.
# If set to zero (default) JK will wait for an infinite amount of time on all socket operations.
# its better to define this for load balancer env.
worker.ajp13w206.socket_timeout=30

#Only used for a member worker of a load balancer.
#The integer number lbfactor (load-balancing factor) is how much we expect this worker to work, or the worker's work quota.
worker.ajp13w206.lbfactor=1



# SERVER 2
# -------------------------------------------------------------------------------------
worker.ajp13w111.type=ajp13
worker.ajp13w111.host=192.168.5.111
worker.ajp13w111.port=13771
worker.ajp13w111.socket_timeout=30
worker.ajp13w111.lbfactor=1

# Defining a load balancer
# -------------------------------------------------------------------------------------
worker.wlb.type=lb
worker.wlb.balance_workers=ajp13w206,ajp13w111
worker.wlb.sticky_session=True
worker.retries=3



# Define status worker
# -------------------------------------------------------------------------------------
worker.jkstatus.type=status






**5. create file mod_jk/uriworkermap.properties:**




# Exclusion rules are applied after all normal rules have been applied.
# If the Tomcat Host appBase (webapps) directory is accessible (on the same server) by the Apache web server
# Apache can be configured to serve web application context directory static files instead of passing the request to Tomcat.

# normal rules for status
# ----------------------------------------------
/jkstatus=jkstatus# normal rules
# ----------------------------------------------
/bill|/*=wlb
# exclusion rules
# ----------------------------------------------

!/*.html=wlb
!/*.htm=wlb
!/*.css=wlb
!/*.js=wlb
!/*.jpg=wlb
!/*.png=wlb
!/*.gif=wlb






**6. copy the static content to the apache directory**.(eg, <apache_home>/htdocs/bill/)






## ****
NOTE:****





	
  1. If you want to use session stickiness, you must set different jvmRoute attributes in the Engine element in Tomcat's server.xml. Furthermore the names of the workers which are managed by the balancer have to be equal to the jvmRoute of the Tomcat instance they connect with.

	
  2. Session stickiness and perfect load balancing are conflicting targets, especially when the number of sessions is small, or the usage of sessions is extremely varying For huge numbers of sessions this usually is not a problem.

	
  3. Set sticky_session to False when Tomcat is using a [Session Manager ](http://tomcat.apache.org/tomcat-5.5-doc/cluster-howto.html)which can persist session data across multiple instances of Tomcat.


**REFERENCE:**



	
  1. [the Apache Tomcat Connector – Webserver HowTo](http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html)

	
  2. [The Apache Tomcat Connector – AJP Protocol Reference](http://tomcat.apache.org/connectors-doc/ajp/ajpv13a.html)

	
  3. [How To Configure Tomcat to work with Apache](http://www3.ntu.edu.sg/home/ehchua/programming/howto/ApachePlusTomcat_HowTo.html)

	
  4. [Fronting Tomcat with apache](http://people.apache.org/~mturk/docs/article/ftwai.html)

	
  5. [Proxying Tomcat with Apache HTTP Server](http://www.harezmi.com.tr/proxying-tomcat-with-apache-http-server/?lang=en)

	
  6. [workers.properties configuration](http://tomcat.apache.org/connectors-doc/reference/workers.html)

	
  7. [The Engine Container](http://tomcat.apache.org/tomcat-5.5-doc/config/engine.html)

	
  8. [uriworkermap.properties configuration](http://tomcat.apache.org/connectors-doc/reference/uriworkermap.html)


