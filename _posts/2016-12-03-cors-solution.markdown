---
layout: post
author: gelnyang
comments: true
date: 2016-12-03
title: 如何实现跨域访问
categories:
- 经验
tags:
- 开发
- 经验
- CORS
---
---

# 如何实现跨域访问

## 1. 背景

### 1.1 跨应用访问
假设现在有两个应用，应用A和应用B，分别可通过以下两个地址访问,A应用的index.html要跨域访问B应用的data.html ：

```Text
应用A: http://127.0.0.1:8080/app_a/index.html
应用B: http://127.0.0.1:8081/app_b/data.txt
```

index.html:

```HTML
<html>
 <head><title>index</title>
   <script src="http://apps.bdimg.com/libs/jquery/2.1.4/jquery.min.js"></script>
 </head>

 <body>
  <h1>Index Page of application A</h1>
  <div id="container"></div>
  <script>
   $("#container").load("http://127.0.0.1:8081/app_b/data.txt");
  </script>
 </body>
</html>
```

data.txt:

```Text
DATA OF APPLICATION B
```


### 1.2. nginx 配置两个应用访问地址
nginx.conf配置内容如下：

```JSON
worker_processes  1;
events {
        worker_connections  1024;
}
http {
        server {
                listen       8080;
                location /
                {
                        root /app/nginx/html;
                }
        }
        server {
                listen       8081;
                location /
                {
                        root /app/nginx/html;
                }
        }
}
```


### 1.3 测试访问

浏览器访问 http://127.0.0.1:8080/app_a/index.html, 请求request的头部有包含Origin头部，其值为http://127.0.0.1:8080，浏览器会将其和response中的Access-Control-Allow-Origin的头部值进行比较，如果一致，或者Access-Control-Allow-Origin的值为“*”, 则允许访问.

但回复response头部不包含Access-Control-Allow-Origin，则不允许访问，提示报错：

```Text
XMLHttpRequest cannot load http://127.0.0.1:8081/app_b/data.txt. No 'Access-Control-Allow-Origin' header is present on the requested resource. Origin 'http://127.0.0.1:8080' is therefore not allowed access. The response had HTTP status code 404.
```

## 2. 服务端Response增加Access-Control-Allow-Origin头部允许跨域访问：

修改应用B的配置，在response中增加Access-Control-Allow-Origin头部允许跨域访问：

```JSON
server {
        listen       8081;
        location /
        {
                root /app/nginx/html;
                add_header  "Access-Control-Allow-Origin" "*";
        }
}
```

注意：此处配置为“*”，允许所有其他应用访问，也可以配置为 "http://127.0.0.1:8080" 只允许这个应用地址的请求。

![CORS Request](/media/files/2016/cors_request.png "CORS Request")


## 3. 通过反向代理转发请求实现跨域

将请求应用A和B的请求都通过统一的反向代理地址去访问，使其对client端而言不存在跨域问题。

ngnix增加一个server配置映射8082端口，反向代理应用A和应用B的请求：

```JSON
http {
        server {
                listen       8080;
                location /
                {
                        root /app/nginx/html;
                }
        }
        server {
                listen       8081;
                location /
                {
                        root /app/nginx/html;
                }
        }
        server {
                listen       8082;
                location /app_a
                {
                        proxy_pass http://127.0.0.1:8080;
                }
                location /app_b
                {
                        proxy_pass http://127.0.0.1:8081;
                }
        }
}
```

现在通过 http://127.0.0.1:8082/app_a/index.html 访问应用A，这个时候对客户端而言相当于请求同一个应用，也就无跨域的问题了。


![CORS Proxy Solution](/media/files/2016/cors_proxy_solution.png "CORS Proxy Solution")


## 参考

* 《跨域资源共享 CORS 详解》，阮一峰, http://www.ruanyifeng.com/blog/2016/04/cors.html



{{ page.date | date_to_string }},{{ page.author }}
