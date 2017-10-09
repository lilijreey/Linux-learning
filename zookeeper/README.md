## zookeeper


## 常用命令
zkClient.sh
zkServer.sh

## session的状态变化与生命期
not-connected
connecting
connected
close

* session-timeout
  在创建Session时，需要设置Session Timeout这个重要参数。
  这是Zookeeper服务允许一个Session在定义它失效之前的时间。
  如果服务在时间t内不能看到与一个Session关联的消息，它将定义这个Session失效。
  如果客户端在1/3 t时间内没有听到任何从服务器过来的消息，它将发送一个心跳消息给服务器。
  在(2/3)t时间， Zookeeper客户端开始寻找另一个Zookeeper服务器，
  并且它有另外的(1/3)t的时间寻找。


[info](http://blog.csdn.net/jeff_fangji/article/details/43916359)


### watcher
订阅者模式
watcher在server中运行
* event类型，当session在不同状态时，event也会不同
 CREATE-EVENT 
 DELETE-EVENT
 CHANGED-EVENT
 CHILD-EVENT
 SESSION-EVENT

* session timeout
* session loss
* session expired

C bind 提供了两种watcher的使用模式，
1. 所有wather回调公用一个回调函数. 在zookeeper-init 中设置
2. 使用w开头的设置函数
   为每个wathcer指定回调函数，和上文


