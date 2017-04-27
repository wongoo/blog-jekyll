---
layout: post
author: gelnyang
comments: true
date: 2017-04-27
title: how to use xcode to develop c project
categories:
- experience
tags:
- xcode
- c
- ide
---
---

This is a simple guide to use xcode to develop c project.

# 1. config c project

## 1.1 create project

File > New > Project, to create a "command line project"

Right click project > "Add files to .." , to add exists files into project.

## 1.2. header search paths:
project > Build Settings > Search Paths > Header Search Paths (debug/release), like:

```
/usr/local/include /usr/local/openssl/include
```

## 1.3. Library search paths:
project > Build Settings > Search Paths > Library Search Paths (debug/release), like:

```
/usr/local/lib /usr/local/openssl/lib
```

## 1.4. Other C Flags:
project > Build Settings > Other C Flags (debug/release), like:

```
-g -Wall
```


## 1.5. Other Linker Flags:
project > Build Settings > Linking >Other Linker Flags (debug/release), like:

```
 -lssl -lcrypto -lhiredis -levent
```

## 1.6 Build Path

default is build path: /Users/USERNAME/Library/Developer/Xcode/DerivedData/PROJECTNAME/Build/Products/Debug

change build path: project > Build Settings > Build Locations > Build Products Path

Build release binary: Produt > Build For > Profiling

# 2. Build/Compile

short cut: command + B, or Product > build

# 3. Run/Debug

## 3.1 add breakpoint

edit the c code , close line to add the breakpoint

## 3.2 run

shortcut: command + R , or Product > Run

# 4.Code Format

shortcut: ctrl + i , or Editor > Structure > Re-Indent


{{ page.date | date_to_string }},{{ page.author }}

