---
author: admin
comments: true
date: 2012-02-29 02:20:55+00:00
excerpt: Convert HTML to other type File using Struts Result
layout: post
slug: convert-html-to-other-type-file-using-struts-result
title: Convert HTML to other type File using Struts Result
wordpress_id: 216
categories:
- Experience
tags:
- Conversion
- java
- Struts
---

At the beginning , the naturally considered solution is intercepting the response stream and converting it to the format of the target file type.

The detail steps is:

1. using WapperedResponse to wrap the HTTP Response Object in Servlet Filter ;
2. do filter using wrapperd response object;
3. get the Response bytes from wrapperd response;
4. convert the bytes to the format of target file type;
5. set the response header, like Content-disposition and Content-Type;

When having a look at the StreamResult of struts, I had a idea that converting the stream in struts result. I am sure it's possible after finding three methods as follow in the class DefaultActionInvocation(Xwork 2.2.3) :

DefaultActionInvocation.getResultCode()
DefaultActionInvocation.createResult()
DefaultActionInvocation.setResultCode(String)

The Steps is :

1. Wapper and replace the response in ServletActionContext;
2. create Struts result which will generate the HTML Stream and execute;
3. get the Response bytes from wrapperd response;
4. convert the bytes to the format of target file type;
5. set the response header, like Content-disposition and Content-Type;

Code: [HtmlStreamResult.java](http://sisopipo.com/blog/archives/216/htmlstreamresult)
