---
layout: post
author: gelnyang
comments: true
date: 2017-02-21
title: Spring Cloud Stream Components
categories:
- Note
tags:
- spring-cloud stream message
---
---


参考：http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/

## 1. 注解 @Output
注解接口方法，指定发布消息通道MessageChannel，例如：

```
public interface MessageSource {
    String MESSAGE_SOURCE = "message-source";

    @Output(MESSAGE_SOURCE)
    MessageChannel messageSource();
}
```
### Injecting the Bound Interfaces

For each bound interface, Spring Cloud Stream will generate a bean that implements the interface. Invoking a @Input-annotated or @Output-annotated method of one of these beans will return the relevant bound channel.

## 2. 注解 @Input
注解接口方法，指定接收消息通道，例如：

```
public interface MessageSink {
    String MESSAGE_SINK = "message-sink";

    @Output(MESSAGE_SINK)
    SubscribableChannel messageSink();
}
```

注意： 
1) 可以在一个接口的不同方法中同时注解@Input和@Output;
2) @Input和@Output的注解指定的value值为binding的名称，具体的通道名称是有binding进行指定；

## 3. 接口 Source
org.springframework.cloud.stream.messaging.Source，
提供binding名称为output的消息发布服务。

## 4. 接口 Sink
org.springframework.cloud.stream.messaging.Sink，
提供binding名称为input的消息接收服务。

## 5. 接口Processor
org.springframework.cloud.stream.messaging.Processor ， 继承了Source和Sink，同时提供消息接收和发布服务。

## 6. binding
指定消息通道信息，具体参考 BindingProperties 属性参数，主要有：
1) destination: 指定具体的通道名称
2) group: Unique name that the binding belongs to (applies to consumers only). Multiple consumers within the same group share the subscription. A null or empty String value indicates an anonymous group that is not shared.
3) contentType: 消息MIME 
4) binder: 具体服务提供者名称
5) consumer: [consumer properties](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_consumer_properties)
6) producer: [producer properties](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_producer_properties)

## 7. binder
指定消息服务提供者连接信息，具体参考 org.springframework.cloud.stream.config.BinderProperties 属性，例如：

```
spring:
  cloud:
    stream:
      bindings:
        footprintChannel:
          destination: footprint
          contentType: application/json
          group: testgroup
          binder: footprint
      binders:
        footprint:
          type: rabbit
          environment:
            spring:
              rabbitmq:
                host: ${FOOTPRINT_RMQ_HOST:192.168.2.203}
                port: ${FOOTPRINT_RMQ_PORT:5672}
                username: ${FOOTPRINT_RMQ_USERNAME:footprint}
                password: ${FOOTPRINT_RMQ_PASSWORD:footprint}
                virtual-host: ${FOOTPRINT_RMQ_VHOST:SOP}
```
binders下为binder的名称，environment下为标准的服务连接配置属性信息。

如果binding没有配置binder，则使用默认的消息服务提供者，例如：

```
spring:
  cloud:
    stream:
      bindings:
        footprintChannel:
          destination: footprint
          contentType: application/json
  rabbitmq:
    host: ${FOOTPRINT_RMQ_HOST:192.168.2.203}
    port: ${FOOTPRINT_RMQ_PORT:5672}
    username: ${FOOTPRINT_RMQ_USERNAME:footprint}
    password: ${FOOTPRINT_RMQ_PASSWORD:footprint}
    virtual-host: ${FOOTPRINT_RMQ_VHOST:SOP}
```

## 8. 注解 @EnableBinding
应用binding服务，其value值为包含 @Input 或 @Output 的接口，指定这些接口启用相关消息通道的服务。例如：

```
@EnableBinding({MessageSource.class,MessageSink.class})
```

## 9. 注解 @Gateway
org.springframework.integration.annotation.Gateway,注解一个方法，注解的方法可用于发送消息。注解配置属性：
1) requestChannel
2) replyChannel
3) requestTimeout
4) replyTimeout

注解的方法接收Message对象或payload对象，可以通过@Header或@Headers注解传递头部信息。

Indicates that a method is capable of mapping its parameters to a message or message payload. These method-level annotations are detected by the GatewayProxyFactoryBean where the annotation attributes can override the default channel settings.

A method annotated with @Gateway may accept a single non-annotated  parameter of type Message or of the intended Message payload type. Method parameters may be mapped to individual Message header values by using the @Header parameter annotation. Alternatively, to pass the entire Message headers map, a Map-typed parameter may be annotated with @Headers.
 
## 10. 注解 @MessagingGateway
org.springframework.integration.annotation.MessagingGateway，指定类为消息服务代理。例如：

```
@MessagingGateway
public interface DemoRequestGateway {
    @Gateway(requestChannel = MessageSource.MESSAGE_SOURCE)
    void generate(@Headers Map<String, Object> headers, DemoRequest request);

}
```
## 11. 注解 @InboundChannelAdapter
org.springframework.integration.annotation.InboundChannelAdapter， 注解方法，方法返回Message(或MessageSource)对象或payload。
注解需指定binding名称，以及Poller信息。

Indicates that a method is capable of producing a Message or Message payload. A method annotated with {@code @InboundChannelAdapter} can't accept any parameters.

Return values from the annotated method may be of any type. If the return value is not a Message, a Message will be created with that object as its {@code payload}. The result Message will be sent to the provided {@link #value()}.

例如:

```
@Bean
@InboundChannelAdapter(value = Source.SAMPLE, poller = @Poller(fixedDelay = "20000", maxMessagesPerPoll = "1"))
public MessageSource<String> timerMessageSource() {
    return new MessageSource<String>() {
        public Message<String> receive() {
            String value = "{\"value\":\"hi\"}";
            System.out.println("sending value: " + value);
            return MessageBuilder.withPayload(value).setHeader(CONTENT_TYPE, "application/json").build();
        }
    };
}
```

## 12. 注解 @StreamListener
org.springframework.cloud.stream.annotation.StreamListener，注解方法，注解指定binding名称，方法监听接收消息，对消息进行处理。例如：

```
@StreamListener(FootprintSink.CHANNEL_NAME)
public void process(@Headers Map<String, Object> headers, @Payload Footprint footprint) {
    //...
}
```

## 13. 注解 @SendTo
如果监听方法有返回值，可以通过@SendTo指定将返回值发布到注解指定的binding通道中。例如：

```
@StreamListener(Processor.INPUT)
@SendTo(Processor.OUTPUT)
public Bar receive(Bar barMessage) {
    barMessage.setValue(TRANSFORMATION_VALUE);
    return barMessage;
}
```

## 14. 注解 @Transformer
对于消息传递，还可以使用@Transformer注解指定输入输出的通道实现，例如：

```
@EnableBinding(Processor.class)
public class TransformProcessor {
  @Transformer(inputChannel = Processor.INPUT, outputChannel = Processor.OUTPUT)
  public Object transform(String message) {
    return message.toUpper();
  }
}
```

## 15. Other Tips
1) [Content Type and Message Converter](
http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#contenttypemanagement)
2) [Customizing message conversion](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_customizing_message_conversion)
3) [Schema-based message converters](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_schema_based_message_converters)
4) [Schema Registry Support](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_schema_registry_support)
5) [Partitioning](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_partitioning)
6) [Testing](http://docs.spring.io/spring-cloud-stream/docs/current/reference/htmlsingle/#_testing)

{{ page.date | date_to_string }},{{ page.author }}

