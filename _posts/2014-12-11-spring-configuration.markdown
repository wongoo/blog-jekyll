---
author: admin
comments: true
date: 2014-12-11 08:43:00+00:00
excerpt: 了解了spring框架PropertySource、PropertyResolver、Environment的设计架构,如果将Environment引入到项目参数配置方案中，将带来极大的方便。仅需通过@Inject将其注入Environment实例即可获得相关配置属性。
layout: post
slug: spring-configuration
title: Spring项目参数配置方案
wordpress_id: 798
categories:
- Experience
tags:
- configuration
- Environment
- spring
---

了解了[spring框架PropertySource、PropertyResolver、Environment的设计架构](http://sisopipo.com/blog/archives/788),如果将Environment引入到项目参数配置方案中，将带来极大的方便。仅需通过@Inject将其注入Environment实例即可获得相关配置属性。



## **1. 获得项目配置信息资源：**


	项目中常常会有多个配置文件，如jdbc.properties, site.properites, version.perperties等，每个配置文件配置的内容并不一样，一般不会出现重复的情况，故可将所有这些配置文件内容合并的一个properties对象中统一作一个属性资源。
	Spring框架的PathMatchingResourcePatternResolver提供了读取资源的相关方法,而PropertiesLoaderSupport类的mergeProperties()方法将多个properties进行合并。	故利用这两个类可以获得所有配置信息，即可创建一个配置属性资源对象。
	

    public class AppPropertiesLoader extends PropertiesLoaderSupport {
    	public Properties loadProperties() throws IOException {
    		return this.mergeProperties();
    	}
    }
    
    public PropertiesPropertySource getAppConfigurationPropertySource() throws IOException {
    	AppPropertiesLoader propertiesLoader = new AppPropertiesLoader();
    	propertiesLoader.setLocations(new PathMatchingResourcePatternResolver().getResources("classpath*:*.properties"));
    	return new PropertiesPropertySource("properties", propertiesLoader.loadProperties());
    }




## **2. 添加项目配置资源到Environment资源链表中：**


	按照Spring Javadoc，有两种方式将资源加入到Environment：
	


	
  1. 客制化一个Environment类(Web项目可以继承StandardServletEnvironment)，然后覆盖customizePropertySources方法，通过方法参数变量MutablePropertySources添加资源。然后再调用context的setEnvironment方法替换默认环境资源为此新创建子类。

	
  2. 调用Environment类的getPropertySources()方法，获得属性资源链表，调用其添加方法添加资源。



**第一种方法比较繁琐且涉及修改Spring框架类实例内部变量，不建议。故选择第二种方式。**

如果使用Web相关的ApplicationContext,则会使用StandardServletEnvironment，而其已经有添加了ServletConfig、ServletContext、JNDI、环境属性和系统变量这些资源。
按照内部优先原则，项目配置资源应该作为优先考虑，应将应将项目资源作为最高优先级(addFirst)添加到资源链表中。

    public void initialAppConfiguration(ConfigurableApplicationContext applicationContext) throws IOException {
    	ConfigurableEnvironment environment = applicationContext.getEnvironment();
    	environment.getPropertySources().addFirst(getAppConfigurationPropertySource());
    }

	


## **3. 使用Environment：**


	以下示例从Environment中读取DB连接配置并实例化数据源:

    @Inject
    private Environment env;
    
    @Bean
    public DataSource dataSource() {
    	BasicDataSource dataSource = new BasicDataSource();
    	dataSource.setDriverClassName(env.getRequiredProperty("default.jdbc.driver"));
    	dataSource.setUrl(env.getRequiredProperty("default.jdbc.url"));
    	dataSource.setUsername(env.getRequiredProperty("default.jdbc.username"));
    	dataSource.setPassword(env.getRequiredProperty("default.jdbc.password"));
    	dataSource.setValidationQuery("select 1");
    	dataSource.setDefaultTransactionIsolation(Connection.TRANSACTION_READ_UNCOMMITTED);
    
    	dataSource.setMaxActive(env.getRequiredProperty("default.pool.max.active", Integer.class));
    	dataSource.setMaxIdle(env.getRequiredProperty("default.pool.max.idle", Integer.class));
    	dataSource.setMinIdle(env.getRequiredProperty("default.pool.min.idle", Integer.class));
    	return dataSource;
    }

