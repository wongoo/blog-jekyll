---
author: admin
comments: true
date: 2012-05-30 06:09:47+00:00
layout: post
slug: restart-dead-eai-steps-of-tibco-iprocess-sql-server
title: Restart dead EAI steps of Tibco iProcess (SQL Server)
wordpress_id: 275
categories:
- Experience
tags:
- EAI
- iProcess
- Tibco
---

* * *

**Steps：**

1. 停止IPE BG 進程：

%SWDIR%\util\swsvrmgr shutdown 1 BG

  


2. 確定某一筆case（需知道該筆case的casenum）已經被移入deadqueue中：

select last_failed,0,msg_id,msg_hdr,msg_data from swpro.sw_db_deadqueue where msg_data like '%43657%'

3. 將該筆case（需知道該筆case的casenum）的deadqueue消息插入到bg進程隊列中：

insert into swpro.sw_db_bgqueue_1(last_failed,failure_count,msg_id,msg_hdr,msg_data )

select last_failed,0,msg_id,msg_hdr,msg_data from swpro.sw_db_deadqueue where msg_data like '%43657%'

4. 將該筆case消息從deadqueue中移除：

delete from swpro.sw_db_deadqueue where msg_data like '%43657%'

  


5. 啟動IPE BG進程：

%SWDIR%\util\swsvrmgr start 1 BG

* * *

Note1：第1步和第5步可以對應替換為“停止/啟動”整個IPE服務；一般情况可以忽略这两个步骤（Production环境建议保留此步骤）

* * *

Note2：swpro.sw_db_deadqueue 中有一个rowid的递增字段，可以通过这个字段判断一段时间范围内的消息。

* * *

Note3：在sw_warn log文件中可以根据类似以下正则表达式找到有问题的caseNum：

"WARNING: <CPLInstruction \\- Error> <\\-2> <RELEASE> <RELEASE\\^**staffw_nod1**\\^[\\d:\\.]+\\^\\d+\\^\\^**(\\d+)**\\^";

* * *
