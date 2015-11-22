---
author: admin
comments: true
date: 2013-01-20 00:59:11+00:00
excerpt: VMWare安装Gentoo Linux记录
layout: post
slug: vmware-install-gentoo
title: VMware安装Gentoo记录
wordpress_id: 391
categories:
- Experience
tags:
- gentoo
- linux
- vmware
---

**根据官方安装文档(http://www.gentoo.org/doc/en/handbook/handbook-amd64.xml),整理安装过程执行的脚本命令如下。  

安装环境为vmware9，VM设置内存为2G，CPU设置为1个双核，设置光盘从Gentoo Livecd ISO文件启动。
**



echo ==================启动LiveCD
gentoo-nofb

echo ==================设置网路
net-setup eth0
ifconfig eth0 192.168.7.123/24
route add default gw 192.168.7.1
echo nameserver 192.168.7.1 > /etc/resolv.conf

echo ==================启动SSH并修改密码
time /etc/init.d/sshd start
passwd

echo ==================下载stage3
fdisk /dev/sda
/dev/sda1 primary（32M）
/dev/sda2 primary（512M）
/dev/sda3 Extend
/dev/sda5 Logic (逻辑分区1)


   Device Boot      Start         End      Blocks   Id  System
/dev/sda1   *        2048    10487807     5242880   83  Linux
/dev/sda2        10487808    20973567     5242880   82  Linux swap / Solaris
/dev/sda3        20973568    41943039    10484736    5  Extended
/dev/sda5        20975616    31461375     5242880   83  Linux




mke2fs /dev/sda1
mke2fs -j /dev/sda3
mkswap /dev/sda2 && swapon /dev/sda2（创建并激活交换分区）

echo ==================下载stage3
mount /dev/sda5 /mnt/gentoo
mkdir /mnt/gentoo/boot
mount /dev/sda1 /mnt/gentoo/boot

echo ==================下载stage3
cd /mnt/gentoo
wget http://mirrors.xmu.edu.cn/gentoo/releases/amd64/current-stage3/stage3-amd64-20121210.tar.bz2
time tar xjpf stage3*

echo ==================下载portage
cd /mnt/gentoo/usr
wget http://mirrors.xmu.edu.cn/gentoo/releases/snapshots/current/portage-latest.tar.bz2
time tar xjf portage-lat*

echo ==================建立默认下载站点
mirrorselect -i -o >> /mnt/gentoo/etc/make.conf
mirrorselect -i -r -o >> /mnt/gentoo/etc/make.conf

echo ==================切换系统
echo 挂载/proc和/dev文件系统
echo 将/proc文件系统挂载到/mnt/gentoo/proc，这样chroot后的环境里安装时也可以获取内核提供的相关信息，然后以bind方式挂载/dev文件系统。
cd /
mount -t proc none /mnt/gentoo/proc
mount -o bind /dev /mnt/gentoo/dev
cp -L /etc/resolv.conf /mnt/gentoo/etc/
chroot /mnt/gentoo /bin/bash
env-update && source /etc/profile
>>> Regenerating /etc/ld.so.cache...

export PS1="(chroot) $PS1"

echo ==================更新Portage树
emerge --sync

echo ==================切换Profile，我只用gnome
eselect profile list
eselect profile set 4

echo ==================设定时区
ls /usr/share/zoneinfo
cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime （以上海为例）

echo ==================设定主机名和域名
cd /etc
echo "127.0.0.1 center.home center localhost" > hosts
sed -i -e 's/HOSTNAME.*/HOSTNAME="center"/' conf.d/hostname
hostname center （使用指定的主机名并检查）
hostname -f

echo ==================下载内核
cd /usr/portage/distfiles/
wget http://mirrors.xmu.edu.cn/gentoo/distfiles/linux-3.6.tar.bz2
wget http://mirrors.xmu.edu.cn/gentoo/distfiles/genpatches-3.6-11.base.tar.bz2
wget http://mirrors.xmu.edu.cn/gentoo/distfiles/genpatches-3.6-11.extras.tar.bz2

echo ==================配置内核,安装一个内核源码包（通常为gentoo-sources），配置、编译并拷贝arch/x86_64/boot/bzImage文件到/boot。
time emerge gentoo-sources
cd /usr/src/linux
make menuconfig（配置内核）
#------------------------------
VMware需要修改配置（参考来源:http://chengchow.blog.51cto.com/1642666/925657）
-->Device Drivers-->SCSI device support--><*>SCSI disk support
-->Device Drivers-->SCSI device support-->SCSI low-level drivers-->
<*>BusLogic SCI support-->FlashPoint support
-->Device Drivers-->Fusion MPT device support-->所有的
-->Device Drivers-->USB support--><*>USB Mass Storage support
-->Device Drivers-->Networking support-->Ethernet driver support(NEW)-->
<*>AMD PCnet32 PCI support

-->Processor type and features-->Processor family-->Pentium M
-->File System下选择(根据你自己的需要取舍)
EXT2文件系统支持（Second extended fs support）
EXT3文件系统支持
JFS文件系统支持
ReiserFS文件系统支持
XFS文件系统支持
备注：内核系统编译主要是靠硬件知识支持，具体安装可以通过 lspci 或者 lspci | less来查看自己系统的硬件信息。
#------------------------------
time make -j2（所花的时间很大程度上决定于你所选的选项）
make modules_install
cp arch/x86_64/boot/bzImage /boot/kernel

echo ==================配置系统,编辑/etc/fstab，用实际的分区名代替BOOT、ROOT和SWAP
cd /etc
nano -w fstab

echo ==================设置网卡
cd /etc/conf.d
echo 'config_eth0=( "192.168.7.123/24" )' >> net
echo 'routes_eth0=( "default via 192.168.7.1" )' >> net
rc-update add net.eth0 default
#--------------如果以上这句执行失败，先请执行下面两句
cd /etc/init.d
ln -s net.lo net.eth0
#---------------
emerge dhcpcd

echo ==================设置SSHD为默认启动级别，以便重启后通过ssh重新连接到新机器
rc-update add sshd default

echo ==================设置root密码
passwd

echo ==================设置时区
nano -w /etc/conf.d/clock
TIMEZONE="Asia/Shanghai"

echo ==================检查系统配置
nano -w /etc/rc.conf
nano -w /etc/conf.d/rc
nano -w /etc/conf.d/keymaps

echo ==================安装系统工具，一个系统日志如syslog-ng和一个cron守护进程如vixie-cron，并把它们添加到默认运行级别。
time emerge syslog-ng vixie-cron
rc-update add syslog-ng default
rc-update add vixie-cron default

echo ==================配置引导程序
time emerge grub
nano -w /boot/grub/grub.conf
#-------------------
default 0
timeout 10

title Gentoo
root (hd0,0)
kernel /boot/kernel root=/dev/sda5

grub
grub>root (hd0,0)
grub>setup (hd0)
grub>quit

echo ==================重启
exit
umount /mnt/gentoo/dev /mnt/gentoo/proc /mnt/gentoo/boot /mnt/gentoo

reboot

echo ==================重新登录，清理文件
rm /portage-latest.tar.bz2*
rm /stage3-*.tar.bz2*

echo ==================最后涉及的配置，在/etc/make.conf中定义SYNC变量和GENTOO_MIRRORS变量，或者也可以使用mirrorselect
emerge mirrorselect
mirrorselect -i -o >> /etc/make.conf
mirrorselect -i -r -o >> /etc/make.conf
echo 'MAKEOPTS="-j3"' >> /etc/make.conf （通常，一个较好的值为处理器数加一,以便不会浪费任何一个cpu）

echo ==================Portage列出软件包和它们的USE标记。作为例子，让我们禁用ipv6和fortran，并启用unicode
emerge -vpe world
echo 'USE="nptl nptlonly -ipv6 -fortran unicode"' >> /etc/make.conf

echo ==================定义语言环境
cd /etc
nano -w locale.gen
locale-gen

echo ==================为make.conf作最后的修改
nano -w make.conf
CFLAGS="-O2 -march=x86_64 -pipe"（在CFLAGS中将-march设为你的CPU类型）
FEATURES="ccache"（添加下面一行）
ACCEPT_KEYWORDS="~x86" （只有清楚自己在干什么才添加这一行）
#echo 参考http://www.gentoo.org/doc/zh_cn/gcc-upgrading.xml

echo ==================升级软件包
emerge ccache
emerge -vpuD --newuse world （好好看一看软件包列表和它们的USE标记，有阻止的包就先卸载，然后启动漫长的过程）
time emerge -vuD --newuse world（（重）编译了79个软件包）
emerge --oneshot libtool（重编译libtool以避免一些潜在的问题）
dispatch-conf（更新配置文件，确保不要让dispatch-conf更新你配置好的文件）
time perl-cleaner all（如果升级了perl，应该执行perl-cleaner脚本）
python-updater（如果升级了python的主要软件包，应该执行python-updater脚本）

echo ==================添加人员
adduser -g users -G lp,wheel,audio,cdrom,portage,cron -m geln
passwd geln

