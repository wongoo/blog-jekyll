---
author: admin
comments: true
date: 2012-11-11 09:28:16+00:00
excerpt: '1. 多表join的时候, 勿使用多表之间OR条件。遇到这样的情况，建立的索引无法起作用。应尽量改善语句使用到索引以提高查询性能。

  2. 如果有跨表OR条件的查询需求，说明表设计的并不很好，在表设计上应考虑具体需求，尽量避免这样的情况发生。'
layout: post
slug: not-use-or-for-cross-table-join
title: 提升查询性能——勿使用跨表OR条件
wordpress_id: 343
categories:
- Experience
tags:
- explain-plan
- oracle
- performance
---

有两个表，tparent（父表），tchild（子女表），可以用下面的#1脚本创建，taparent的主键是。现在要查询tparent和tchild中uname=’sky’ 的信息，同时查出父表和子女表的资料，查询语句如#2. 当资料量比较小的时候查询速度很快。执行脚本#3，插入大量测试数据，tparent数据量在500万以上，tchlid数据在1000万以上。#4再执行dbms_stats.gather_table_stats收集表统计信息以改善执行计划。这时候再执行#2查询，查询所需时间大大增加。

**# 1 initial table schema**

    drop table tparent;
    drop table tchild;
    create table tparent(typeid int,sno number,uname varchar2(50) );
    create table tchild(typeid int,sno number,seqno int,uname varchar2(50));
    create unique index uidx_tparent_typeidsno on tparent(typeid,sno);
    create unique index uidx_tchild_typeidsnoseqno on tchild(typeid,sno,seqno);
    create index idx_tparent_name on tparent(uname);
    create index idx_tchild_name on tchild(uname);
    insert into tparent values (1,10,'lily');
    insert into tparent values (1,11,'eric');
    insert into tparent values (2,10,'tom');
    insert into tparent values (2,11,'eric');
    insert into tparent values (3,10,'sky');
    insert into tchild values (1,10,1,'sam');
    insert into tchild values (1,10,2,'sky');
    insert into tchild values (1,11,1,'eric');
    insert into tchild values (1,11,2,'john');
    insert into tchild values (2,10,1,'sky');
    insert into tchild values (2,11,1,'eric');
    insert into tchild values (3,10,1,'tony');


**# 2 query**

    select p.typeid,p.sno,p.uname,c1.uname as uname1,c2.uname as uname2 from tparent p
    left join tchild c1 on c1.typeid=p.typeid and c1.sno = p.sno and c1.seqno=1
    left join tchild c2 on c2.typeid=p.typeid and c2.sno = p.sno and c2.seqno=2
    where (p.uname='sky' or c1.uname='sky' or c2.uname='sky');


**# 3 inserting a big size of test data**

    BEGIN
    	FOR x IN 1..10
    	LOOP
    		BEGIN
    			FOR y IN 10000..100000
    			LOOP
    				BEGIN
    					insert into tparent values (x,y,'name'|| y);
    					insert into tchild values (x,y,1,'name'|| y);
    					insert into tchild values (x,y,2,'name'|| y);
    				END;
    			END LOOP;
    			COMMIT;
    		END;
    	END LOOP;
    END;


**#4 exec gather_table_stats to optimize explain plan**

    exec dbms_stats.gather_table_stats('rtmartvts','tparent',degree => 4,estimate_percent => null,method_opt => 'for all indexed columns',cascade => TRUE);
    exec dbms_stats.gather_table_stats('rtmartvts','tchild',degree => 4,estimate_percent => null,method_opt => 'for all indexed columns',cascade => TRUE);


首先查看#2的Explain Plan（如下图），计划COST显示超过一分钟，性能主要消耗在三次全表扫描和两次OUTER Hash Join上，完全没用使用到索引。仔细分析一下这个查询语句，因为where条件中包含的跨表字段的OR条件，不能根据一个表的字段值决定一条记录是否满足条件，必须分别查询出各自符合的记录，在使用outer join的方式结合起来才能得到满足条件的结果。

[![](http://sisopipo.com/blog/media/files/2012/11/1.jpg)](http://sisopipo.com/blog/archives/343/attachment/1)

如果没有跨表的Or条件，会不会有提高呢？去掉tchild中的两个条件，再查看Explain plan，性能大大提升，在300毫秒左右。没有全表扫描和hash关联。使用两层嵌套循环，第一层循环先通过IDX_TPARENT_NAME索引找到指定记录，在通过索引找到TCHILD(SEQNO=2)关联；第二次嵌套循环再通过索引关联查询TCHILD(SEQNO=1)。
分别单独使用某一个table的字段作为查询条件，Explain plan显示查询的时候都使用到索引，性能也自然变好。

**#5 querying only using one field condition**

    select p.typeid,p.sno,p.uname,c1.uname as uname1,c2.uname as uname2 from tparent p
    	left join tchild c1 on c1.typeid=p.typeid and c1.sno = p.sno and c1.seqno=1
    	left join tchild c2 on c2.typeid=p.typeid and c2.sno = p.sno and c2.seqno=2
    where (p.uname='sky' )


[![](http://sisopipo.com/blog/media/files/2012/11/2.jpg)](http://sisopipo.com/blog/archives/343/attachment/2)

现在看来，分三次去查询应该比一次查询的性能要好很多。语句修改如下，分三次查询，将结果union起来。查考其Explain结果，3此查询分别300毫秒左右，union结果也在1秒以内，相比最初语句的查询性能提升了60倍。

**#6 querying statement after optimizing**

    	select p.typeid,p.sno,p.uname,c1.uname as uname1,c2.uname as uname2 from tparent p
    		left join tchild c1 on c1.typeid=p.typeid and c1.sno = p.sno and c1.seqno=1
    		left join tchild c2 on c2.typeid=p.typeid and c2.sno = p.sno and c2.seqno=2
    	where (p.uname='sky' )
    union
    	select p.typeid,p.sno,p.uname,c1.uname as uname1,c2.uname as uname2 from tparent p
    		left join tchild c1 on c1.typeid=p.typeid and c1.sno = p.sno and c1.seqno=1
    		left join tchild c2 on c2.typeid=p.typeid and c2.sno = p.sno and c2.seqno=2
    	where ( c1.uname='sky' )
    union
    	select p.typeid,p.sno,p.uname,c1.uname as uname1,c2.uname as uname2 from tparent p
    		left join tchild c1 on c1.typeid=p.typeid and c1.sno = p.sno and c1.seqno=1
    		left join tchild c2 on c2.typeid=p.typeid and c2.sno = p.sno and c2.seqno=2
    	where ( c2.uname='sky')


[![](http://sisopipo.com/blog/media/files/2012/11/3.jpg)](http://sisopipo.com/blog/archives/343/attachment/3)

**总结：**
1. 多表join的时候, 勿使用跨表OR条件。遇到这样的情况，建立的索引无法起作用。应尽量改善语句使用到索引以提高查询性能。
2. 上面的例子是参考客户项目中已有的数据表建立的，如果有跨表OR条件的查询需求，说明表设计的并不很好，在表设计上应考虑具体需求，尽量避免这样的情况发生。

**参考：**
1. Oracle Explain Plan: [http://docs.oracle.com/cd/E11882_01/server.112/e10821/ex_plan.htm](http://docs.oracle.com/cd/E11882_01/server.112/e10821/ex_plan.htm)
2. Understanding the results of Execute Explain Plan in Oracle SQL Developer: [http://stackoverflow.com/questions/860450/understanding-the-results-of-execute-explain-plan-in-oracle-sql-developer?rq=1](http://stackoverflow.com/questions/860450/understanding-the-results-of-execute-explain-plan-in-oracle-sql-developer?rq=1)
3. Access and Filter predicates in Oracle execution plan: [http://stackoverflow.com/questions/1464469/access-and-filter-predicates-in-oracle-execution-plan](http://stackoverflow.com/questions/1464469/access-and-filter-predicates-in-oracle-execution-plan)

    "Access" predicates are those that are used to scan the index - these predicates are used to select which branch and leaf blocks to retrieve from the index, and they are more important to the performance of the query. 
    "Filter" predicates are those that are applied to the rows returned from the index; they determine which rows are ultimately sent up to the next step in the query execution plan.


4. GATHER_TABLE_STATS Procedure: [http://docs.oracle.com/cd/B19306_01/appdev.102/b14258/d_stats.htm#i1036461](http://docs.oracle.com/cd/B19306_01/appdev.102/b14258/d_stats.htm#i1036461)
5. Hash join: [http://en.wikipedia.org/wiki/Hash_join](http://en.wikipedia.org/wiki/Hash_join)
6. Nested loop join: [http://en.wikipedia.org/wiki/Nested_loop_join](http://en.wikipedia.org/wiki/Nested_loop_join)
7. Sort-merge join: [http://en.wikipedia.org/wiki/Sort-merge_join](http://en.wikipedia.org/wiki/Sort-merge_join)
