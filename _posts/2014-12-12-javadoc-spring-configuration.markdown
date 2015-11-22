---
author: admin
comments: true
date: 2014-12-12 07:51:51+00:00
excerpt: Javadoc翻译——Spring @Configuration注解
layout: post
slug: javadoc-spring-configuration
title: Javadoc翻译——Spring @Configuration注解
wordpress_id: 809
categories:
- Knowledge
tags:
- annotatioin
- configuration
- javadoc
- spring
---

org.springframework.context.annotation.Configuration


    @Target(ElementType.TYPE)
    @Retention(RetentionPolicy.RUNTIME)
    @Documented
    @Component
    public @interface Configuration 


此注解表明该类中定义了@Bean注解的方法，并在运行时由spring容器对这些bean生成bean定义和服务请求。

     @Configuration
     public class AppConfig {
         @Bean
         public MyBean myBean() {
             // instantiate, configure and return bean ...
         }
     }




## **1. Bootstrapping @Configuration classes (引导加载@Configuration类)**




### **1.1 使用AnnotationConfigApplicationContext的方式：**


	@Configuration类一般通过AnnotationConfigApplicationContext或子类AnnotationConfigWebApplicationContext引导加载，例如：

     AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext();
     ctx.register(AppConfig.class);
     ctx.refresh();
     MyBean myBean = ctx.getBean(MyBean.class);
     // use myBean ...

 
查看AnnotationConfigApplicationContext Javadoc了解更多，或查看AnnotationConfigWebApplicationContext了解web.xml配置方式。 



### **1.2 通过Spring XML 配置：**


	不同于直接在AnnotationConfigApplicationContext中注册，另一种选择是通过在xml中定义bean，如下：

     
        
        
     

上例中<context:annotation-config/>是必要的，用于启用ConfigurationClassPostProcessor和其他注解相关的post processors来处理@Configuration注解类。



### **1.3 通过component scanning组件扫描的方式:**


@Configuration注解本身是被@Component注解的，因此@Configuration类是可以被组件扫描到的（一般使用<context:component-scan/> xml配置），同时也可以在方法和变量上使用@Autowired/@Inject。
@Configuration类并不能仅仅使用组件扫描引导加载，需要其再使用@ComponentScan注解配置组件扫描：

     @Configuration
     @ComponentScan("com.acme.app.services")
     public class AppConfig {
         // various @Bean definitions ...
     }

查看@ComponentScan Javadoc了解更多。




## **2. Working with externalized values(使用外部值)**


	


### **2.1 Using the Environment API(使用 Environment API)**


	可以使用@Autowired或@Inject注解将Environment注入到@Configuration类中以便获得外部配置值：

     @Configuration
     public class AppConfig {
         @Inject Environment env;
    
         @Bean
         public MyBean myBean() {
             MyBean myBean = new MyBean();
             myBean.setName(env.getProperty("bean.name"));
             return myBean;
         }
     }

 
通过Environment获得属性，其内部实际包含一个或多个属性资源对象，@Configuration类可以使用@PropertySources注解添加属性资源到Environment对象中。

     @Configuration
     @PropertySource("classpath:/com/acme/app.properties")
     public class AppConfig {
         @Inject Environment env;
    
         @Bean
         public MyBean myBean() {
             return new MyBean(env.getProperty("bean.name"));
         }
     }

 
查看Environment和@PropertySource Javadoc了解更多。
 


### **2.2 Using the @Value annotation(使用@Value注解)**


	通过@Value注解将外部值注入到@Configuration类中。

     @Configuration
     @PropertySource("classpath:/com/acme/app.properties")
     public class AppConfig {
         @Value("${bean.name}") String beanName;
    
         @Bean
         public MyBean myBean() {
             return new MyBean(beanName);
         }
     }


当使用PropertySourcesPlaceholderConfigurer时(xml中通过<context:property-placeholder/>启用)，这种方式很有用。查看以下通过xml @ImportResource的方式创建@Configuration类，查看@Value、@Bean Javadoc 了解如何和BeanFactoryPostProcessor一起工作。




## **3. Composing @Configuration classes(构建@Configuration类)**




### **3.1 With the @Import annotation（使用@Import注解）**


	可以用@Import注解构建@Configuration类，但不像xml中的。因为@Configuration对象本身是作为bean实例被容器控管，引用的配置可以用@Autowired或@Inject注解注入。

     @Configuration
     public class DatabaseConfig {
         @Bean
         public DataSource dataSource() {
             // instantiate, configure and return DataSource
         }
     }
    
     @Configuration
     @Import(DatabaseConfig.class)
     public class AppConfig {
         @Inject DatabaseConfig dataConfig;
    
         @Bean
         public MyBean myBean() {
             // reference the dataSource() bean method
             return new MyBean(dataConfig.dataSource());
         }
     }

 
	那么只需在context中注册AppConfig就可以引导加载AppConfig和被引用的DatabaseConfig。
	new AnnotationConfigApplicationContext(AppConfig.class);



### **3.2 With the @Profile annotation(使用@Profile注解)**


	使用@Profile注解标记@Configuration类，则只有对应的情景模式是激活状态时才会被加载处理。

     @Profile("embedded")
     @Configuration
     public class EmbeddedDatabaseConfig {
         @Bean
         public DataSource dataSource() {
             // instantiate, configure and return embedded DataSource
         }
     }
    
     @Profile("production")
     @Configuration
     public class ProductionDatabaseConfig {
         @Bean
         public DataSource dataSource() {
             // instantiate, configure and return production DataSource
         }
     }

 
查看@Profile and Environment Javadoc了解更多。

	


### **3.3 With Spring XML using the @ImportResource annotation(使用@ImportResource加载xml配置)**



如上，@Configuration类可以以的方式定义在xml文件中，也可以用@ImportResource加载xml的配置，xml中定义的bena也可以用@Autowired或@Import注入。

     @Configuration
     @ImportResource("classpath:/com/acme/database-config.xml")
     public class AppConfig {
         @Inject DataSource dataSource; // from XML
    
         @Bean
         public MyBean myBean() {
             // inject the XML-defined dataSource bean
             return new MyBean(this.dataSource);
         }
     }

 

	


### **3.4 With nested @Configuration classes(使用内嵌的@Configuration类)**


	@Configuration类可以按照如下方式内嵌到其他配置类中：	

     @Configuration
     public class AppConfig {
         @Inject DataSource dataSource;
    
         @Bean
         public MyBean myBean() {
             return new MyBean(dataSource);
         }
    
         @Configuration
         static class DatabaseConfig {
             @Bean
             DataSource dataSource() {
                 return new EmbeddedDatabaseBuilder().build();
             }
         }
     }

 
这种方式只需注册AppConfig类到context中。因DatabaseConfig是内部类，故会自动被注册。这样就不用@Import再引入这层关系。
注意，还可以在同一个@Configuration类中使用@Profile注解声明两个不同情景模式的内部@Configuration类。




## **4. Configuring lazy initialization(配置懒加载)**


默认@Bean方法会在容器加载的时候立即初始化，可以使用@Lazy注解类表明此类的所有@Bean方法都默认懒加载。当然，@Lazy也可以单独用在@Bean方法上。




## **5. Testing support for @Configuration classes(测试支持)**


spring测试模块提供@ContextConfiguration注解引入TestContext框架，注解接收一个@Configuration类数组对象：

     @RunWith(SpringJUnit4ClassRunner.class)
     @ContextConfiguration(classes={AppConfig.class, DatabaseConfig.class})
     public class MyTests {
    
         @Autowired MyBean myBean;
    
         @Autowired DataSource dataSource;
    
         @Test
         public void test() {
             // assertions against myBean ...
         }
     }

查看TestContext框架参考文档了解更多。 




## **6. Enabling built-in Spring features using @Enable annotations(使用@Enable注解启用spring内部特性)**



Spring特性包括异步方法执行、排程任务执行、注解驱动事务管理，甚至Spring MVC，都可以在@Configuration类中通过@Enable相关的注解配置启用。查看相关注解了解更多@EnableAsync, @EnableScheduling, @EnableTransactionManagement, @EnableAspectJAutoProxy, and @EnableWebMvc。




## **7. Constraints when authoring @Configuration classes(约束条件)**







  * @Configuration类不是final的(non-final)


  * @Configuration类不能定义在方法内部(non-local) 


  * @Configuration类需有一个无参默认构造函数，不能有@Autowired注解参数的构造函数。所有内部@Configuration类必须是static的。





