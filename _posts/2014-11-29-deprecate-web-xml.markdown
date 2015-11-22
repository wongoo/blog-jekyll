---
author: admin
comments: true
date: 2014-11-29 17:03:33+00:00
excerpt: Annotation的时代——遗弃web.xml
layout: post
slug: deprecate-web-xml
title: 注解时代——遗弃web.xml
wordpress_id: 749
categories:
- Inspiration
tags:
- annotation
- java
- servlet
- spring
- web.xml
---

**注解Annotation的时代，xml终要被人遗弃。**


在spring盛行之前是EJB的时代，EJB大量使用注解，框架复杂，让初学的人望而生畏。而Spring提供依赖注入功能，只需写好关注于业务本身的POJO以及相关服务类，另外通过灵活的xml配置即可实现复杂的业务功能，慢慢Spring替代EJB成为事实上JavaEE开发的标准。而XML配置替代注解的方式也被大家所认同和接受。


但是后来，大家开始觉得xml配置实属多余和繁杂，一个大型项目的xml配置太繁多，而且大多是重复类似的配置，维护起来也很麻烦，和维护java代码无甚差别。Spring于是加入了类似这样的组件扫描的xml配置接口，相关java组件只需用注解声明依赖关系即可。


使用上了注解，日子久了觉得用这样的xml配置也是扎眼，何不都换成注解呢？ 没问题，使用通过@Configuration注解的类来定义配置即可。


Spring都换成注解了，但web.xml还需配置ContextLoaderListener和DispatcherServlet,也都是千篇一律的，能否把web.xml也换成注解呢？ 


Servlet自3.0版本提供了javax.servlet.ServletContainerInitializer接口，只要在META-INF/services/javax.servlet.ServletContainerInitializer文件中声明实现类的类完整类名，当servlet container启动的时候就会执行此实现类的onStartup方法，通过此方法可以获得ServletContext对象，则可以在class类中往其中添加Listener或Servlet。 另外还可以通过添加HandlesTypes注解添加要通知的接口的类型，实现或继承此接口的类型将被添加到onStartup方法的webAppInitializerClasses参数传入，这样就提供了一个很方便的扩展接口，将多个初始化逻辑放到实现这个接口的类中。实际上Spring就提供了这样的实现类org.springframework.web.SpringServletContainerInitializer，同时定义了HandlesTypes处理接口org.springframework.web.WebApplicationInitializer，只要实现了WebApplicationInitializer的类都会在容器初始化的时候被调用。这样就可以把web.xml的配置逻辑放到WebApplicationInitializer的实例类中做了，从此遗弃web.xml!


以下附上javax.servlet.ServletContainerInitializer接口的JavaDoc说明：
**Interface which allows a library/runtime to be notified of a web application's startup phase and perform any required programmatic registration of servlets, filters, and listeners in response to it. Implementations of this interface may be annotated with HandlesTypes, in order to receive (at their onStartup method) the Set of application classes that implement, extend, or have been annotated with the class types specified by the annotation. **

If an implementation of this interface does not use HandlesTypes annotation, or none of the application classes match the ones specified by the annotation, the container must pass a null Set of classes to onStartup. When examining the classes of an application to see if they match any of the criteria specified by the HandlesTypes annontation of a ServletContainerInitializer, the container may run into classloading problems if any of the application's optional JAR files are missing. Because the container is not in a position to decide whether these types of classloading failures will prevent the application from working correctly, it must ignore them, while at the same time providing a configuration option that would log them. 

**Implementations of this interface must be declared by a JAR file resource located inside the META-INF/services directory and named for the fully qualified class name of this interface**, and will be discovered using the runtime's service provider lookup mechanism or a container specific mechanism that is semantically equivalent to it. In either case, ServletContainerInitializer services from web fragment JAR files excluded from an absolute ordering must be ignored, and the order in which these services are discovered must follow the application's classloading delegation model.

