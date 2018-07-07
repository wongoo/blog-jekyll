---
author: 望哥
comments: true
date: 2018-07-07
title: aws sqs lambda 范例
categories:
- 经验
tags:
- AWS
- SQS
- Lambda

---

最近AWS Lambda增加了SQS事件触发器, 由于团队中大部分服务都以lambda的形式提供服务，
没有SQS触发器之前，接收处理事件只能交给EC2独立部署服务去处理；而现在则可以直接使用lambda来处理事件了。
于是就想整理一下使用范例，供团队成员使用。

# 1.发送 SQS 消息




## 1.1 建立SQS连接

```
func SqsConnect(key, secret string) *sqs.SQS {
   sess, _ := session.NewSession(&aws.Config{
      Region:      aws.String(endpoints.CnNorth1RegionID),
      Credentials: credentials.NewStaticCredentials(key, secret, "")},
   )
   _, err := sess.Config.Credentials.Get()
   if err != nil {
      fmt.Println("config Credentials err:", err.Error())
      return nil
   }
   return sqs.New(sess)
}


svc := sqsutil.SqsConnect(*accessKeyId, *accessKeySecret)
```


## 1.2 获得队列地址

```
qUrl, err := sqsutil.SqsQueueUrl(svc, *qName)
if err != nil {
   exitErrorf(err.Error())
}
fmt.Println("queue url:", *qUrl)
```


## 1.3 构造发送消息体

```
func NewSqsEntry(msg string) *sqs.SendMessageBatchRequestEntry {
   id, _ := uuid.NewV4()
   message := &sqs.SendMessageBatchRequestEntry{
      Id: aws.String(id.String()),
      MessageAttributes: map[string]*sqs.MessageAttributeValue{
         "contentType": &sqs.MessageAttributeValue{
            DataType:    aws.String("String"),
            StringValue: aws.String("application/json"),
         },
         "Author": &sqs.MessageAttributeValue{
            DataType:    aws.String("String"),
            StringValue: aws.String("test"),
         },
      },
      MessageBody: aws.String(string(msg)),
   }
   fmt.Println("new sqs request:", id)
   return message
}


request := &sqsutil.StockRecycleRequest{
   ProductId: "product_1",
   OrderId:   "order_1",
}
b, _ := json.Marshal(request)
msg := string(b)

msgList := []*sqs.SendMessageBatchRequestEntry{
   NewSqsEntry(msg),
}
```


## 1.4 发送消息

```
func SqsSendBatch(sqsSvc *sqs.SQS, entries [] *sqs.SendMessageBatchRequestEntry, queueUrl *string) {
   params := &sqs.SendMessageBatchInput{
      Entries:  entries,
      QueueUrl: queueUrl,
   }

   sendResult, err := sqsSvc.SendMessageBatch(params)

   if err != nil {
      glog.Errorf("Error", err)
   }

   glog.Infof("Send SQS   batch successful : %v", len(sendResult.Successful))
   glog.Infof("Send SQS   batch failed : %v", len(sendResult.Failed))
}


SqsSendBatch(svc, msgList, qUrl)
```

# 2. Lambda 接收处理消息

注意: 配置lambda SQS时，所选角色需要有 SQS ReceiveMessage,DeleteMessage权限，可以为角色直接添加 AWSLambdaSQSExecutionRole 权限策略.

```
import (
   "fmt"
   "github.com/aws/aws-lambda-go/events"
   "context"
   "github.com/aws/aws-lambda-go/lambda"
)

func main() {
   lambda.Start(handler)
}

func handler(ctx context.Context, sqsEvent events.SQSEvent) error {
   for _, message := range sqsEvent.Records {
      fmt.Printf("The message %s for event source %s = %s \n", message.MessageId, message.EventSource, message.Body)
   }

   return nil
}
```

CloundWatch 日志：

```
09:56:05 The message d8df2f61-e163-4987-98d8-330cfc7563d9 for event source aws:sqs = {"product_id":"product_1","order_id":"order_1"}
```


# 3.参考

- golang 发送 SQS 消息范例, [link](https://github.com/wongoo/go-lambda-example/blob/master/sqssender/sqssender.go)
- golang lambda 接收处理 SQS 消息代码范例: [link](https://github.com/wongoo/go-lambda-example/blob/master/sqslambda/sqslambda.go)

{{ page.date | date_to_string }},{{ page.author }}