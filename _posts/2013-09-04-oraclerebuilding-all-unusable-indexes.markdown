---
author: admin
comments: true
date: 2013-09-04 06:24:27+00:00
excerpt: Oracle:Rebuild all unusable indexes
layout: post
slug: oraclerebuilding-all-unusable-indexes
title: Oracle:Rebuild all unusable indexes
wordpress_id: 493
categories:
- Experience
tags:
- index
- oracle
- sql
---

1. unusable a index:

    alter index GROUPS_PK unusable;


2. find all unusable indexs:

    select * from  user_indexes where status='UNUSABLE';


3. rebuild a unusable index:

    alter index table_owner.index_name rebuild online nologging;


4. rebuild all unusable indexs:

    
    declare 
       cursor cur is select 'alter index '||table_owner || '.' ||index_name ||' rebuild online nologging' as sql_string from user_indexes where status='UNUSABLE';
    begin 
      for item in cur loop
        EXECUTE IMMEDIATE item.sql_string;
      end loop;
    end;
    

