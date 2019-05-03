# 项目名称 LockFreeQueueConsumer
生产者和消费者使用场景比较多，一种比较惯用的方式是使用一个队列进行加锁，来处理这种需求，然而当生产者速度比较快时，锁的竞争比较激烈。为此使用c++11原子操作实现一个无锁队列消费者，提高性能，使用模板类的方式可以兼容任意数据类型。

# 性能比较
样本数量|无锁耗时|有锁耗时
-|-|-
10000|0|0|
100000|0|0|
1000000|1|6|
10000000|20|61|
100000000|201|567|

# 用法
```c++
#include <iostream>
#include "LockFreeQueueConsumer.hpp"
using namespace std;
// 数据处理函数
void DataHandler(int & number)
{
	std::cout << number << std::endl;
}

int main()
{
	LockFreeQueueConsumer<int>lockFreeQueueConsumer;
	lockFreeQueueConsumer.BindDataDealFunc(&DataHandler);
	lockFreeQueueConsumer.StartConsuming();
	
	while (true)// 生产者生产数据 
	{
		lockFreeQueueConsumer.OnDataArrived(1);
	}
	
	return 0;
}
```

# 许可证
MIT
