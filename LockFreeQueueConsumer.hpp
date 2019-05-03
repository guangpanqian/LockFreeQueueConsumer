/***************************************************************************
 * CopyRight (C)
 * Author: Qianguangpan
 * Description: Design a consumer with queue which lock free
 *
 ***************************************************************************/
#include <atomic>
#include <thread>
#include <queue>
#include <functional>

template <class T>
class LockFreeQueueConsumer
{
public:
	LockFreeQueueConsumer();
	virtual ~LockFreeQueueConsumer();
	//	绑定一个回调，用以处理收到的数据
	void BindDataDealFunc(std::function<void(T&)>dataDealFunc);
	//	启动消费线程
	void StartConsuming();
	//	数据到来时调用此函数
	void OnDataArrived(const T& data);
private:
	static void ConsumeData(void *param);
	void ConsumerDataImp();
	//	关闭消费线程
	void StopConsuming();

	//	标识当前正在写入的队列的编号
	std::atomic<int> m_WriteQueueIndex;
	//	两个队列，生产者和消费者分别使用其中一个，避免竞争
	std::queue<T> m_Queues[2];
	//	消费线程句柄
	std::thread *m_ThrConsume;
	//	标识当前线程是否退出
	std::atomic<bool>m_Exit;
	//	数据处理回调
	std::function<void(T&)>m_DataDealFunc;
};

template <class T>
LockFreeQueueConsumer<T>::LockFreeQueueConsumer()
	:m_ThrConsume(nullptr)
	, m_WriteQueueIndex(0)
	, m_Exit(false)
	, m_DataDealFunc(nullptr)
{
}


template <class T>
LockFreeQueueConsumer<T>::~LockFreeQueueConsumer()
{
	StopConsuming();
}

template <class T>
void LockFreeQueueConsumer<T>::BindDataDealFunc(std::function<void(T&)>dataDealFunc)
{
	m_DataDealFunc = dataDealFunc;
}

template<class T>
void LockFreeQueueConsumer<T>::StartConsuming()
{
	m_ThrConsume = new std::thread(&ConsumeData, this);
}

template<class T>
void LockFreeQueueConsumer<T>::StopConsuming()
{
	if (!m_Exit)
	{
		m_Exit = true;
		if (nullptr != m_ThrConsume)
		{
			if (m_ThrConsume->joinable())
				m_ThrConsume->join();
			delete m_ThrConsume;
			m_ThrConsume = nullptr;
		}
	}
}

template <class T>
void LockFreeQueueConsumer<T>::ConsumeData(void *param)
{
	LockFreeQueueConsumer<T> *consumer = (LockFreeQueueConsumer<T>*)param;
	consumer->ConsumerDataImp();
}

template <class T>
void LockFreeQueueConsumer<T>::OnDataArrived(const T& data)
{
	m_Queues[m_WriteQueueIndex].push(data);
}

template <class T>
void LockFreeQueueConsumer<T>::ConsumerDataImp()
{
	while (!m_Exit)
	{
		int  readQueueIndex = m_WriteQueueIndex;
		//如果当前这个队列是空的
		if (m_Queues[readQueueIndex].empty())
		{
			//1.所有的数据都消费完：释放时间片，让生产者继续生产；
			if (m_Queues[readQueueIndex ^ 1].empty())
			{
				if (m_Exit)
					break;
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				continue;
			}
			//2.数据在另外的队列里：直接取另外队列的数据消费即可，生产者继续往当前的队列中生产；
			else
			{
				readQueueIndex ^= 1;
			}
		}
		//当前这个队列有数据，此时消费者占用这个队列，让生产者往另外的队列里生产
		else
		{
			atomic_fetch_xor(&m_WriteQueueIndex, 1);
		}
		//将当前占用的队列的数据消费完
		while (!m_Queues[readQueueIndex].empty())
		{
			T& data = m_Queues[readQueueIndex].front();

			if (m_DataDealFunc)
				m_DataDealFunc(data);

			m_Queues[readQueueIndex].pop();

			if (m_Exit)
				break;
		}
	}
}

