# 项目名称 LockFreeQueueConsumer
使用c++11原子操作实现一个无锁队列消费者

# 用法
```
#include <iostream>
#include "LockFreeQueueConsumer.hpp"
using namespace std;

void DataHandler(int & number)
{
	std::cout << number << std::endl;
}

int main()
{
	LockFreeQueueConsumer<int>lockFreeQueueConsumer;
	lockFreeQueueConsumer.BindDataDealFunc(&DataHandler);
	lockFreeQueueConsumer.StartConsuming();
	while (true) 
	{
		lockFreeQueueConsumer.OnDataArrived(1);
	}
	return 0;
}
```
# 许可证
MIT
