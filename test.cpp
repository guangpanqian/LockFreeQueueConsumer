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

