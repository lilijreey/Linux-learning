### 分布式系统理论

##一致性问题

一致性从强到弱可以分为:
1. Sequential Consistency (顺序一致性)
2. Linearizabliity Consistency (线性一致性)
  ???

## 共识算法
共识算法解决分布式系统中不同节点如何达成一致

* 拜占庭错误（恶意错误)
    * POW
    * PBFT


* 非占庭错误 (故障，不响应)
    * Paxos
    * Raft

* 共识问题的理论界限
  　不存在能够保证可以达成共识的算法 FLP不可能原理

* FLP	不可能原理
 在网络可靠,存在节点失效(即便只有一个)的最小化异步模型系统
 中,不存在一个可以解决一致性问题的确定性算法


### CAP 原理
* Consistency
* Availablity
* Partition
只能同时满足两个
