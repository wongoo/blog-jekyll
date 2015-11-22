---
author: admin
comments: true
date: 2014-12-03 10:20:29+00:00
excerpt: 日志归档shell脚本
layout: post
slug: shell-for-log-achive
title: 日志归档bash脚本
wordpress_id: 764
categories:
- Experience
tags:
- archive
- log
- shell
---


    #!/bin/sh
    
    # gelnyang@163.com , 2014/12/03
    #
    #=================================================================
    # myapp系统会产生多个log文件备份,但并无日志归档的功能,超过指定个数的log文件会被刪除.
    # 但系统要求日志需长期保存,故写此脚步循环检查log文件目录,将备份目录下格式为*.log.?的日志拷贝到备份目录并压缩归档.
    #
    # configure system schedule
    #   1) copy myapp_logbackup.sh to /etc/init.d/
    #   2) chmod +x /etc/init.d/myapp_logbackup.sh 
    #   3) vi /etc/crontab:
    #        ------------------
    #        */10 * * * * root /etc/init.d/myapp_logbackup.sh >> /tmp/myapp_log_backup.log
    #=================================================================
    
    # configuration format:
    # [LOG_DIR]:[BACKUP_DIR],[LOG_DIR]:[BACKUP_DIR]
    BACKUP_DIR_CONFIG=/opt/myapp/logs1:/opt/logs_backup/log1,/opt/myapp/logs2:/opt/logs_backup/log2
    
    function backup_log_files(){	
    	echo ---------------------------------------
    	echo $(date +%Y/%m/%d' '%H:%M:%S)
    	
    	today=$(date +%Y%m%d)
    	timenow=$(date +%Y%m%d%H%M%S)
    	log_dir=$1
        backup_dir=$2	
    	today_backup_dir=$backup_dir/$today
    	
    	mkdir -p $today_backup_dir
    	
    	echo check directory:$log_dir
    	for file_path in ` ls $log_dir/*.log.? `
    	do
    		file_name=${file_path##/*/}
    		target_file_path=$today_backup_dir/${file_name/.log./.}.$timenow.log
    		echo mv $file_path $target_file_path
    		mv $file_path $target_file_path
    	done
    	
    	echo try to zip log files under directory:$today_backup_dir 
    	cd $today_backup_dir
    	for file_path in ` ls *.log `
    	do
    		file_name=${file_path##/*/} 
    		echo zip $file_path.zip $file_name
    		zip $file_path.zip $file_name
    	done
    	
    	echo rm -rf $today_backup_dir/*.log
    	rm -rf $today_backup_dir/*.log
    }
    
    config_arr=(${BACKUP_DIR_CONFIG//,/ })  
    for config_item in ${config_arr[@]}  
    do  
        item_arr=(${config_item//:/ })
    	backup_log_files ${item_arr[0]} ${item_arr[1]} 
    done  


