---
layout: post
author: gelnyang
comments: true
date: 2017-03-01
title: Netflix feign exception logging
categories:
- experience
tags:
- spring-cloud
- netflix
- feign
- hystrix
- exception
---
---

项目中使用netflix feign访问其他微服务接口，接口访问一直正常，最近突然访问不成功，服务请求最终都会被熔断call back处理。
分析目标服务是有接收到请求并正常处理返回，怀疑是feign处理回复结果的时候有异常，但并没有任何异常报错。

Feign client：

```
@FeignClient(value = "SOP-APP-SERVICE", fallback = SopAppServiceHystrix.class)
public interface SopAppService {

    /**
     * 获得应用
     */
    @RequestMapping(path = "/application/{appId}", method = RequestMethod.GET)
    public AppInfo getAppInfo(@PathVariable("appId") String appId);
}
```
Hystrix:

```
@Slf4j
@Service
public class SopAppServiceHystrix implements SopAppService {

    @Override
    public AppInfo getAppInfo(String appId) {
        log.error("failed to get app info with id {}", appId);
        return null;
    }
}
```

经google一番，可通过设置 logging.level.com.netflix=debug，有异常的时候打印出异常信息。


但这样实在是麻烦，还需要设置debug模式。再google一番，发现可以通过配置HystrixCommandExecutionHook实现。
HystrixCommandExecutionHook提供了feign请求各个时间节点的钩子，其中包括请求发生异常的时候，可以获得具体的异常类信息。
以下是具体的实现代码:

Hook Definition:

```
@Slf4j
public class DefaultHystrixCommandExecutionHook extends HystrixCommandExecutionHook {

    @Override
    public <T> Exception onError(HystrixInvokable<T> commandInstance,
            HystrixRuntimeException.FailureType failureType, Exception e) {
        log.error(e.getMessage(), e);
        return super.onError(commandInstance, failureType, e);
    }

    @Override
    public <T> Exception onExecutionError(HystrixInvokable<T> commandInstance, Exception e) {
        log.error(e.getMessage(), e);
        return super.onExecutionError(commandInstance, e);
    }

    @Override
    public <T> Exception onFallbackError(HystrixInvokable<T> commandInstance, Exception e) {
        log.error(e.getMessage(), e);
        return super.onFallbackError(commandInstance, e);
    }
}
```

Hook Registration:

```
@Configuration
public class ServiceBaseConfiguration {

    @PostConstruct
    public void init() {
        HystrixPlugins.getInstance()
                .registerCommandExecutionHook(new DefaultHystrixCommandExecutionHook());
    }
}
```

{{ page.date | date_to_string }},{{ page.author }}

