---
author: admin
comments: true
date: 2013-09-17 15:35:47+00:00
excerpt: Add partition to an existing NON-PARTITIONED table
layout: post
slug: add-partition-to-an-existing-non-partitioned-table
title: Add partition to an existing NON-PARTITIONED table
wordpress_id: 505
categories:
- Experience
tags:
- oracle
- partition
---

If you have a current NON-PARTITIONED table, you need to:

1. create a PARTITIONED table
2. load the NON-PARTITIONED data into the PARTITIONED table
3. drop the NON-PARTITIONED table
4. rename PARTITIONED table to whatever is appropriate

or:
1. export table and data
2. drop table
3. create new partitioned table the way you want it
4. import table data into new partitioned table (this should also create all the necessary indexes and constraints from your original table)
