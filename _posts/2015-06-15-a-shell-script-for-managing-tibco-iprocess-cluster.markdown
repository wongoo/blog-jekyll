---
author: admin
comments: true
date: 2015-06-15 06:50:05+00:00
layout: post
slug: a-shell-script-for-managing-tibco-iprocess-cluster
title: A Shell Script for managing Tibco iProcess Cluster
wordpress_id: 862
categories:
- Experience
tags:
- cluster
- iProcess
- shell
- Tibco
---


    #!/bin/sh
    # gelnyang@163.com , 2015/6/8
    
    . /etc/profile
    
    export ORACLE_SID=ORCL
    export ORACLE_BASE=/oracle
    export GRID_HOME=/grid/grid_home
    export ORACLE_HOME=$ORACLE_BASE/db11g
    export PATH=$ORACLE_HOME/bin:$GRID_HOME/bin:$PATH
    export LD_LIBRARY_PATH=$ORACLE_HOME/lib:$ORACLE_HOME/rdbms/lib:/lib:/usr/lib:$LD_LIBRARY_PATH
    export CLASSPATH=$ORACLE_HOME/JRE:$ORACLE_HOME/jlib:$ORACLE_HOME/rdbms/jlib:$ORACLE_HOME/network/jlib
    export ORACLE_OWNER=oracle
    export NLS_LANG=AMERICAN_AMERICA.ZHS16GBK
    export LANG=en_US.UTF-8
    
    export SWDIR=/opt/bpm/swnod1
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SWDIR/libs
    export PATH=$PATH:/usr/sso/lib64
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/sso/lib64
    export SW_SYSCLASSPATH=$SWDIR/eaijava/libs/bootstrap/log4j-1.2.13.jar:$SWDIR/eaijava/libs/bootstrap/bootstrap.jar
    
    datenow=$(date '+%Y%m%d')
    USER=pro
    sw_log_path=/tmp/iprocess/swbpm_$datenow.log
    NODE1=IPESERVER1
    NODE2=IPESERVER2
    IPEPORT=1145
    THREADPORTPREFIX=5578
    TIMEWAIT=10
    
    # check current node
    CURR_NODE=$(hostname)
    CURR_NODE=$(echo $CURR_NODE | tr '[a-z]' '[A-Z]')   
    if [ "$CURR_NODE" == $NODE1 ]; then
    	CURR_NODE=$NODE1
    	REMOTE_NODE=$NODE2
    else
    	CURR_NODE=$NODE2
    	REMOTE_NODE=$NODE1
    fi
    
    # create log directory
    mkdir -p /tmp/iprocess/
    
    function wait_ipe_thread_port_shutdown()
    {
    	portlength=$(netstat -anot |grep $THREADPORTPREFIX | tail -1 | wc -L)
    	while (($portlength > 0 ))
    	do
    	   echo "there are ports with prefix "$THREADPORTPREFIX", wait ..." >> $sw_log_path
    	   sleep 1
    	   portlength=$(netstat -anot |grep $THREADPORTPREFIX | tail -1 | wc -L)
    	done
    }
    
    function start_ipe()
    {
    	echo "start iprocess on "$CURR_NODE >> $sw_log_path
    	procmgrflag=$(/bin/ps -ef | grep $SWDIR"/etc/procmgr" | grep -v grep )
    	if [ "$procmgrflag" != "" ]; then
    		/bin/su $USER -c $SWDIR"/bin/swstart" >> $sw_log_path
    	else
    		wait_ipe_thread_port_shutdown
    		/bin/su $USER -c $SWDIR"/bin/swstart -p" >> $sw_log_path
    	fi
    }
    
    function shutdown_ipe()
    {
    	echo "shutdown iprocess on "$CURR_NODE >> $sw_log_path
    	procmgrflag=$(/bin/ps -ef | grep $SWDIR"/etc/procmgr" | grep -v grep )
        if [ "$procmgrflag" != "" ]; then
    		/bin/su $USER -c $SWDIR"/bin/swstop -f 10"
    	fi
    	processflag=$(ps aux | grep $SWDIR | grep -v grep |tail -1)
    	if [ "$processflag" != "" ]; then
    		ps aux | grep $SWDIR | grep -v grep | cut -c 9-15 | xargs kill -s 9 >> $sw_log_path
    	fi
    	if [ "$procmgrflag" != "" ]; then
    		/bin/su $USER -c $SWDIR"/bin/swstop -p"
    	fi
    }
    
    function set_master_slave()
    {
    	echo "set master to "$1",set slave to "$2 >> $sw_log_path
    	/bin/su $USER -c $SWDIR"/util/swadm move_server 1 dummy" >> $sw_log_path
    	/bin/su $USER -c $SWDIR"/util/swadm move_server 2 "$2 >> $sw_log_path
    	/bin/su $USER -c $SWDIR"/util/swadm move_server 1 "$1 >> $sw_log_path
    }
    
    function is_current_master()
    {
    	echo "check whether current node is the master node" >> $sw_log_path
    	master_flag=$(/bin/su $USER -c $SWDIR"/util/swadm find_master |tail -1 |grep "$CURR_NODE)
    	if [ "$master_flag" != "" ]; then
    		echo "current $CURR_NODE is master node" >> $sw_log_path
    		echo "Y"
    	else
    		echo "current $CURR_NODE is slave node" >> $sw_log_path
    		echo "N"
    	fi
    }
    
    function get_remote_status()
    {
    	echo $(nc $REMOTE_NODE $IPEPORT < /dev/null; echo $?)
    }
    
    
    # add log header
    echo "----------------------------" >> $sw_log_path
    echo  $(date '+%Y%m%d%H%M%S')  >> $sw_log_path
    echo "$0" "$1" >> $sw_log_path
    echo "----------------------------" >> $sw_log_path
    
    case "$1" in
        start)
            echo "Starting iProcess Engine Server" >> $sw_log_path
    		
    		portflag=$(netstat -anot | grep $IPEPORT | grep -v grep |tail -1)
    		echo $portflag  >> $sw_log_path
    		if [ "$portflag" == "" ]; then
    			echo "no port "$IPEPORT" running on current node, calling shutdown first ...!" >> $sw_log_path
    			shutdown_ipe
    		fi
    		
    		master_flag=$(is_current_master)
    		if [ "$master_flag" == "Y" ]; then
    			echo "current is master, move server" >> $sw_log_path
    			set_master_slave $CURR_NODE $REMOTE_NODE
            else
    			echo "current is slave node" >> $sw_log_path
                remotealive=$(get_remote_status)
    			if [ "$remotealive" != "0" ]; then
    				echo "master can't connect, wait to check again later .." >> $sw_log_path
    				sleep $TIMEWAIT
    				remotealive=$(get_remote_status)
    				if [ "$remotealive" != "0" ]; then
    					echo "master is dead, make current node as master " >> $sw_log_path
    					set_master_slave $CURR_NODE $REMOTE_NODE
    				fi
    			fi
            fi
    		
    		start_ipe
            ;;
        stop)
            echo "Shutting down iProcess Engine Server" >> $sw_log_path
    		master_flag=$(is_current_master)
    		if [ "$master_flag" == "Y" ]; then
    			echo "current is master, switch it to slave" >> $sw_log_path
    			shutdown_ipe
    			set_master_slave $REMOTE_NODE $CURR_NODE
    		
    			remotealive=$(get_remote_status)
    			if [ "$remotealive" != "0" ]; then
    				echo "master can't connect, wait to check again later .." >> $sw_log_path
    				sleep $TIMEWAIT
    				remotealive=$(get_remote_status)
    				if [ "$remotealive" != "0" ]; then
    					echo $REMOTE_NODE" is master,but not started,can't start slave on "$CURR_NODE >> $sw_log_path
    				else
    					echo $REMOTE_NODE" is master,and already started,start slave on "$CURR_NODE >> $sw_log_path
    					start_ipe
    				fi
    			fi
    		else
    			remotealive=$(get_remote_status)
    			if [ "$remotealive" != "0" ]; then
    				echo "master node is dead on "$REMOTE_NODE >> $sw_log_path
    				shutdown_ipe
    				
    			else
    				echo "master node is live, do nothing for slave stopping" >> $sw_log_path
    			fi
    		fi
            
            ;;
        status)
            echo "Checking status for iProcess Engine Server: " >> $sw_log_path
    		procmgrflag=$(/bin/ps -ef | grep $SWDIR"/etc/procmgr" | grep -v grep )
            if [ "$procmgrflag" == "" ]; then
                echo "no process sentinel thread ..." >> $sw_log_path
    			exit 1
            fi
    		
    		#return OK only for master node
    		portflag=$(netstat -anot | grep $IPEPORT | grep -v grep |tail -1)
    		echo $portflag  >> $sw_log_path
    		if [ "$portflag" == "" ]; then
    			exit 1
    		else
    			exit 0
    		fi
            ;;
        *)
            echo "Usage: $0 {start|stop|status}" >> $sw_log_path
            exit 1
            ;;
    esac
    

