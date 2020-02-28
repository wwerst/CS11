#ifndef CBQUEUE_H
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>


// TODO(WHW): Document this
class ConcurrentBoundedQueue {
public:

	ConcurrentBoundedQueue(int max_items) {
		deque.resize(max_items);
	}
	ConcurrentBoundedQueue(const ConcurrentBoundedQueue &&) = delete;
	ConcurrentBoundedQueue(const ConcurrentBoundedQueue &) = delete;

	void put(SP_MandelbrotPointInfo item) {
		std::lock_guard<std::mutex> guard(mutex);
		deque.push_back(item);
		wait_empty.notify_one();
	}
	SP_MandelbrotPointInfo get() {
		std::lock_guard<std::mutex> guard(mutex);
		auto item = deque.front();
		deque.pop_front();
		wait_full.notify_one();
		return item;
	}
	std::condition_variable wait_empty;
	std::condition_variable wait_full;
private:
	std::deque<SP_MandelbrotPointInfo> deque;
	std::mutex mutex;
};

#define CBQUEUE_H
#endif // CBQUEUE_H
