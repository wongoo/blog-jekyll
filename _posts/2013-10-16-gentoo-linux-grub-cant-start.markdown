---
author: admin
comments: true
date: 2013-10-16 11:07:09+00:00
excerpt: Gentoo Linux Grub 無法啓動問題
layout: post
slug: gentoo-linux-grub-cant-start
title: Gentoo:Grub 無法啓動的問題
wordpress_id: 528
categories:
- Experience
tags:
- gentoo
- grub
- grub2
- linux
---

常遇到Grub 無法啓動的問題，大多數是因爲對GRUB不了解，配置錯誤造成的，在此整理一下。Gentoo執行emerge grub默認只會安裝最新的grub2版本，不會安裝grub1的版本。往往沒有安裝grub1，系統無法正常引導。要讓gurb1和grub2版本共存，可以執行以下兩個命令完成安裝：

    
    emerge --noreplace sys-boot/grub:0
    emerge --ask sys-boot/grub:2
    


**grub1和grub2的啓動方式有些差異：**

1）配置文件：
 grub1使用配置文件/boot/grub/grub.conf,同目錄下建立menu.lst文件連接到grub.conf文件（ln -snf grub.conf menu.lst）；
 grub2使用配置文件/boot/grub/grub.cfg,此文件可以通過執行（grub2-mkconfig -o /boot/grub/grub.cfg）命令自動生成。

2）安裝命令

     
    grub1：grub-install /dev/sda
    grub2：grub2-install /dev/sda
    


根據/etc/fstab配置文件，linux啓動后，啓動分區/dev/sda1會自動挂載到/boot，如果啓動kernel核心位于sda1的根目錄下，則配置grub.conf或grub.cfg的時候需要注意，指定kernel位置是直接根目錄/kernel，而不是/boot/kernel.

**參考：**
1.GRUB2 Quick Start，[http://wiki.gentoo.org/wiki/GRUB2_Quick_Start](http://wiki.gentoo.org/wiki/GRUB2_Quick_Start)
2.Grub错误汇总，[http://www.gentoo.org/doc/zh_cn/grub-error-guide.xml](http://www.gentoo.org/doc/zh_cn/grub-error-guide.xml)
3. Installing GRUB,[http://www.gentoo.org/doc/en/handbook/handbook-amd64.xml?full=1#book_part1_chap10](http://www.gentoo.org/doc/en/handbook/handbook-amd64.xml?full=1#book_part1_chap10)
