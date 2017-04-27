---
layout: post
author: gelnyang
comments: true
date: 2017-04-27
title: message push gateway based on nginx+lua+redis
categories:
- architecture
tags:
- nginx
- lua
- redis
---
---

# Abstract

A gateway is needed for client to push message into redis, the article describe the steps 
to build the gateway based on nginx+lua+redis.

## 1. lua-nginx-module

[ngx_http_lua_module](https://github.com/openresty/lua-nginx-module) - Embed the power of Lua into Nginx HTTP Servers.



### 1.1 installation

see [detail](https://github.com/openresty/lua-nginx-module#installation)

#### 1.1.1 Method1: install modules one by one

1) install luaJIT

Install [LuaJIT](http://luajit.org/download.html) 2.0 or 2.1 (recommended) or [Lua](http://www.lua.org/) 5.1 (Lua 5.2 is not supported yet). 

Some distribution package managers also distribute LuaJIT and/or Lua.

```
 #curl -O http://luajit.org/download/LuaJIT-2.0.4.tar.gz
 curl -O http://luajit.org/download/LuaJIT-2.1.0-beta2.tar.gz
 tar -xzvf LuaJIT-2.1.0-beta2.tar.gz
 cd LuaJIT-2.1.0-beta2
 make
 sudo make install
 
 
 # tell nginx's build system where to find LuaJIT 2.1:
 export LUAJIT_LIB=/usr/local/lib
 export LUAJIT_INC=/usr/local/include/luajit-2.1
```

2) download ngx_devel_kit

Download the latest version of the [ngx_devel_kit (NDK)](https://github.com/simpl/ngx_devel_kit/tags) module: 
    
```
curl -o ngx_devel_kit_v0.3.0.tar.gz https://codeload.github.com/simpl/ngx_devel_kit/tar.gz/v0.3.0
tar -xzvf ngx_devel_kit_0.3.0.tar.gz

```

3) download lua-nginx-module

Download the latest version of [ngx_lua](https://github.com/openresty/lua-nginx-module/tags):  
    
```
curl -o lua-nginx-module.v0.10.8.tar.gz https://codeload.github.com/openresty/lua-nginx-module/tar.gz/v0.10.8
tar -xzvf lua-nginx-module.v0.10.8.tar.gz
```

4) install pcre:

nginx rewrite needs pcre:
```
curl -O https://ftp.pcre.org/pub/pcre/pcre-8.40.tar.gz
tar -xzvf pcre-8.40.tar.gz
./configure
make
sudo make install
```

5) install nginx

Download the latest version of [Nginx](http://nginx.org/): 
    
```
 curl -O http://nginx.org/download/nginx-1.11.13.tar.gz
 tar -xzvf nginx-1.11.13.tar.gz
 
 curl -O http://nginx.org/download/nginx-1.11.2.tar.gz
 tar -xzvf nginx-1.11.2.tar.gz
 cd nginx-1.11.2

 ./configure --prefix=/opt/nginx \
         --with-ld-opt="-Wl,-rpath,/usr/local/lib" \
         --add-module=/Users/gelnyang/temp/ngx_devel_kit-0.3.0 \
         --add-module=/Users/gelnyang/temp/lua-nginx-module-0.10.8

 make -j2
 sudo make install

```


#### 1.1.2 Method2: just install openresty release

```
# http://openresty.org/en/installation.html

tar -xvf openresty-VERSION.tar.gz
cd openresty-VERSION/
./configure -j2
make -j2
sudo make install

# better also add the following line to your ~/.bashrc or ~/.bash_profile file.
export PATH=/usr/local/openresty/bin:$PATH

```


## 2. lua-resty-redis

[Lua redis client driver](https://github.com/openresty/lua-resty-redis#redis-transactions) for the ngx_lua based on the cosocket API


```
curl -o lua-resty-redis.zip https://codeload.github.com/openresty/lua-resty-redis/zip/master
sudo unzip lua-resty-redis.zip -d /opt
```


## 3. nginx config

### 3.1 message_gateway_push.lua

put the file message_gateway_push.lua in /opt/nginx/lua:

```
local redis = require "resty.redis"

local function error(self, message, err)
    if err then
        ngx.print("F|", message, error)
    else
        ngx.print("F|", message)
    end
end

local function success(self, timestamp, sid)
    ngx.print("S|", timestamp, "|", sid)
end

local channel = ngx.var.arg_c

if not channel then
    error(self, "no param c")
    return
end

ngx.req.read_body()
local data = ngx.req.get_body_data()

local red = redis:new()
local ok, err = red:connect("127.0.0.1", 6379)
if not ok then
    error(self, "failed to connect: ", err)
    return
end

red:set_timeout(1000)


ok, err = red:multi()
if not ok then
    if string.find(err, "AUTH", 1, true) then
        ok, err = red:auth("redis_SOUND318")
        if not ok then
            error(self, "failed to auth: ", err)
            return
        end
        ok, err = red:multi()
        if not ok then
            error(self, "failed to multi: ", err)
            return
        end
    else
        error(self, "failed to multi: ", err)
        return  
    end
end

local timestamp = (ngx.now() * 1000)
local sid = ngx.var.request_id
red:lpush(channel,  timestamp.. "|" .. sid .. "|" .. data)
red:lpush("im_message_event", channel)

ok, err = red:exec()
if not ok then
    error(self, "failed to exec: ", err)
    return
end

-- put it into the connection pool of size 500,
-- with 10 seconds max idle time
ok, err = red:set_keepalive(10000, 500)
if not ok then
    red:close()
end

success(self, timestamp, sid)

```

### 3.2 nginx.conf
```
worker_processes  1;

events {
    worker_connections  1024;
}

http {
    lua_package_path "/opt/nginx/lua/?.lua;/opt/lua-resty-redis-master/lib/?.lua;;";

    server {
        listen 80;

    	location /push {
            default_type text/plain;
            content_by_lua_file lua/message_gateway_push.lua;
    	}
    }

}
```


{{ page.date | date_to_string }},{{ page.author }}

