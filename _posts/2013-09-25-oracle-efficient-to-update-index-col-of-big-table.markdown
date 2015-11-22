---
author: admin
comments: true
date: 2013-09-25 07:18:17+00:00
excerpt: Oracle:大批量高效地更新大数据表索引字段
layout: post
slug: oracle-efficient-to-update-index-col-of-big-table
title: Oracle大批量高效地更新大数据表索引字段
wordpress_id: 512
categories:
- Experience
tags:
- index
- oracle
- sql
---

更新索引字段会触发索引重建，如果是要大批量修改大数据表的索引字段，速度会很慢，时间大多花在重建索引上了。
高效的办法是将索引先unusable，待批次更新修改后再重建（rebuild）索引。
整理語法如下,需要注意：
1. 通过索引判断一个table是否大数据表（大于50M）。
2. 对于分区表的索引，需用对不同分区进行重建。


    
    declare
        CURSOR c1 IS
    	SELECT index_name,PARTITIONED
    	FROM user_indexes
    	WHERE index_name in (
    			SELECT DISTINCT INDEX_NAME
    			FROM user_ind_columns
    			WHERE table_name = 'TABLE_NAME' --表名
    				AND column_name = 'COLUMN_NAME' --更新字段
    				AND table_name IN (
    					SELECT segment_name
    					FROM (
    						SELECT segment_name
    							,sum(siz_M) AS siz_M
    						FROM (
    							SELECT segment_name
    								,segment_type
    								,bytes / 1024 / 1024 AS siz_M
    							FROM user_segments
    							WHERE segment_type LIKE 'TABLE%'
    								AND segment_name = 'TABLE_NAME' --表名
    							)
    						GROUP BY segment_name
    						)
    					WHERE siz_M >= 50  --定义达到50M的表为大数据表
    					)
    			);
    	
    	TYPE IDXINFOREC 
    	IS TABLE OF c1%ROWTYPE INDEX BY BINARY_INTEGER; 
    
    	idxinfo    c1%ROWTYPE; 
    	idxinfolist IDXINFOREC; 
    	counter INTEGER; 
        
    BEGIN
    	counter := 0; 
    
    	OPEN c1;
    	
    	LOOP 
            FETCH c1 INTO idxinfo; 
    
            IF c1%FOUND THEN 
              counter := counter + 1; 
            END IF; 
    
            idxinfolist(counter) := idxinfo; 
    
            IF c1%NOTFOUND THEN 
              EXIT; 
            END IF; 
        END LOOP; 
    
        CLOSE c1; 
    	
    	--1.unusable索引
    	FOR i IN 1..counter LOOP 
    		execute immediate 'ALTER INDEX ' || idxinfolist(i).index_name || ' unusable';
    	END LOOP;
    		
    	--2.中间执行更新任务
    	-- update something
    	COMMIT;
    	
    	--3.重建索引
    	FOR i IN 1..counter LOOP 
    		IF idxinfolist(i).PARTITIONED = 'NO' then
    			execute immediate 'ALTER INDEX ' || idxinfolist(i).index_name || ' rebuild parallel nologging';
    			
    		ELSE 
    			IF idxinfolist(i).PARTITIONED = 'YES' then
    				FOR x IN (
    							SELECT PARTITION_NAME
    							FROM user_IND_PARTITIONS
    							WHERE index_name = idxinfolist(i).index_name
    							ORDER BY PARTITION_POSITION
    							) LOOP
    
    						execute immediate 'ALTER INDEX ' || idxinfolist(i).index_name || ' rebuild PARTITION ' || x.PARTITION_NAME || ' parallel nologging';
    				END LOOP;
    			END IF;
    		END IF;
    		execute immediate 'ALTER INDEX ' || idxinfolist(i).index_name || ' noparallel';
    	END LOOP;
    END;
    

