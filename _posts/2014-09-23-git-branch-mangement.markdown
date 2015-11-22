---
author: admin
comments: true
date: 2014-09-23 04:09:07+00:00
layout: post
slug: git-branch-mangement
title: Git分支管理策略
wordpress_id: 702
categories:
- Knowledge
- Collection
tags:
- branch
- git
- merge
- VCS
---

文章来源：http://www.ruanyifeng.com/blog/2012/07/git.html

Git版本的分支（branch）和合并（merge）十分方便。有些传统的版本管理软件，分支操作实际上会生成一份现有代码的物理拷贝，而Git只生成一个指向当前版本（又称"快照"）的指针，因此非常快捷易用。但是，太方便了也会产生副作用。如果你不加注意，很可能会留下一个枝节蔓生、四处开放的版本库，到处都是分支，完全看不出主干发展的脉络。
Vincent Driessen提出了一个分支管理的策略，我觉得非常值得借鉴。它可以使得版本库的演进保持简洁，主干清晰，各个分支各司其职、井井有条。理论上，这些策略对所有的版本管理系统都适用，Git只是用来举例而已。如果你不熟悉Git，跳过举例部分就可以了。

**一、主分支Master**
首先，代码库应该有一个、且仅有一个主分支。所有提供给用户使用的正式版本，都在这个主分支上发布。
Git主分支的名字，默认叫做Master。它是自动建立的，版本库初始化以后，默认就是在主分支在进行开发。

**二、开发分支Develop**
主分支只用来分布重大版本，日常开发应该在另一条分支上完成。我们把开发用的分支，叫做Develop。这个分支可以用来生成代码的最新隔夜版本（nightly）。如果想正式对外发布，就在Master分支上，对Develop分支进行"合并"（merge）。

Git创建Develop分支的命令：

    git checkout -b develop master


切换到Master分支

    git checkout master


对Develop分支进行合并到master分支，--no-ff参数是什么意思。默认情况下，Git执行"快进式合并"（fast-farward merge），会直接将Master分支指向Develop分支。

    git merge --no-ff develop


**三、功能（feature）分支：**
      它是为了开发某种特定功能，从Develop分支上面分出来的。开发完成后，要再并入Develop。功能分支的名字，可以采用feature-*的形式命名。
   创建一个功能分支：

    git checkout -b feature-x develop

  开发完成后，将功能分支合并到develop分支：

    git checkout develop
    git merge --no-ff feature-x

删除feature分支：

    git branch -d feature-x


**四、预发布（release）分支**
     指发布正式版本之前（即合并到Master分支之前），我们可能需要有一个预发布的版本进行测试。预发布分支是从Develop分支上面分出来的，预发布结束以后，必须合并进Develop和Master分支。它的命名，可以采用release-*的形式。

创建一个预发布分支：

    git checkout -b release-1.2 develop

确认没有问题后，合并到master分支：

    git checkout master
    git merge --no-ff release-1.2
    # 对合并生成的新节点，做一个标签
    git tag -a 1.2

再合并到develop分支：

    git checkout develop
    git merge --no-ff release-1.2

最后，删除预发布分支：

    git branch -d release-1.2


**五、修补bug（fixbug）分支**
软件正式发布以后，难免会出现bug。这时就需要创建一个分支，进行bug修补。修补bug分支是从Master分支上面分出来的。修补结束以后，再合并进Master和Develop分支。它的命名，可以采用fixbug-*的形式。

创建一个修补bug分支：

    git checkout -b fixbug-0.1 master

修补结束后，合并到master分支：

    git checkout master
    git merge --no-ff fixbug-0.1
    git tag -a 0.1.1

再合并到develop分支：

    git checkout develop
    git merge --no-ff fixbug-0.1

最后，删除"修补bug分支"：

    git branch -d fixbug-0.1


