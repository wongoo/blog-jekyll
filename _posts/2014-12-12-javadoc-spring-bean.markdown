---
author: admin
comments: true
date: 2014-12-12 08:47:13+00:00
excerpt: Javadoc翻译——Spring注解@Bean
layout: post
slug: javadoc-spring-bean
title: Javadoc翻译——Spring注解@Bean
wordpress_id: 818
categories:
- Knowledge
tags:
- annotation
- bean
- spring
---


    package org.springframework.context.annotation;
    
    @Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE})
    @Retention(RetentionPolicy.RUNTIME)
    @Documented
    public @interface Bean {
    	String[] name() default {};
    	Autowire autowire() default Autowire.NO;
    	String initMethod() default "";
    	String destroyMethod() default AbstractBeanDefinition.INFER_METHOD;
    }


该注解表明一个方法返回一个bean类并被spring容器管理.



## **1. 概览**


此注解的相关属性同xml <bean/>配置类似,例如：

    @Bean
    public MyBean myBean() {
     // instantiate and configure MyBean obj
     return obj;
    }



## **2. 指定bean名称**


当注解指定了name属性则会使用其指定的名称。这样很方便和直观！如果要求明确的指定bean，name属性是很有用的。
注意，其name可以指定一个数组，多个名称指向同一个bean实例。

    @Bean(name={"b1","b2"}) // bean available as 'b1' and 'b2', but not 'myBean'
    public MyBean myBean() {
     // instantiate and configure MyBean obj
     return obj;
    }

	 


## **3. scope, depends-on, primary, or lazy**


注意,@Bean注解没有提供scope, depends-on, primary, or lazy属性。如果要使用这些属性有单独的注解@Scope, @DependsOn, @Primary, and @Lazy。例如：

    @Bean
    @Scope("prototype")
    public MyBean myBean() {
     // instantiate and configure MyBean obj
     return obj;
    }




## **4. Inter-bean引用**


一般情况,@Bean注解在定义了@Configuration的类中使用。这种情况下，bean方法可以直接引用其他在同一个class中@Bean注解的方法。这种bean之间的引用是明确清晰、可追溯的，称之为“Inter-bean引用”，保证了符合Scope和AOP特性，就像getBean()方法查找一样。还有从基础‘Spring JavaConfig’项目特性要求在运行时将这些配置CGLIB子类化。因此，@Configuration类和其中的工厂类方法不能标记为final或者private。例如：

    @Configuration
    public class AppConfig {
     @Bean
     public FooService fooService() {
    	 return new FooService(fooRepository());
     }
     @Bean
     public FooRepository fooRepository() {
    	 return new JdbcFooRepository(dataSource());
     }
     // ...
    }

 


## **5. @Bean Lite模式**


没有被@Configuration注解的类也可以用@Bean注解其中的方法。例如用@Componet注解或POJO类。这种情况下，会通过一种叫做lite的模式处理@Bean注解的方法。
lite模式的bean方法只会被容器当作原始工厂方法（像xml中配置工厂类方法一样），其scope和lifecycle只在调用引用范围。class类也不会被修改保持原始状态，针对class类和工厂方法也不会有其他特别的约束。和@Configuration类中的bean方法对比，lite模式的bean方法不支持“Inter-bean引用”，lite模式下的方法调用只是一般的java方法调用。 Spring不会通过CBLIB代理进行注入。这和代理模式下inter-@Transactional方法的调用类似，spring不会拦截调用——spring只有在AspectJ模式下才会。
例如：

    @Component
    public class Calculator {
     public int sum(int a, int b) {
    	 return a+b;
     }
    
     @Bean
     public MyBean myBean() {
    	 return new MyBean();
     }
    }




## **6. Bootstrapping(引导启动)**


查看@Configuration Javadoc了解容器使用AnnotationConfigApplicationContext和其他相关类实现引导启动。



## **7. 返回BeanFactoryPostProcessor类型的@Bean方法**


特别要注意，返回BeanFactoryPostProcessor (BFPP)类型的@Bean方法。因为在container的lifecycle中BFPP对象需要提前初始化，它们会干扰@Configuration类中的@Autowired, @Value, and @PostConstruct这些注解的处理。为了解决这个lifecycle的问题，需将返回BFPP的@Bean方法标记为static的。例如：

    @Bean
    public static PropertyPlaceholderConfigurer ppc() {
     // instantiate, configure and return ppc ...
    }

标记为static后就无需@Configuration类初始化就可以进行调用，避免以上lifecycle冲突。但是static标记的@Bean方法则不会有scope和AOP特性了。这种方式在其他情况也是有用的，因为这些方法一般也不会被其他@Bean方法调用。另外提醒，任何非static @Bean方法返回一个BeanFactoryPostProcessor类型将会产生一个WARN警告日志。

