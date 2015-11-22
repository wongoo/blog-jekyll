---
author: admin
comments: true
date: 2013-04-06 02:53:49+00:00
excerpt: Kerberos Protocol Learning Note
layout: post
slug: kerberos-protocol-learning-note
title: Kerberos Protocol Learning Note
wordpress_id: 417
categories:
- Knowledge
tags:
- kerberos
- protocol
- security
- sso
---

**What's Kerberos Protocol?**

1. Its designers aimed primarily at a client–server model, and it provides mutual authentication—both the user and the server verify each other's identity.
2. Kerberos protocol messages are protected against eavesdropping and replay attacks.
3. Kerberos builds on symmetric key cryptography and requires a trusted third party, and optionally may use public-key cryptography during certain phases of authentication.
4. Kerberos uses port 88 by default.

** Who use Kerberos?**

Microsoft uses the Kerberos protocol, it does not use the MIT software.Many UNIX and UNIX-like operating systems, including FreeBSD, Apple's Mac OS X, Red Hat Enterprise Linux, Oracle's Solaris, IBM's AIX and Z/OS, HP's OpenVMS, Univention's Univention Corporate Server and others, include software for Kerberos authentication of users or services.

**Kerberos Protocol Description**

KDC:Key Distribution Center
C: The Client
AS: Authentication Server
SS: Service Server
CID: Client ID
SID: The request service ID
IP: Client Network Address
TGS: Ticket Granting Service
K_USER: hash(user_password)
K_TGS: The key of TGS
K_SS: The key of Serivce Server
SK_C/TGS: Client/TGS Session Key
SK_C/SS: Client/SS Session Key
TGT: Ticket Granting Ticket {CID,IP,validity_period,K_C/TGS}
SGT: Client to Server Ticket(Service Granting Ticket) {CID,IP,validity_period,K_C/SS}

1. C->AS :
CID

2. AS->C :
K_USER(SK_C/TGS) --> Decrypted by client to get SK_C/TGS
K_TGS(TGT)

3. C->TGS:
K_TGS(TGT),SID --> Decrypted by TGS to valid TGT
SK_C/TGS(CID,Timestamp) --> A Authenticator,decrypted by TGS and used to validate the client

4. TGS->C:
K_SS(SGT)
SK_C/TGS(SK_C/SS) --> Decrypted by client to get SK_C/SS

5. C->SS:
K_SS(SGT) --> Decrypted by SS to get SGT,and get K_C/SS
SK_C/SS(CID,Timestamp) --> A Authenticator,decrypted by SS and used to validate the client

6. SS->C:
SK_C/SS(Timestamp+1) --> Decrypted by client to validte server(Timestamp should be correctely updated)




* * *


**Reference:**
1. Kerberos (protocol),[http://en.wikipedia.org/wiki/Kerberos_(protocol)](http://en.wikipedia.org/wiki/Kerberos_(protocol))
2. Kerberos Basic Authentication Concepts,[http://msdn.microsoft.com/en-us/library/aa374743(v=vs.85).aspx](http://msdn.microsoft.com/en-us/library/aa374743(v=vs.85).aspx)
3. The Kerberos Network Authentication Service (V5),[http://www.ietf.org/rfc/rfc4120.txt](http://www.ietf.org/rfc/rfc4120.txt)
