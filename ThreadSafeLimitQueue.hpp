#include <queue>
#include <mutex>

template<class T>
class ThreadSafeLimitQueue
{
public:
	ThreadSafeLimitQueue();
	virtual ~ThreadSafeLimitQueue();
	void SetCapacity(int capacity);
	void push(const T&data);
	void pop();
	T& front();
	bool empty();
private:
	int m_Capacity;
	std::mutex m_Mutex;
	std::queue<T> m_Queue;
};

template<class T>
ThreadSafeLimitQueue<T>::ThreadSafeLimitQueue()
	:m_Capacity(1000)
{
}

template<class T>
ThreadSafeLimitQueue<T>::~ThreadSafeLimitQueue()
{
}

template<class T>
void ThreadSafeLimitQueue<T>::SetCapacity(int capacity)
{
	m_Capacity = capacity;
}

template<class T>
void ThreadSafeLimitQueue<T>::push(const T&data)
{
	std::unique_lock<std::mutex>lock(m_Mutex);
	if (m_Queue.size() >= m_Capacity)
	{
		std::cout << "The queue is full which capacity is " << m_Capacity << std::endl;
		m_Queue.pop();
	}
	m_Queue.push(data);
}

template<class T>
void ThreadSafeLimitQueue<T>::pop()
{
	std::unique_lock<std::mutex>lock(m_Mutex);
	m_Queue.pop();
}

template<class T>
T& ThreadSafeLimitQueue<T>::front()
{
	std::unique_lock<std::mutex>lock(m_Mutex);
	return m_Queue.front();
}

template<class T>
bool ThreadSafeLimitQueue<T>::empty()
{
	std::unique_lock<std::mutex>lock(m_Mutex);
	return m_Queue.empty();
}
