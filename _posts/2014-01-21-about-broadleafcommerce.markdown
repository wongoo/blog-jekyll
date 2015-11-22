---
author: admin
comments: true
date: 2014-01-21 07:58:31+00:00
excerpt: 认识Java电子商务开源框架BroadleafCommerce
layout: post
slug: about-broadleafcommerce
title: 认识Java电子商务开源框架BroadleafCommerce
wordpress_id: 553
categories:
- Knowledge
tags:
- broadleaf
- ecommerce
- introduction
---

### **1. What's BroadleafCommerce**


	BroadleafCommerce是一个Java开源电子商务网站框架。其目标是开发企业级商务网站，它提供健壮的数据和服务模型、富客户端管理平台、已经一些核心电子商务有关的工具。



### **2.Features**



2.1	Catalog （目录分类）
提供灵活的产品和类型管理，一个重要的特性是可以继承产品分类来满足特殊的商业需求。
管理界面可以管理各种类别和产品。

2.2	Promotion System（促销系统）

可通过配置的方式管理促销。
以下类促销示无需客制化而通过管理界面即可管理：
	百分比折扣、金额折扣、固定价格(Percent Off / Dollar Off / Fixed Price)
	订单、订单项、快递级别促销
	买一赠一促销
	基于客户、购物车或类别属性的促销

2.3	Content Management System（内容管理系统）
内容管理系统有以下特性：
	支持用户直接管理静态页面
	可以配置顾客内容类型（如广告）
	提供UI界面管理静态页面、结构化内容、图片以及其他内容；
	结构化内容能够针对性的对某些客户显示（如对满足一定条件的客户显示广告）



### **3.Architecture**



3.1	Spring Framework
Spring提供诸多功能，包括依赖注入和事务管理

3.2	Security
Spring Security提供强健的安全认证框架，控制代码和页面级别的认证和授权。

3.3	Persistence
使用JPA和hibernate实现ORM基础

3.4	Asynchronous Messaging
使用spring JMS和一个现代的JMS代理交互来实现应用消息的异步处理。

3.5	Search
通过整合流行的Compass和lucene项目提供可灵活的domain查找功能。

3.6	Task Scheduling
使用Quartz提供排程功能。

3.7	Email
Email功能分为同步和异步（jms）两种模式。Email内容可以通过velocity模块客制化。支持mail打开和连接点击跟踪。

3.8	Modular Design（模块化设计）
提供各种模块，可以和电子商务的一些重要功能进行交互，如信用卡处理、税收服务、快递公司。
比如，USPS快递模块是一个好的案例。 客户模块可以很方便的开发并整合进来。

3.9	Configurable Workflows（可配置的工作流）
电子商务生命周期的关键表现在可配置的工作流。系统能够对这些关键的地方进行完全的控制，包括价格和结账，允许对订单、行为和客户执行模块进行操作。支持复杂内嵌行为的合成工作流。

3.10	Extensible Design（可扩展性设计）
扩展性是我们设计的核心，几乎broadleaf所有的组件都是可以继承、或添加、或者通过修改增强和改变默认的行为。 这些组件包括所有的service、数据访问对象、实体。

3.11	Configuration Merging（配置合并）
我们以扩展模块的附加部分，为客户提供对spring配置文件进行合并的功能。它可以最小化配置，一个实现必须清楚它允许用户只需把精力放在他们自己的配置细节。 Broadleaf在运行时会智能的将实现者的配置信息和自己的配置信息进行合并。

3.12	Runtime Configuration Management（运行时配置管理）
services、模块和其他子系统的配置属性通过JMX暴露，这样管理者不用关闭系统就可以改变应用行为。

3.13	Presentation Layer Support（表现层支持）
提供很多事先写好的spring MVC控制器来加快表现层的开发。

3.14	QoS（服务质量）
提供对客户和默认模块的服务质量监控，同时支持外部日志和email。其他客户Qos处理器可以通过我们的open API添加。

3.15	PCI Considerations（PCI注意事项）
我们的架构和设计经过了仔细的分析，帮助你在决定存储和使用敏感的客户金融账号信息的时候实现PCI遵从性。支付账号信息是分别引用的，允许你将机密的数据隔离存储到一个独立的安全的数据库平台。已经添加了API方法来包含PCI遵从性加密schema。另外，提供冗长的日志跟踪交易交互信息。

PCI（Payment Card Industry）(Payment Card Industry (PCI) Data Security Standard).支付卡行业 (PCI) 数据安全标准 (DSS)是一组全面的要求，旨在确保持卡人的信用卡和借记卡信息保持安全，而不管这些信息是在何处以何种方法收集、处理、传输和存储。

PCI DSS 由 PCI 安全标准委员会的创始成员（包括 American Express、Discover Financial Services、JCB、MasterCard Worldwide 和 Visa International）制定，旨在鼓励国际上采用一致的数据安全措施。
PCI DSS 中的要求是针对在日常运营期间需要处理持卡人数据的公司和机构提出的。具体而言，PCI DSS 对在整个营业日中处理持卡人数据的金融机构、贸易商和服务提供商提出了要求。PCI DSS 包括有关安全管理、策略、过程、网络体系结构、软件设计的要求的列表，以及用来保护持卡人数据的其他措施。

3.16	Customizable Administration Platform （客制化管理平台）
管理应用基于我们新的开放的管理平台，使用标准面向对象的技术提供一个清晰的客制化方式。管理平台和核心框架一样，都有很好扩展性。表现层是基于有名的可信赖的GWT和SmartGWT技术。



### **4.Reference**


4.1.[get start](http://docs.broadleafcommerce.org/core/current/getting-started) 
4.2.[Framework source code](https://github.com/BroadleafCommerce/BroadleafCommerce):
4.3.[Switching to MySQL](http://docs.broadleafcommerce.org/core/current/tutorials/getting-started-tutorials/switch-to-mysql-tutorial)
4.4.[Customize the UI](http://docs.broadleafcommerce.org/core/current/tutorials/getting-started-tutorials/customize-ui-for-heat-clinic-tutorial)(客制化UI界面)
4.5.Configure Checkout(配置结账)
4.6.[Production Considerations](http://docs.broadleafcommerce.org/core/current/appendix/production-considerations)
4.7.[Storing additional customer properties](http://docs.broadleafcommerce.org/core/current/tutorials/getting-started-tutorials/adding-customer-attribute-tutorial)
4.8.[Extend the Customer entity](http://docs.broadleafcommerce.org/core/current/tutorials/getting-started-tutorials/extending-customer-for-heat-clinic-tutorial)
4.9.[Hook into the order submit workflow](http://docs.broadleafcommerce.org/core/current/tutorials/getting-started-tutorials/order-submit-workflow-for-heat-clinic-tutorial)
4.10.[Hook into the add to cart workflow](http://docs.broadleafcommerce.org/core/current/tutorials/getting-started-tutorials/add-to-cart-workflow-for-heat-clinic-tutorial)
