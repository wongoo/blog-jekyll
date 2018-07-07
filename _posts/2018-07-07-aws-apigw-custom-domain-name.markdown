---
author: 望哥
comments: true
date: 2018-07-07
title: aws api gateway配置custom domain name遇到的那些坑
categories:
- 经验
tags:
- AWS

---

先讲一下未配置custom domain name之前我们的技术架构：
* 后端使用lambda提供多个服务（架设有record和info两个服务）
* 使用api gateway提供请求集成暴露调用接口
* 在api gateway配置中配置authorizer对资源方法`/a/ANY`进行认证

按照以上架构, lambda record 服务提供以下访问资源:
* 插入记录，需Authorizer认证: `/a/insert`
* 查询记录，无需认证: `/list`

lambda info 服务提供以下访问资源:
* 插入记录，需Authorizer认证: `/a/insert`
* 查询记录，无需认证: `/list`

分别为record和info两个服务配置 api gateway， 他们的配置如下，只是整合的lambda服务不一样:

```
/
    /a
        OPTIONS
        /{proxy+}
            ANY       ---> 整合lambda record/info 服务, 配置Authorizer认证
            OPTIONS
    /{proxy+}
        ANY           ---> 整合lambda record/info 服务
        OPTIONS
```

按照以上配置，设定阶段名称为live，访问服务的地址为:
* https://xxxx.execute-api.cn-north-1.amazonaws.com.cn/live/a/insert
* https://xxxx.execute-api.cn-north-1.amazonaws.com.cn/live/list
* https://yyyy.execute-api.cn-north-1.amazonaws.com.cn/live/a/insert
* https://yyyy.execute-api.cn-north-1.amazonaws.com.cn/live/list

可以看出url阶段名称(live)后面的地址（如/a/insert）就是api gateway中配置的地址，也是lambda种资源的地址。

以上配置有两个问题：
* 访问地址是aws的域名地址，不是自己公司的域名；
* 服务接口地址很多，对前端整合带来很多麻烦；


我们期望最后的访问路径变为:
* https://api.company.com/record/a/insert
* https://api.company.com/record/list
* https://api.company.com/info/a/insert
* https://api.company.com/info/list


**如果要使用自己的域名，就需要使用api gateway的custom domain name的功能。**

custom domain name是配置一个域名，如 `api.company.com` ,并设置自己域名的证书和私钥。
接下来就是要设定一个base path（如record），设置这个base path对应的api gatway及其阶段, 如下：

```
Create a CNAME resource record with your DNS provider to map api.company.com to zzzz.execute-api.cn-north-1.amazonaws.com.cn

Certificate name: api.company.com
Target domain name: zzzz.execute-api.cn-north-1.amazonaws.com.cn

API Mappings
Base path 	API 	    Stage
-------------------------------
record 	    record  	live
info 	    info 	    live
```

但这样配置后访问 `https://api.company.com/record/list` 提示404找不到资源地址。
查看lambda日志，lambda 接收到的资源请求地址是 `/record/list`,

大胆假设: **custom domain name访问时资源的转发地址会把域名后面的全部地址(包括 base path)作为资源地址**。

于是修改lambda record 服务资源地址为:
* /record/a/insert
* /record/list

把api geteway配置改为:

```
/
    /record
        /a
            OPTIONS
            /{proxy+}
                ANY       ---> 整合lambda record 服务, 配置Authorizer认证
                OPTIONS
        /{proxy+}
            ANY           ---> 整合lambda record 服务
            OPTIONS

```

这样修改后，直接通过api gateway访问的地址是:
* https://xxxx.execute-api.cn-north-1.amazonaws.com.cn/live/record/a/insert
* https://xxxx.execute-api.cn-north-1.amazonaws.com.cn/live/record/list

但是这时候通过custom domain name访问 `https://api.company.com/record/list` 返回 `{"message":"Missing Authentication Token"}`，
按照经验，这个返回是因为api gateway无法路由资源请求。因为刚才还可以，现在不行，尝试恢复api gateway的配置为：

```
/
    /a
        OPTIONS
        /{proxy+}
            ANY       ---> 整合lambda record 服务, 配置Authorizer认证
            OPTIONS
    /{proxy+}
        ANY           ---> 整合lambda record 服务
        OPTIONS
```
这样配置所有直接通过api gateway的请求实际上都是通过 `//{proxy+}/ANY` 方法映射的，也就没有进行验证。
而通过custom domain name访问 `https://api.company.com/record/a/insert` 则是有进行验证的。

得出结论: **通过custom domain name访问的请求会将base path后面部分交由api gateway去路由，但却会以包括base path的完整资源地址请求后端lambda服务。**

团队原本想做整合测试的时候直接通过api gateway的地址访问，
如果要实现这样的功能，api gateway资源映射还需要考虑两者请求，分别进行配置，如下：

```
/
    /a
        OPTIONS
        /{proxy+}
            ANY       ---> 整合lambda record 服务, 配置Authorizer认证
            OPTIONS
    /{proxy+}
        ANY           ---> 整合lambda record 服务
        OPTIONS
    /record
        /a
            OPTIONS
            /{proxy+}
                ANY       ---> 整合lambda record 服务, 配置Authorizer认证
                OPTIONS
        /{proxy+}
            ANY           ---> 整合lambda record 服务
            OPTIONS
```

这样配置还真是蛋疼啊！

最后，来看看两种请求的流程不同之处。

**通过 api gateway 地址请求**:

```
1. 请求地址: https://xxxx.execute-api.cn-north-1.amazonaws.com.cn/live/record/a/insert
2. 获得 api gateway 资源映射路径（stage 之后的部分）: /record/a/insert
3. 查找 api gateway 对应的路由方法: //record/a/{proxy+}/ANY
4. api gateway 路由请求 /record/a/insert 到 lambda 服务
```

**通过 custom domain name 地址请求**:

```
1. 请求地址: https://api.company.com/record/a/insert
2. 获得 api gateway 资源映射路径（base path之后的部分）: /a/insert
3. 查找 api gateway 对应的路由方法: //a/{proxy+}/ANY
4. api gateway 路由请求 /record/a/insert （base path + 资源映射路径）到 lambda 服务
```

希望以上内容能为你带来帮助！

{{ page.date | date_to_string }},{{ page.author }}