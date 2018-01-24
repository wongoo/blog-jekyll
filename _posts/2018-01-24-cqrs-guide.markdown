---
layout: post
author: gelnyang
comments: true
date: 2018-01-24
title: CQRS Guide
categories:
- architecture
tags:
- cqrs
- ddd
---
---


## 1. 什么是CQS?

CQRS最早来自于Betrand Meyer（Eiffel语言之父，开-闭原则OCP提出者）在 Object-Oriented Software Construction 这本书中提到的一种 [命令查询分离](http://martinfowler.com/bliki/CommandQuerySeparation.html) ([Command Query Separation](http://en.wikipedia.org/wiki/Command-query_separation),CQS) 的概念。其基本思想在于，任何一个对象的方法可以分为两大类：

- 命令(Command):不返回任何结果(void)，但会改变对象的状态。
- 查询(Query):返回结果，但是不会改变对象的状态，对系统没有副作用。

根据CQS的思想，任何一个方法都可以拆分为命令和查询两部分，比如：
```
private int i = 0;
private int Increase(int value)
{
    i += value;
    return i;
}
```
这个方法，我们执行了一个命令即对变量i进行相加，同时又执行了一个Query，即查询返回了i的值，如果按照CQS的思想，该方法可以拆成Command和Query两个方法，如下：
```
private void IncreaseCommand(int value)
{
    i += value;
}
private int QueryValue()
{
    return i;
}
```
操作和查询分离使得我们能够更好的把握对象的细节，能够更好的理解哪些操作会改变系统的状态。当然CQS也有一些缺点，比如代码需要处理多线程的情况。

## 2. 什么是CQRS?

CQRS - Command Query Responsibility Seperation,命令查询职责分离,由Greg Young在`CQRS, Task Based UIs, Event Sourcing agh!` 这篇文章中提出。`“CQRS只是简单的将之前只需要创建一个对象拆分成了两个对象，这种分离是基于方法是执行命令还是执行查询这一原则来定的(这个和CQS的定义一致)”` , 目前在DDD领域中被广泛使用。

它是一种架构风格，对软件系统的整体架构产生重要影响。

![](https://martinfowler.com/bliki/images/cqrs/cqrs.png)

## 2. CQRS对DDD的影响
DDD聚合根是非常结构化的东西，这个结构化的东西带来的一个负面的影响：大家查询的需求是非常灵活的，但聚合根的结构是固定的，如果走聚合根，你要对获取的聚合内的结构化数据进行二次的非结构化过滤和转换，这就是不必要的复杂性。

那我们为什么需要聚合根？聚合根的意义在于一致性逻辑的封装，其实只有在写入时才需要这种一致性保护，而不是读的时候。

因此，聚合根只跟写有关，那读就完全没有必要用聚合根，所以应该绕过聚合根。

因此就提出一个概念，就是我们数据的读和取要从两条路径走。这就叫做CQRS。

## 4. CQRS 优点

- 分工明确，可以负责不同的部分
- 将业务上的命令和查询的职责分离能够提高系统的性能、可扩展性和安全性。并且在系统的演化中能够保持高度的灵活性，能够防止出现CRUD模式中，对查询或者修改中的某一方进行改动，导致另一方出现问题的情况。
- 逻辑清晰，能够看到系统中的那些行为或者操作导致了系统的状态变化。
- 可以从数据驱动(Data-Driven) 转到任务驱动(Task-Driven)以及事件驱动(Event-Driven).

## 5. CQRS应用场景
- 当在业务逻辑层有很多操作需要相同的实体或者对象进行操作的时候。CQRS使得我们可以对读和写定义不同的实体和方法，从而可以减少或者避免对某一方面的更改造成冲突
- 对于一些基于任务的用户交互系统，通常这类系统会引导用户通过一系列复杂的步骤和操作，通常会需要一些复杂的领域模型，并且整个团队已经熟悉领域驱动设计技术。写模型有很多和业务逻辑相关的命令操作的堆，输入验证，业务逻辑验证来保证数据的一致性。读模型没有业务逻辑以及验证堆，仅仅是返回DTO对象为视图模型提供数据。读模型最终和写模型相一致。
- 适用于一些需要对查询性能和写入性能分开进行优化的系统，尤其是读/写比非常高的系统，横向扩展是必须的。比如，在很多系统中读操作的请求时远大于写操作。为适应这种场景，可以考虑将写模型抽离出来单独扩展，而将写模型运行在一个或者少数几个实例上。少量的写模型实例能够减少合并冲突发生的情况
- 适用于一些团队中，一些有经验的开发者可以关注复杂的领域模型，这些用到写操作，而另一些经验较少的开发者可以关注用户界面上的读模型。
- 对于系统在将来会随着时间不段演化，有可能会包含不同版本的模型，或者业务规则经常变化的系统
- 需要和其他系统整合，特别是需要和事件溯源Event Sourcing进行整合的系统，这样子系统的临时异常不会影响整个系统的其他部分。

## 6. CQRS不适应场景

- 领域模型或者业务逻辑比较简单，这种情况下使用CQRS会把系统搞复杂。
- 对于简单的，CRUD模式的用户界面以及与之相关的数据访问操作已经足够的话，没必要使用CQRS，这些都是一个简单的对数据进行增删改查。
- 不适合在整个系统中到处使用该模式。在整个数据管理场景中的特定模块中CQRS可能比较有用。但是在有些地方使用CQRS会增加系统不必要的复杂性。


## 8. AxonFramework CQRS 架构
![](http://blog.sisopipo.com/media/files/cqrs/axon-cqrs-arch.png)

项目地址:https://github.com/AxonFramework/AxonFramework

Command和Event都有对应的Handler来处理。它们具有一个共同的特征，即支持异步处理方式。这也是为何在架构中需要引入Command Bus和Event Bus的原因。在UI端执行命令请求，事实上就是将命令（注意，这是一个命令对象，你完全将其理解为Command模式的运用。注意，命令的命名一定要恰如其分地体现业务的意图）发送到Command Bus中。Command Bus更像是一个调停者（Mediator），在接收到Command时，会将其路由到准确的CommandHandler，由CommandHandler来处理该命令。在Axon Framework中，Command Bus提供了dispatch()方法对命令进行分发。也就是说，在它的实现中，并没有对Command提供异步处理，而仅仅是完成路由的功能。不过，在我自己的框架实现中，我却将Command Bus看做是消息通道，而将Command Handler看做是该消息通道的侦听者。因此，我引入了队列来实现Command Bus。很难说明哪种方式更合理，这还要取决于业务模型。整体来看，后一个方案似乎有些重型了。不过，我现在并未引入消息队列，而是使用了Scala的Actor，从多线程的角度来实现CommandBus的异步模型，也可以说得过去。

Event的处理与之相似。Axon Framework同时支持同步和异步方式。从框架角度讲，提供更多的选择是一件好事。但基于CQRS模式的核心思想来看，如果对Command(包括Event)的处理未采用异步模型，它就没有发挥出足够的优势，此时采用CQRS，反而会增加设计难度，有些得不偿失。

在Command端，基本的处理流程是由UI发起命令请求，发送到CommandBus，并由它分发给对应的Command Handler来处理命令。Command Handler会与领域对象，特别是与Aggregation Root对象通信。在处理了相关的业务逻辑后，会触发Event。一方面，它会将Event放到Event Store中；另一方面，同时会将Event发送到Event Bus，再由Event Handler处理事件。根据Axon Framework的官方文档，Event Handler会负责更新数据源，从而保证查询端能够得到最新的数据。

然而，这一过程并不是这样简单。因为整个过程可能体现的是一个状态机。Command会导致状态的迁移，并在执行Aggregate的逻辑时，触发对应的Event。Event Handler在处理事件时，并不一定是这个业务过程的终点，它可能会发送引起下一个状态迁移的命令，从而形成一个不断迁移的过程，直至业务完全结束。这就需要我们在引入CQRS时，需要改变之前的设计思路，尽量从状态迁移的角度去理解业务逻辑。UML中的状态图是一个很好的分析工具。另外，它也带来一个挑战，就是事务。因为整个过程都涉及到数据状态的变化，当某个状态迁移出现问题时，要保证数据的最终结果是一致的。Axon Framework的解决方案是引入Unit of Work模式。此外，在真正实现时，究竟是由Event Handler去更新数据源，还是交由Aggregate去完成，还有待考量。我倾向于由Aggregate委派给Repository来完成。从职责分配的角度来看，这种方式更为合理。因为与数据源打交道的逻辑绝对不能太过于分散，以免数据源的改变影响到整个领域层。在DDD中，持久逻辑都是被封装到Repository（在其内部，又会委派给基础设施层中提供数据访问的对象）。换言之，这种实践是符合DDD的设计思想的。

## 参考
- Command Query Responsibility Seperation，https://martinfowler.com/bliki/CQRS.html
- 浅谈命令查询职责分离(CQRS)模式，http://www.cnblogs.com/yangecnu/p/Introduction-CQRS.html
- CQRS, Task Based UIs, Event Sourcing agh, http://codebetter.com/gregyoung/2010/02/16/cqrs-task-based-uis-event-sourcing-agh/
- Event Sourcing, https://martinfowler.com/eaaDev/EventSourcing.html
- Apply CQRS to a Spring REST API,http://www.baeldung.com/cqrs-for-a-spring-rest-api


{{ page.date | date_to_string }},{{ page.author }}

