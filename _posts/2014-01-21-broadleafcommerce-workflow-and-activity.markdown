---
author: admin
comments: true
date: 2014-01-21 10:15:52+00:00
excerpt: BroadleafCommerce工作流和活动
layout: post
slug: broadleafcommerce-workflow-and-activity
title: BroadleafCommerce工作流和活动
wordpress_id: 560
categories:
- Knowledge
tags:
- activity
- broadleaf
- ecommerce
- workflow
---

框架对关键的电子商务流程提供一个可配置的工作流的功能。如结算、支付、定价、购物车等的操作。这些工作流通过spring xml配置文件进行定义。一般其概况，broadleaf提供一个默认的配置，包括结算和支付这些基本的步骤，只需使用到简单的一些模块。大多数用户需要覆盖部分或全部步骤以满足实际商业需求。先讲解默认配置，再讲解如何客制化需求。



### **1.工作流概览**



每个工作流实际上是一个SequenceProcessor 的实例。它管理顺序性的流程活动，以及处理异常状况。一个工作流主要有三个配置，process context factory、activities list、rollback handler。

    
        
    
    
        
            
        
        
            
                
            
        
        
    


**1.1	ProcessContextFactory**
processContextFactory 属性必须是一个实现ProcessContextFactory接口的类。此类负责创建一个ProcessContext实例。此例SimplePaymentProcessContextFactory创建了一个SimplePaymentContext实例。

ProcessContexts一般指和某一特殊的工作流有关，所以不同的工作流会有不同的ProcessContextFactory。一个特例是购物车工作流的新增、删除、更新等操作使用同一个ProcessContext 和ProcessContextFactory 。

**1.2	Activities**
	activities 属性包含一个或个活动列表。每个活动只处理单一的工作（如计算税费、订单价格加总）。以上例子，提供了一个组合活动。组合活动可以包含一个“子工作流”，这样可以创建复杂嵌套的工作流。这个特殊的组合活动处理认证和借贷的组合功能。组合活动后面会再讲。

**1.3	Error Handler**
defaultErrorHandler属性设置异常处理器，它是一个ErrorHandler的实例。默认设置blDefaultErrorHandler，框架提供的一个简单的异常处理器，它只记录异常信息到system.out然后再抛出异常—停止工作流和当前线程正在处理的活动。稍后会再讲异常处理器。



### **2.Activity Ordering（活动顺序）**


上例中的活动配置：

    
        
    

blPaymentActivity 有一个p:order属性，这是设置此活动的顺序，框架根据此属性决定各个活动执行的先后顺序。这让框架合并活动列表到同一个bean中时非常便捷。看一个更复杂的例子：

    
    
    
    
    
    
        
            
        
        
            
                
                
                
                
            
        
        
    

在Broadleaf工作流中，每个活动顺序都会定义大于1000，这样允许你在其间插入你自己的活动。例如增加一个活动在记录促销使用情况和标记订单完成两个活动之间，可以在自己的spring配置文件applicationContext-mycompany.xml中进行如下定义：

    
        
            
        
    

**需要注意：**如果2个活动的顺序一样（例如都是3000），那么将根据所处位置进行排序。意思是根据applicationContext配置文件中的被解析合并的顺序（在web.xml中的patchConfigLocations 中定义）。整合模块定义活动顺序应该以100为间隔（如3100,3200等），这样未来还可以在其间加入一些特殊的活动。所有框架的活动可以通过bean id覆盖同时改变p:order的值以改变活动顺序。如果没有明确声明顺序的活动将在流程最后执行（默认顺序是Ordered.LOWEST_PRECEDENCE）。



### **3.Rollback Handlers(回滚处理器)**


回滚处理器为活动提供了注册状态的途径，可以在之后的某一个时间执行一个回滚动作。此例，结算流程第一步可以是一个信用卡借贷。信用卡支付后，第二步是改变购物车状态。假如此时因某些原因导致购物车状态更新失败，应有一个标准的方式将之前信用卡支付进行退还或者作废。这就是回滚处理器的作用。
回滚处理器能处理任何客户代码，也能根据传入状态进行处理。总之，回滚操作无需向支付网关一样需要一个外部服务。可以认为是一个compensating transaction（补偿事务）恢复DB到执行活动之前的状态。这是一个重要的差别，大多是工作流在JDBC事务中无法执行，因为工作流的生命周期长度让保证事务开启状态变得不切实际。既然整个工作流并没有在单一的某一个事务中，那么遇到异常的时候就不能立刻自动回滚DB状态，意味着回滚处理器需要明确的将DB设置回想要的状态。
最简单回滚处理器可通过在配置在spring活动定义中。看一个例子：

    
    
    
    
        
    
    
    
    
    
        
            
        
        
            
                
                
                
                
            
        
        
    

此例，blPaymentServiceActivity配置rollbackHandler 为blTestRollbackHandler，它将在任何子活动发送异常的时候执行。注意，一个活动发生异常时，它自己的回滚处理器不会被执行，只执行那些在工作流中已经处理过的活动的回滚处理器。如果需要，当活动失败的时候，活动有职责修正自身的状态。此例test rollback handler只是简单记录log，但在实际生产环境需要调用支付网关将支付退还或作废。

**3.1	RollbackHandler Interface(回滚处理器接口)**
所有回滚处理器实现RollbackHandler接口，它只有一个方法。

    public void rollbackState(Activity activity
    	, ProcessContext processContext
    	, Map stateConfiguration) throws RollbackFailureException;

接口参数包含所有补偿事务所需的信息，包括活动对象activity和ProcessContext以及stateConfiguration。stateConfiguration包含前面活动执行相关操作的信息，可用于反转之前的操作。stateConfiguration是活动activity执行的时候提供的，后面会再讲。

**3.2	ActivityStateManager（活动状态管理器）**
ActivityStateManager是一个灵活的组件，用来注册rollback handler回滚处理器和其他附随的状态信息。它可在工作流任何位置直接调用(Activity, Module, ErrorHandler, 等).如下获得其实例：

    ActivityStateManagerImpl.getStateManager();

ActivityStateManager接口提供多个方法注册RollbackHandler（回滚处理器）和state（状态）。实际上，你可以重载版本的registerState（其接收region参数）改善回滚行为。Region允许给一个活动多个RollbackHandler添加一个群组标签，这样提供了一个方式可选择性的回滚一部分已注册rollback handler。ActivityStateManager也提供方法清除状态或执行回滚，这些方法可以在任何时间调用执行（或者让系统在之后自动调用这些方法）。
以下实际的例子，PaymentActivity执行完后注册状态。

    if (getRollbackHandler() != null && automaticallyRegisterRollbackHandlerForPayment) {
        Map myState = new HashMap();
        if (getStateConfiguration() != null && !getStateConfiguration().isEmpty()) {
            myState.putAll(getStateConfiguration());
        }
        myState.put(ROLLBACK_ACTIONTYPE, seed.getActionType());
        myState.put(ROLLBACK_PAYMENTCONTEXT, paymentContext);
        myState.put(ROLLBACK_RESPONSEITEM, paymentResponseItem);
    
        ActivityStateManagerImpl.getStateManager().registerState(this, context, getRollbackHandler(), myState);
    }

此代码片段看是否在context配置了静态的state。接着将当前支付信息加入到state配置中。最后注册Activity、ProcessContext、RollbackHandler和state MAP对象到ActivityStateManager单例中。

**3.3	Implicit vs Explicit Registration (隐式或显式注册)**
默认需在代码中（如上例）显示注册RollbackHandler，因为RollbackHandler需要注册特殊线程状态以便获得可行的回滚状态。但对于简单的rollback handler，无需特殊线程状态完成回滚，系统可以配置为隐式（自动的）回滚注册类型。例如：

    
        
        
            
                
            
        
        
    

此例，声明了rollbackHandler和一些静态状态，让系统自动注册RollbackHandler。因为这里我们不关系线程特殊状态，注册RollbackHandler到ActivityStateManager单例中，这些就是所有需要的配置。

**3.4	Implicit vs Explicit Rollback (隐式或显式回滚)**
默认，工作流执行异常时会自动执行注册的RollbackHandler。但有时可能更希望是通过代码中通过ActivityStateManager明确的调用回滚。要启动“显式回滚”，需在runtime property文件中声明配置：

    workflow.auto.rollback.on.error=false

注意，此配置会被每个workflow工作流的autoRollbackOnError属性覆盖。



### **4.ProcessContext(执行容器)**


ProcessContext是一个容器对象，作为activity的传入传出参数。它一般包含workflow工作流相关信息。支付workflow工作流用SimplePaymentContext，它包含PaymentSeed。而PaymentSeed中包含多个PaymentInfo、Order和PaymentResponse这些处理订单请求的对象。另外 ProcessContext提供stopProcess和isStopped方法设置和检查工作流状态。调用stopProcess可不用抛出异常停止工作流继续执行。

    public class SimplePaymentContext implements ProcessContext {
    
        public final static long serialVersionUID = 1L;
    
        private boolean stopEntireProcess = false;
        private PaymentSeed seedData;
    
        public void setSeedData(Object seedObject) {
            this.seedData = (PaymentSeed) seedObject;
        }
    
        public boolean stopProcess() {
            this.stopEntireProcess = true;
            return stopEntireProcess;
        }
    
        public boolean isStopped() {
            return stopEntireProcess;
        }
    
        public PaymentSeed getSeedData() {
            return seedData;
        }
    
    }


**4.1	Conditional Activity Execution(活动条件执行)**
Activity接口根据ProcessContext中的信息决定是否要跳过执行当前活动。Activity再调用前都会调用此方法进行判断，避免工作流重复定义，因为或许一个较大的工作流配置使用同一个ProcessContext的多个配置。



### **5.Activities(活动)**


一个活动是Activity接口的实例，提供执行活动和获取error handler的方法。大多活动对象继承BaseActivity虚类。如PaymentActivity，可获得PaymentContext对象，也可加入到PaymentService中完成支付交易。
一个Java范型定义活动只针对特定的工作流（及只能在特定的ProcessContext中执行），例如下面这个针对blPricingWorkflow工作流的活动:

    public class TotalActivity extends BaseActivity {
    
        @Override
        public PricingContext execute(PricingContext context) throws Exception {
            Order order = context.getSeedData();
            //compute all totals for the order
            return context;
    
        }
    }


针对blCheckoutWorkflow工作流的活动:

    public class CompleteOrderActivity extends BaseActivity {
    
        @Override
        public CheckoutContext execute(CheckoutContext context) throws Exception {
            CheckoutSeed seed = context.getSeedData();
    
            seed.getOrder().setStatus(OrderStatus.SUBMITTED);
            seed.getOrder().setOrderNumber(new SimpleDateFormat("yyyyMMddHHmmssS").format(SystemTime.asDate()) + seed.getOrder().getId());
            seed.getOrder().setSubmitDate(Calendar.getInstance().getTime());
    
            return context;
        }
    
    }

组合活动也是继承于BaseActivity，所以也可加入到工作流活动list列表中。但他们配置上有差异，组合活动需配置一个子工作流。组合活动由父工作流调用，传递到子工作流，子工作流的活动也会依次被调用。所有子工作流和所有异常都绑定到同一个ProcessContext对象，调用stopProcess方法将停止所有嵌套的工作流。使用组合活动，可以方便实现复杂嵌套的工作流。



### **6.Error Handlers(错误处理器)**


Error handlers错误处理器都实现ErrorHandler接口。但发送异常时，可以通过Error handlers做一些必要的事情。如记录异常日志，或释放资源等。之前提过，所有工作流默认使用DefaultErrorHandler，它只是将异常信息输出到System.out后再抛出异常。

    public interface ErrorHandler extends BeanNameAware {
        public void handleError(ProcessContext context, Throwable th) throws WorkflowException;
    }





### **7.Removing a Broadleaf Workflow(删除工作流)**


如果因为需要扩展功能而需要删除框架默认定义的工作流，可通过定义一个EmptySequenceProcess实例的工作流覆盖默认配置（但我们不建议这样做）。如因你已继承了OrderService覆盖了performCheckout()方法，你想去掉blCheckoutWorkflow工作流，可以通过如下定义去掉：

    




### **8.Provided Workflows(已提供的工作流)**


以下是一些框架提供的工作流：

blAddItemWorkflow 添加商品到购物车流程
blUpdateItemWorkflow 更新购物车商品流程
blRemoveItemWorkflow 删除购物车商品流程
blPricingWorkflow 被 blPricingService(被OrderService调用)使用 来计算订单价格
blCheckoutWorkflow 订单中被blCheckoutService 调用以完成订单结算（支付、标记减少存货、改变状态为SUBMITTED等等）
blPaymentWorkflow blCheckoutWorkflow 中的 CompositeActivity ，其运行多种支付方式

订单支付有多种发生情况也有多种支付方式。blPaymentWorkflow 是一个活动组件，可将默认的支付配置流程进行包装（即blAuthorizeAndDebitWorkflow）。一般情况，如果在订单送货之前无需支付，可将blPaymentWorkflow 的workflow属性改为blAuthorizeWorkflow ，它只会简单的通过支付提供者对客户的支付信息进行认证。当到快递包装的时候再执行blDebitWorkflow完成支付。支付相关流程列举如下：
[![broadleaf_framework_workflow](http://sisopipo.com/blog/media/files/2014/01/broadleaf_framework_workflow.jpg)](http://sisopipo.com/blog/media/files/2014/01/broadleaf_framework_workflow.jpg)



### **9.参考**


1. [Workflows and Activities](http://docs.broadleafcommerce.org/core/current/broadleaf-concepts/workflows-and-activities)
2.[bl-framework-applicationContext-workflow.xml](https://github.com/BroadleafCommerce/BroadleafCommerce/blob/develop/core/broadleaf-framework/src/main/resources/bl-framework-applicationContext-workflow.xml )
