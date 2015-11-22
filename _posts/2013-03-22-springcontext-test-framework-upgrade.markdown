---
author: admin
comments: true
date: 2013-03-22 15:05:12+00:00
excerpt: SpringContext测试框架升级，AbstractDependencyInjectionSpringContextTests.java升级使用AbstractTransactionalJUnit4SpringContextTests.java
layout: post
slug: springcontext-test-framework-upgrade
title: SpringContext测试框架升级
wordpress_id: 411
categories:
- Experience
tags:
- junit
- spring
- test
---

原本我们有一个基于Spring的数据库Manager测试基础类BaseDBTestCase.java，其继承于 AbstractDependencyInjectionSpringContextTests.java（基于junit3.8），所有其他测试类继承于BaseDBTestCase.java。Spring 3.0之后弃用了AbstractDependencyInjectionSpringContextTests，提供了一个新的测试框架AbstractTransactionalJUnit4SpringContextTests(基于junit4)，支持注解功能，通过注解可以创建spring context、声明注入、声明测试方法、声明测试前/后执行方法等。为了升级使用AbstractTransactionalJUnit4SpringContextTests，做了以下改动：

**首先，**原来规定测试类中所有需要测试方法名都以“test”开头，junit3会自动识别并执行。AbstractTransactionalJUnit4SpringContextTests并没有继承TestCase，并不会自动识别方法名以“test”开头的测试方法。可以通过为测试方法添加“@org.junit.Test”注解标明此方法是测试类。

**其次，**BaseDBTestCase.java原本继承于TestCase，声明了onSetUp和onTearDown两个方法，方法中主要使用dbunit工具做一些初始化DB测试数据和清除测试数据的工作，执行测试方法前后会自动调用这两个方法。修改为继承AbstractTransactionalJUnit4SpringContextTests后，由于此类并没有继承TestCase类，不会自动调用onSetUp和onTearDown方法。但可以通过为onSetUp方法添加“@org.junit.Before”注解，为onTearDown方法添加“@org.junit.After”注解达到同样的效果。

**第三，**由于没有继承于TestCase，也就没有Assert类中的断言方法，但不要紧，Assert类中的断言方法都是静态方法，在调用的时候加上Assert类名即可。

**第四，**原来AbstractDependencyInjectionSpringContextTests.java类会在启动测试前通过getConfigLocations()方法获取spring 配置文件来创建SprintContext。原来每个测试类会有一个单独的spring配置文件，此spring文件的位置和命名规则有明确规定，BaseDBTestCase.java覆盖了此方法，返回指定的spring配置文件和固定的DB连接的spring配置文件数组，子类无需关心SpringContext的创建，只需按照规则创建好Spring配置文件即可。 BaseDBTestCase.java改为继承AbstractTransactionalJUnit4SpringContextTests.java后，初始化SpringContext的方式改变为通过注解声明spring配置文件位置，而且父类和子类的注解声明的spring配置文件同时生效。在BaseDBTestCase.java中添加注解声明公共使用的连接DB的spring配置文件@ContextConfiguration(locations = { "/test/spring/springContext-db.xml" })， 而在具体的测试子类中添加该子类所需的spring配置文件的注解声明，如@ContextConfiguration(locations = { "springContext-personManagerImpl.xml" }) （如果location地址不是以“/”开头，则默认当前类package路径）。注解配置起来相等方便，子类的spring配置文件的命名、位置、数量不再受约束。

**第五，**原来AbstractDependencyInjectionSpringContextTests.java类会根据变量名称自动将所需的bean注入到测试类中，而AbstractTransactionalJUnit4SpringContextTests.java需要对所需注入的变量添加@javax.annotation.Resource注解。虽然不智能，但让注入动作更明确。

**总结：**如果按照原来的测试类编写规范写测试类，此次升级的意义并不明显。但升级使用junit4后，使用注解的方式配置调度测试，代码量减少，更加灵活，更方便扩展。

