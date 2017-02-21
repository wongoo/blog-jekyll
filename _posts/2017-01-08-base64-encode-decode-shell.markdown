---
layout: post
author: gelnyang
comments: true
date: 2017-01-08
title: Base64 encode decode shell script
categories:
- 经验
tags:
- base64
- encode
- shell
---
---

# Base64 encode decode shell script
经常在shell下需要对字符进行base64编码解码，准备两个shell脚本就会方便很多。
目标是写2个脚本可以将要编码和解码的字符串作为脚本参数传递给脚本，脚本输出结果到控制台，并自动将结果添加到粘贴板中供其他程序使用。

## 1. BASE64编码脚本：base64encode.sh

```bash
#!/bin/bash
s=$(echo -n $1 | openssl base64 | tr -d '\n')
echo $s
echo $s | pbcopy
```

## 2. BASE64解码脚本: base64decode.sh

```bash
#!/bin/bash
s=$(echo "$1" | openssl base64 -d)
echo $s
echo $s | pbcopy
```


## 参考

* https://www.madboa.com/geek/openssl/#how-do-i-base64-encode-something
* http://superuser.com/questions/120796/os-x-base64-encode-via-command-line

{{ page.date | date_to_string }},{{ page.author }}
