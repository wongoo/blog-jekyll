---
author: admin
comments: true
date: 2013-08-04 08:51:06+00:00
excerpt: 'Oracle ROWID '
layout: post
slug: oracle-rowid
title: Oracle ROWID Introduction
wordpress_id: 463
categories:
- Experience
tags:
- oracle
- rowid
---

### **1. ROWID Inroduction**


ROWID pseudocolumn is a base 64 string representing the unique address of a row in its table.  Oracle Database rowid values contain information necessary to locate a row:：



	
  1. The data object number of the object

	
  2. The datafile in which the row resides (first file is 1). The file number is relative to the tablespace

	
  3. The data block in the datafile in which the row resides

	
  4. The position of the row in the data block (first row is 0)


**ROWID base64 format**:OOOOOO FFF BBBBBB RRR



	
  1. OOOOOO is the object ID（32 bits,6 base64 chars , 2^32=4G)

	
  2. FFF is the file number (10 bits, 3 base64 chars, 2^10 =1024, except 0 ,left 1023)

	
  3. BBBBBB is the block number (22 bits, 6 base64 chars , 2^22 =4M)

	
  4. RRR is the row number (16bit, 3 base64 chars , 2^16 = 65536)


**Get all parts of a ROWID:**
select dbms_rowid.rowid_object('AAASOeAAEAAFCL/CcQ') object_id
,dbms_rowid.rowid_relative_fno('AAASOeAAEAAFCL/CcQ')  file_id
,dbms_rowid.rowid_block_number('AAASOeAAEAAFCL/CcQ')  block_id
,dbms_rowid.rowid_row_number('AAASOeAAEAAFCL/CcQ')   num
from dual;

OBJECT_ID    FILE_ID   BLOCK_ID        NUM
---------- ---------- ---------- ----------
74654          4    1319679      10000



<table border="1" >Base64 to Integer table
<tbody >
<tr >

<td >Base64
</td>

<td >A
</td>

<td >B
</td>

<td >...
</td>

<td >Z
</td>

<td >a
</td>

<td >b
</td>

<td >...
</td>

<td >z
</td>

<td >0
</td>

<td >1
</td>

<td >...
</td>

<td >9
</td>

<td >+
</td>

<td >/
</td>
</tr>
<tr >

<td >Integer
</td>

<td >0
</td>

<td >1
</td>

<td >...
</td>

<td >25
</td>

<td >26
</td>

<td >27
</td>

<td >...
</td>

<td >51
</td>

<td >52
</td>

<td >53
</td>

<td >...
</td>

<td >61
</td>

<td >62
</td>

<td >63
</td>
</tr>
</tbody>
</table>







AAASOe = SOe = 18*64*64 + 14*64 + 30 = 74654
AAE = E = 4
AAFCL/ =FCL/ = 1319679
CcQ = 10000

**Get object details:** select owner, object_name from dba_objects where data_object_id = 74654;


### **2. Rowid values have several important uses:**





	
  * They are the fastest way to access a single row.

	
  * They can show you how the rows in a table are stored.

	
  * They are unique identifiers for rows in a table.





You should not use ROWID as the primary key of a table. If you delete and reinsert a row with the Import and Export utilities, for example, then its rowid may change. If you delete a row, then Oracle may reassign its rowid to a new row inserted later.


### **3. Used in where clause:**


Although you can use the ROWID pseudocolumn in the SELECT and WHERE clause of a query, these pseudocolumn values are not actually stored in the database. You cannot insert, update, or delete a value of the ROWID pseudocolumn.
eg: select *  from TABLE_NAME where ROWID<=chartorowid('AAASOeAAEAAFCL/CcQ');


### **4.  Use Oracle HINT to read records order by ROWID:**


A query like "select * from table" will default query by ROWID if there isn't index column. Otherwise, it will query throgh the indexs and return records order by the indexs.
Use Oracle HINT to tell oracle to query by ROWID explicitly:  select /*+rowid(TABLE_NAME)*/ ROWID, t.* from TABLE_NAME t




