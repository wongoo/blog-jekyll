---
author: admin
comments: true
date: 2013-01-14 07:25:40+00:00
excerpt: Oracle MD5 Encode
layout: post
slug: oracle-md5-encode
title: Oracle MD5 Encode
wordpress_id: 386
categories:
- Experience
tags:
- encode
- MD5
- oracle
---




    
    CREATE OR REPLACE FUNCTION GET_MD5
        ( p_str in varchar2)
        RETURN varchar2 IS
    BEGIN
        RETURN  utl_raw.cast_to_varchar2(utl_encode.base64_encode(Utl_Raw.Cast_To_Raw(DBMS_OBFUSCATION_TOOLKIT.MD5(input_string => P_Str))));
    END;
    //test
    select GET_MD5('123') FROM DUAL;


Refercence：
1.[http://docs.oracle.com/cd/B19306_01/appdev.102/b14258/d_obtool.htm#i1003449](http://docs.oracle.com/cd/B19306_01/appdev.102/b14258/d_obtool.htm#i1003449)
2.[http://stackoverflow.com/questions/3804279/base64-encoding-and-decoding-in-oracle](http://stackoverflow.com/questions/3804279/base64-encoding-and-decoding-in-oracle)
