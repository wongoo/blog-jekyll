---
author: admin
comments: true
date: 2012-09-18 05:50:54+00:00
excerpt: Linux Top命令 选择显示列及列排序
layout: post
slug: linux-top-and-sort
title: Linux Top命令 选择显示列及列排序
wordpress_id: 323
categories:
- Experience
tags:
- linux
- top
---

Top用于查看Linux系统下进程信息，有时候需要选择显示那些列，以及按照某一列进行排序。查询整理如下：

选择显示列：
执行top命令后，按 f 键，再按某一列的代表字母，即可选中或取消显示；

列显示位置调整：
执行top命令后，按 o 键，选择要调整位置的列（如K:CUP Usageage），按动一下大写K则显示位置往上调整，按动一下小写K则显示位置往下调整。

列排序：
执行top命令后，按 shift + f（小写），进入选择排序列页面，再按要排序的列的代表字母即可；
