---
author: admin
comments: true
date: 2013-09-17 13:54:39+00:00
excerpt: Oracle Partition Common Usage
layout: post
slug: oracle-partition-common-usage
title: Oracle Partition Common Usage
wordpress_id: 501
categories:
- Experience
tags:
- oracle
- partition
- sql
---


    
    --create tablespaces
    create tablespace cus_area_1 datafile 'F:\oracle\oradata\cus_area_1.dbf'  size 5m  autoextend on next  10m maxsize unlimited;
    create tablespace cus_area_2 datafile 'F:\oracle\oradata\cus_area_2.dbf'  size 5m  autoextend on next  10m maxsize unlimited;
    create tablespace cus_area_3 datafile 'F:\oracle\oradata\cus_area_3.dbf'  size 5m  autoextend on next  10m maxsize unlimited;
    
    --create table with partition declaration
    CREATE TABLE CUSTOMER
    (
      CUSTOMER_ID NUMBER NOT NULL PRIMARY KEY,
      CUST_AREA INT NOT NULL,
      FIRST_NAME VARCHAR2(30) NOT NULL,
      LAST_NAME VARCHAR2(30) NOT NULL,
      PHONE VARCHAR2(15) NOT NULL,
      EMAIL VARCHAR2(80),
      SEX VARCHAR2(10),
      STATUS VARCHAR2(10),
      CREATE_DATE DATE
    )
    PARTITION BY LIST (CUST_AREA) --partition declaration
    (
      PARTITION area_1 VALUES(1) TABLESPACE cus_area_1,
      PARTITION area_2 VALUES(2) TABLESPACE cus_area_2,
      PARTITION area_3 VALUES(3) TABLESPACE cus_area_3
    );
    
    -- test data
    insert into CUSTOMER(CUSTOMER_ID,CUST_AREA,FIRST_NAME,LAST_NAME,PHONE,CREATE_DATE) values(1,1,'geln','yang','13779941234',sysdate);
    insert into CUSTOMER(CUSTOMER_ID,CUST_AREA,FIRST_NAME,LAST_NAME,PHONE,CREATE_DATE) values(2,2,'eric','yang','13779941235',sysdate);
    insert into CUSTOMER(CUSTOMER_ID,CUST_AREA,FIRST_NAME,LAST_NAME,PHONE,CREATE_DATE) values(3,3,'tom','yang','13779941236',sysdate);
    insert into CUSTOMER(CUSTOMER_ID,CUST_AREA,FIRST_NAME,LAST_NAME,PHONE,CREATE_DATE) values(4,3,'tony','yang','13779941237',sysdate);
    commit;
    
    -- query by partition
    select CUSTOMER_ID,CUST_AREA,FIRST_NAME,LAST_NAME from CUSTOMER;
    select CUSTOMER_ID,CUST_AREA,FIRST_NAME,LAST_NAME from CUSTOMER partition (area_1) ;
    select /*+rowid(CUSTOMER)*/ ROWID,LAST_NAME,FIRST_NAME,PHONE from CUSTOMER  partition (AREA_1)  where  ROWID>='AAASrfAAIAAAACAAAA' and  ROWID<='AAASrfAAIAAAACHCcQ';
    
    -- find all partitioin data for a table
    SELECT e.owner,e.segment_name,e.partition_name,o.data_object_id,
    	e.RELATIVE_FNO,
    	e.BLOCK_ID MIN_BLOCK,
    	e.BLOCK_ID + e.BLOCKS - 1 MAX_BLOCK
    FROM dba_extents e, dba_objects o
    WHERE     e.segment_name = 'CUSTOMER'  -- table name
    	AND o.object_name = e.segment_name
    	AND e.owner = 'MASK' -- schema name
    	AND o.OWNER = e.owner
    	AND NVL (e.partition_name, 0) = NVL (o.SUBOBJECT_NAME, 0)
    	AND o.data_object_id IS NOT NULL;
    

