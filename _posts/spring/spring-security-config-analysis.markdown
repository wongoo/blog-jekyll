---
author: 望哥
comments: true
date: 2018-03-19
title: Spring Security 认证配置错误分析
categories:
- spring
tags:
- spring-security
- spring

---

# 1. 问题描述：
同事升级公司内部spring boot项目到1.5.x版本后，发现oauth2认证失效，去掉token可以跳过认证。

# 2. 问题分析：
spring-boot的oauth2认证本身基于spring security，每一个请求都会经过spring security的 FilterChain中的所有filter执行一遍，如果去掉token都未认证，难道是FilterChain中有少oauth2的filter？
带着这个怀疑进行debug调试，将断点设置到:

![](media/files/spring/spring-security-config-analysis/debug-point.png)

升级前的filter列表：

![](media/files/spring/spring-security-config-analysis/filters-before-upgrade.png)

debug升级后的代码，filters列表关键少了 OAuth2AuthenticationProcessingFilter 这个oauth2处理的filter。

为什么会少？项目maven配置对oauth2的引用 org.springframework.cloud:spring-cloud-starter-oauth2 没有少。没办法直接想到原因，从filters继续往下查。

对于一个请求对应的filters列表，来源于FilterChainProxy.filterChains多个SecurityFilterChain列表中第一个匹配的。
通过调用SecurityFilterChain.matches(request) 匹配判断请求reqeust满足的那一个filter链。
每个SecurityFilterChain都是一个DefaultSecurityFilterChain实例。

下图是升级前可以看到有多个FilterChain，其中有三个的匹配实例都是AnyRequestMatcher，这样如果前面几个url不匹配，都会匹配到下标为5的FilterChain，
而6、7、8几个FilterChain是不会请求到的。升级前第5个FilterChain有包含OAuth2AuthenticationProcessingFilter这个filter。

![](media/files/spring/spring-security-config-analysis/chains-before.png)
![](media/files/spring/spring-security-config-analysis/filters-before.png)

升级后的filterChains变成这样，下标为4的FilterChain没有OAuth2AuthenticationProcessingFilter

![](media/files/spring/spring-security-config-analysis/chains-after.png)
![](media/files/spring/spring-security-config-analysis/filters-after-1.png)

而下标8的filterChain才有OAuth2AuthenticationProcessingFilter：
![](media/files/spring/spring-security-config-analysis/filters-after-2.png)

从上可以看出FilterChain有顺序问题，而且有很多FilterChain是多余的。要解释这两个问题首先得了解到FilterChain是如何创建的。
FilterChainProxy通过WebSecurity创建，并通过 WebSecurity 的  ignoredRequests和securityFilterChainBuilders两个列表的创建对应的filterChain。
```
int chainSize = ignoredRequests.size() + securityFilterChainBuilders.size();
List<SecurityFilterChain> securityFilterChains = new ArrayList<SecurityFilterChain>(chainSize);
```

ignoredRequests创建的SecurityFilterChain的Filter列表为空，所有就会直接通过无需认证。
其中securityFilterChainBuilders 通过 WebSecurityConfigurerAdapter 进行init时添加到WebSecurity中，builder实例是一个HttpSecurity对象，
而WebSecurityConfigurerAdapter负责对HttpSecurity进行配置。通过调用每一个builder （即HttpSecurity） 的build方法创建相关FilterChain。
可见在一个应用中如果有多个 WebSecurityConfigurer，就会创建多个FilterChain，而完全匹配（AnyRequestMatcher）的FilterChain只有第一个会被使用到。
从以下代码注释可以了解这些类之间的关系。

```
/**
 * Allows customization to the {@link WebSecurity}. In most instances users will use
 * {@link EnableWebSecurity} and a create {@link Configuration} that extends
 * {@link WebSecurityConfigurerAdapter} which will automatically be applied to the
 * {@link WebSecurity} by the {@link EnableWebSecurity} annotation.
 *
 * @see WebSecurityConfigurerAdapter
 */
public interface WebSecurityConfigurer<T extends SecurityBuilder<Filter>> extends SecurityConfigurer<Filter, T>

/**
 * Provides a convenient base class for creating a {@link WebSecurityConfigurer} instance.
 * The implementation allows customization by overriding methods.
 * @see EnableWebSecurity
 */
@Order(100)
public abstract class WebSecurityConfigurerAdapter implements WebSecurityConfigurer<WebSecurity>


* Uses a {@link WebSecurity} to create the {@link FilterChainProxy} that performs the web
* based security for Spring Security. It then exports the necessary beans. Customizations
* can be made to {@link WebSecurity} by extending {@link WebSecurityConfigurerAdapter}
* and exposing it as a {@link Configuration} or implementing
* {@link WebSecurityConfigurer} and exposing it as a {@link Configuration}. This
* configuration is imported when using {@link EnableWebSecurity}.
*
@Configuration
public class WebSecurityConfiguration implements ImportAware, BeanClassLoaderAware
    /*
     * WebSecurityConfiguration添加SecurityConfigurer<Filter, WebSecurity> （主要是WebSecurityConfigurerAdapter对象）列表，
     * 会对其进行排序，然后也添加到WebSecurity中
     */
    private List<SecurityConfigurer<Filter, WebSecurity>> webSecurityConfigurers;
```

经查询代码，原来的项目引用的库本来就有多个 WebSecurityConfigurerAdapter 的安全配置，
升级后项目有自己增加了一个WebSecurityConfigurerAdapter配置，而这个新增的配置优先级最高，导致获得的FilterChain不是所期望的。

发生这个问题是因为开发人员认为增加自定义的安全认证就是增加WebSecurityConfigurerAdapter配置就好，而且也没有注意其Order顺序问题。

# 3. 解决办法：

将相关安全配置的东西全部放到 ResourceServerConfigurerAdapter 中去配置， 这样filterChains列表中就只有一个完全匹配（AnyRequestMatcher）的FilterChain，且包含oauth2认证。





# 4. 扩展学习：

## 4.1. 是否可以减少filterChains列表?

从下图可以看到前几个configurer都是spring-boot内部提供的配置项，没办法去掉。
![](media/files/spring/spring-security-config-analysis/adapter-list.png)

一个经验是：配置spring security最好是基于spring security的的扩展去做，比如ResourceServerConfigurerAdapter，他们已经控制好了相关Order顺序问题。
万一要基于spring security本身机制进行配置，最好先了解一下系统内部其他默认配置的order值，再设置自己的order值。


## 4.2. 去掉多余的filter
我们的服务是无状态、分布式的的微服务架构，很多filter都不需要，比如csrf、session、anonymous、logout等。
```
http.csrf().disable()
    .sessionManagement().disable()
    .logout().disable()
    .anonymous().disable()
```

## 4.3. 配置ignore地址的两种方式的差异
```
public abstract class WebSecurityConfigurerAdapter implements WebSecurityConfigurer<WebSecurity>
public void configure(WebSecurity web) throws Exception {
    /*
     * 此种方式是在FilterChain层级判断是否不认证，不经过任何filter处理
     * 每一个pattern会单独创建一个FilterChain，但也意味着对于每一个请求都会和它进行匹配检查一次。
     */
    web.ignoring().antMatchers("/v2/api-docs");
}

protected void configure(HttpSecurity http) throws Exception {
    /* 此种方式是filter级别判断是否不认证，请求会经过所有filter，被当做匿名访问请求。
     * permitAll()是针对所有已认证的(authenticated)用户，包括anonymous，如果anonymous被禁用，需要匿名访问的地址则不会生效。
     * 所以此种情况需要启用anonymous filter。
     */
    http.authorizeRequests().antMatchers("/info").permitAll();
}
```

## 4.4. session管理配置的差异

第一种方式，禁用session管理，不会加入到filter列表中:
```
http.sessionManagement().disable()：
```


第二张方式，加入filter列表，spring security 不会创建也不会使用任何session，但这个设置仅限于spring security，和应用是否使用session无关。
配置stateless的情况下，SessionManagementFilter使用了ChangeSessionIdAuthenticationStrategy策略，主要触发sessionId改变事件。
```
http.sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS)：
```

对于无状态的微服务，应用本身都未使用session，所以最好使用http.sessionManagement().disable()。



# 5.参考：

- [【译】Spring 官方教程：Spring Security 架构](http://www.spring4all.com/article/554)
- [Spring Security Architecture](https://spring.io/guides/topicals/spring-security-architecture/)
- [Control the Session with Spring Security](http://www.baeldung.com/spring-security-session)

{{ page.date | date_to_string }},{{ page.author }}