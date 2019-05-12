#include <iostream>
#include "LockFreeQueueConsumer.hpp"
#include <thread>
#include <chrono>

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
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}

