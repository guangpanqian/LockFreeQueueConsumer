# 项目名称 LockFreeQueueConsumer
使用c++11原子操作实现一个无锁队列消费者

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
