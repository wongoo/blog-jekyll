---
author: admin
comments: true
date: 2014-12-11 03:06:53+00:00
excerpt: Spring框架PropertySource、PropertyResolver、Environment的设计架构
layout: post
slug: spring-propertysource-propertyresolver-environment
title: Spring框架PropertySource、PropertyResolver、Environment的设计架构
wordpress_id: 788
categories:
- Knowledge
tags:
- Environment
- PropertyResolver
- PropertySources
- spring
---

# **1. 属性资源(PropertySource)**




### **1) public abstract class PropertySource **


属性资源，主要包括以下两个方法：

    public boolean containsProperty(String name);
    public abstract Object getProperty(String name);



### **2) public abstract class EnumerablePropertySource extends PropertySource**


可枚举属性资源，增加可返回属性名称数组的方法：

    public abstract String[] getPropertyNames();



### **3) public class MapPropertySource extends EnumerablePropertySource<Map<String, Object>>**


属性资源实现类，资源类型是一个Map对象。


### **4) public class SystemEnvironmentPropertySource extends MapPropertySource**


MapPropertySource的子类，用于获取系统环境变量。弥补了在Bash或其他shells中变量不允许包含period character(.)的不足。允许一般shell中变量是大写的情况。

例如，调用getProperty("foo.bar")将按照以下顺序去找变量值：
foo.bar - the original name (原始变量)
foo_bar - with underscores for periods (if any) (下划线)
FOO.BAR - original, with upper case (大写)
FOO_BAR - with underscores and upper case (下划线、大写)

调用containsProperty(String)也是按照同样的顺序，如果找到了在返回true，否则false。这种方法在通过环境变量声明激活或默认的情景模式是很有用。以下方式再Bash中是不被允许的：

    spring.profiles.active=p1 java -classpath ... MyApp

但是以下语法则是被允许的也是常见的用法：

    SPRING_PROFILES_ACTIVE=p1 java -classpath ... MyApp


启用debug或trace层级的log时，就会有类似'property name resolutions'这样的日志出现。
此属性资源默认包含在StandardEnvironment和其子类中。


### **5) 其他属性资源其对应的资源类型：**



    PropertiesPropertySource:java.util.Properties
    ResourcePropertySource:java.util.Properties
    JOptCommandLinePropertySource:joptsimple.OptionSet
    ServletConfigPropertySource:javax.servlet.ServletConfig
    ServletContextPropertySource:javax.servlet.ServletContext
    JndiPropertySource:org.springframework.jndi.JndiLocatorDelegate




### **6) public class MutablePropertySources implements PropertySources**


其实际上是一个属性资源链表，用于包含其他实际的属性资源，并实现了资源的顺序访问。



# **2. 属性解析器(PropertyResolver)**



**1) public interface PropertyResolver**

定义了获取属性的相关方法:

    String getProperty(String key);
    String getProperty(String key, String defaultValue);
    T getProperty(String key, Class targetType);
    T getProperty(String key, Class targetType, T defaultValue);
    String getRequiredProperty(String key) throws IllegalStateException;
    T getRequiredProperty(String key, Class targetType) throws IllegalStateException;
    String resolvePlaceholders(String text);
    String resolveRequiredPlaceholders(String text) throws IllegalArgumentException;




### **2) public interface ConfigurablePropertyResolver extends PropertyResolver**


定义了设置属性获取条件的相关方法：

    ConfigurableConversionService getConversionService();
    void setConversionService(ConfigurableConversionService conversionService);
    void setPlaceholderPrefix(String placeholderPrefix);
    void setPlaceholderSuffix(String placeholderSuffix);
    void setValueSeparator(String valueSeparator);
    void setRequiredProperties(String... requiredProperties);
    void validateRequiredProperties() throws MissingRequiredPropertiesException;
    void setIgnoreUnresolvableNestedPlaceholders(boolean ignoreUnresolvableNestedPlaceholders);




### **3) public abstract class AbstractPropertyResolver implements ConfigurablePropertyResolver**


属性解析器虚类，实现了大多属性设置和获取的相关方法



### **4) public class PropertySourcesPropertyResolver extends AbstractPropertyResolver**


属性解析器实现类，AbstractEnvironment中相关属性处理方法都是适配调用此类的方法。



# **3. 环境信息(Environment )**





### **1) public interface Environment extends PropertyResolver：**


只定义了获取情景模式的方法，属性获取的方法已在父接口定义:

    String[] getActiveProfiles();
    String[] getDefaultProfiles();
    //此方法在DefaultBeanDefinitionDocumentReader.doRegisterBeanDefinitions(xml配置)
    //和ProfileCondition.matches(annotation注解)两个方法中被调用,如果bean有定义profile但不是active的则不会被初始化。
    boolean acceptsProfiles(String... profiles);


该接口用于表示当前应用程序正在运行的环境信息。主要包括两个应用模型：profiles(情景模式)和properties(属性)。访问属性的相关方法则是由父接口PropertyResolver定义的。
Profile(情景模式)指一个指定名称Bean定义逻辑组，只有当其处于active激活状态时，这些Bean才会注册到容器中。Beans可以指定属于某一个Profile，不管是XML还是注解配置（详细定义语法请查看spring-beans 3.1 schema定义或者@Profile注解说明）。由Environment对象决定决定哪一个是默认的Profile，以及当前使用哪一个激活的Profile。
属性定义对大多应用都很重要，一般可能有多种定义来源，如properties文件、JVM系统参数、系统变量、JNDI、Servlet容器参数、AD-HOC属性对象、Maps等等。Environment提供一个便捷的service接口配置读取各种属性资源。

Beans可以继承EnvironmentAware接口或者添加注解@Inject注入Environment对象，以便查询当前profiles(情景模式)状态以及获取属性定义。

但是，大多情况下应用层面的bean不需要直接和Environment对象直接交互，而是配置属性placeholder ${...}}来获得属性值，比如PropertySourcesPlaceholderConfigurer类，其继承了EnvironmentAware，从Spring 3.1开始如果使用了则会自动注册加载此类。
AbstractApplicationContext继承ConfigurableEnvironment接口，所有其子类都可以通过getEnvironment()方法获得environment对象。可以通过ConfigurableEnvironment的Javadoc查看在调用application context的refresh()方法前读取多种属性资源的例子。



### **2) public interface ConfigurableEnvironment extends Environment, ConfigurablePropertyResolver **


定义了设置情景模式、以及获取特定系统属性资源、合并环境信息的相关方法：

    void setActiveProfiles(String... profiles);
    void addActiveProfile(String profile);
    void setDefaultProfiles(String... profiles);
    MutablePropertySources getPropertySources();
    Map getSystemEnvironment();
    Map getSystemProperties();
    void merge(ConfigurableEnvironment parent);


ConfigurableEnvironment是一个配置接口，大多数Environment对象都是实现于此接口。提供设置激活或默认profiles(情景模式)的功能，也可以管理各种属性资源。允许用户设置和验证必要的属性、客制化转换服务、以及更多父类接口ConfigurablePropertyResolver定义的功能。

**管理属性资源**
属性资源可以删除、排序或替换；还可以通过getPropertySources()方法返回的MutablePropertySources实例添加额外的属性资源。下面针对ConfigurableEnvironment的标准实现类StandardEnvironment列出几个例子，尽管对于某些特殊属性资源处理方式稍有差异，但仍适用于所有实现的情况。

Example: 添加一个具有最高查询优先级的属性资源

    ConfigurableEnvironment environment = new StandardEnvironment();
    MutablePropertySources propertySources = environment.getPropertySources();
    Map myMap = new HashMap();
    myMap.put("xyz", "myValue");
    propertySources.addFirst(new MapPropertySource("MY_MAP", myMap));


Example: 删除默认的系统属性资源

    MutablePropertySources propertySources = environment.getPropertySources();
    propertySources.remove(StandardEnvironment.SYSTEM_PROPERTIES_PROPERTY_SOURCE_NAME)


Example: 测试情况下替换模拟系统属性资源

    MutablePropertySources propertySources = environment.getPropertySources();
    MockPropertySource mockEnvVars = new MockPropertySource().withProperty("xyz", "myValue");
    propertySources.replace(StandardEnvironment.SYSTEM_ENVIRONMENT_PROPERTY_SOURCE_NAME, mockEnvVars);


需要注意，但ApplicationContext使用Environment时，所有属性资源应该在调用context的refresh()方法之前加载处理完成，以便确保再容器启动以及placeholder配置时候所有属性资源都是可获取的。


### **3) public abstract class AbstractEnvironment implements ConfigurableEnvironment **


Environment实现虚类。支持默认保留的情景模式名称，可通过spring.profiles.default和spring.profiles.default属性指定激活和默认的情景模式。
子类主要的区别是默认添加的PropertySource属性资源不同。AbstractEnvironment不会添加。子类需要覆盖customizePropertySources(MutablePropertySources)方法提供属性资源，这样用户可以使用ConfigurableEnvironment.getPropertySources()客制化要求，同时也能和MutablePropertySources API并存使用。具体查看ConfigurableEnvironment的javadoc查看使用案例。
另外此类还实现了大多ConfigurableEnvironment相关方法，如获取系统属性和系统变量，设置情景模式等。

其定义了一下两个变量：

    private final MutablePropertySources propertySources = new MutablePropertySources(this.logger);//存储各种属性资源
    private final ConfigurablePropertyResolver propertyResolver = new PropertySourcesPropertyResolver(this.propertySources);//属性资源相关方法适配类


关于customizePropertySources方法，其主要用于客制化PropertySource属性资源对象集合，以便Environment调用getProperty(String)及相关方法时获取属性。
建议子类覆盖此方法时，使用MutablePropertySources.addLast(PropertySource)添加属性资源，这样子类的子类就可以调用super.customizePropertySources()添加父类指定的资源。例如：


    public class Level1Environment extends AbstractEnvironment {
    	@Override
    	protected void customizePropertySources(MutablePropertySources propertySources) {
    		super.customizePropertySources(propertySources); // no-op from base class
    		propertySources.addLast(new PropertySourceA(...));
    		propertySources.addLast(new PropertySourceB(...));
    	}
    }
    
    public class Level2Environment extends Level1Environment {
    	@Override
    	protected void customizePropertySources(MutablePropertySources propertySources) {
    		super.customizePropertySources(propertySources); // add all from superclass
    		propertySources.addLast(new PropertySourceC(...));
    		propertySources.addLast(new PropertySourceD(...));
    	}
    }

按照这样的结构，属性资源的顺序是A,B,C,D。这就是说A有优先于D。如果Level2Environment喜欢C,D的优先级比A,B高，可以在方法最后调用super.customizePropertySources，如：

    public class Level2Environment extends Level1Environment {
    	@Override
    	protected void customizePropertySources(MutablePropertySources propertySources) {
    		propertySources.addLast(new PropertySourceC(...));
    		propertySources.addLast(new PropertySourceD(...));
    		super.customizePropertySources(propertySources); // add all from superclass
    	}
    }

这样顺序就变为了C,D,A,B.
还需说明，子类可以调用MutablePropertySources提供的add、remove、replace方法来调整组织资源顺序。AbstractEnvironment中未注册任何资源。
需要注意，任何ConfigurableEnvironment对象的调用者还可以通过getPropertySources()方法来调整属性资源，一般是在ApplicationContextInitializer对象中。例如：

    ConfigurableEnvironment env = new StandardEnvironment();
    env.getPropertySources().addLast(new PropertySourceX(...));


**关于访问实例变量的警告：**
定义在子类中的实例变量可能会有其初始化值，不要在customizePropertySources方法中访问这些变量。因为Java对象创建生命周期的限制，任何变量不要在AbstractEnvironment()的构造方法调用此callback方法customizePropertySources的时候设置值初始化值，不然会造成空指针异常或其他错误。如果你的确需要获取实例变量的默认值，则不用覆盖此方法，在资料的构造方法中直接设置属性资源和访问实例变量。注意给实例变量赋值是没有问题的，而是应尽量避免在此方法中访问实例变量默认值。


### **4) public class StandardEnvironment extends AbstractEnvironment**


该类Environment的实现类，用于“标准”（非web）应用。除了ConfigurableEnvironment的常用功能，如获取属性、情景模式相关操作，该实现类配置了两个默认属性资源，他们的查找顺序如下：
system properties （系统属性）
system environment variables（系统环境变量）

例如，如果“xyz”在JVM系统属性和当前进程的环境变量都存在，environment.getProperty("xyz")则会返回jvm系统属性的值。这是默认被采用的顺序，因为系统属性是针对没有jvm的，而系统变量则可能多个jvm都是一样的值。系统属性优先允许再JVM设置属性以覆盖系统变量。

默认属性资源可以被删除、重新排序、或替换。额外的属性资源可以通过getPropertySources()方法返回的MutablePropertySources实例方法添加。

以下实现方法默认添加了系统属性和环境变量两种属性资源:

    protected void customizePropertySources(MutablePropertySources propertySources) {
    	propertySources.addLast(new MapPropertySource(SYSTEM_PROPERTIES_PROPERTY_SOURCE_NAME, getSystemProperties()));
    	propertySources.addLast(new SystemEnvironmentPropertySource(SYSTEM_ENVIRONMENT_PROPERTY_SOURCE_NAME, getSystemEnvironment()));
    }




### **5) public interface ConfigurableWebEnvironment extends ConfigurableEnvironment**


定义了Web应用特有的接收属性资源ServletContext和ServletConfig对象并进行初始化的方法：

    void initPropertySources(ServletContext servletContext, ServletConfig servletConfig);




### **6) public class StandardServletEnvironment extends StandardEnvironment implements ConfigurableWebEnvironment**


针对servlet web应用的Environment实现。所有web相关（servlet）的ApplicationContext都会初始化这样一个实例。
其包含ServletConfig, ServletContext, 和 JNDI-based的属性资源实例。更多查看customizePropertySources方法描述。


    protected void customizePropertySources(MutablePropertySources propertySources) {
    	propertySources.addLast(new StubPropertySource(SERVLET_CONFIG_PROPERTY_SOURCE_NAME)); //添加ServletConfig属性资源
    	propertySources.addLast(new StubPropertySource(SERVLET_CONTEXT_PROPERTY_SOURCE_NAME)); //添加ServletContext属性资源
    	if (JndiLocatorDelegate.isDefaultJndiEnvironmentAvailable()) {
    		propertySources.addLast(new JndiPropertySource(JNDI_PROPERTY_SOURCE_NAME)); //添加JNDI属性资源
    	}
    	super.customizePropertySources(propertySources); //再调用父类方法添加系统属性和环境变量
    }
    
    	@Override
    	public void initPropertySources(ServletContext servletContext, ServletConfig servletConfig) {
    		//初始化替换customizePropertySources方法创建的Stub类型的ServletConfig、ServletContext属性资源
    		WebApplicationContextUtils.initServletPropertySources(getPropertySources(), servletContext, servletConfig);
    	}




### **7) public interface EnvironmentCapable:**


该接口定义了方法getEnvironment()，表明一个类包含了一个Environment引用。

所有的spring application contexts都实现了EnvironmentCapable接口，该接口主要用于框架的instanceof判断，框架方法判断接收的BeanFactory实例（可能是也可能不是ApplicationContext实例）是否EnvironmentCapable接口的实现类，如果是且存在environment，则可以和environment对象交互。
因ApplicationContext继承于EnvironmentCapable，故拥有getEnvironment()方法。ConfigurableApplicationContext重新定义了getEnvironment()指定方法返回一个ConfigurableEnvironment。这样的效果是使ConfigurableApplicationContext开始访问Environment对象前其状态只是只读的，ConfigurableApplicationContext中则可以再修改。



### **8) public interface EnvironmentAware extends Aware**


此接口主要用于注入environment对象。

    void setEnvironment(Environment environment);

