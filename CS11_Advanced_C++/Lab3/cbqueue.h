#ifndef CBQUEUE_H
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

#include "mbrot.h"


// TODO(WHW): Document this
class ConcurrentBoundedQueue {
public:

	ConcurrentBoundedQueue(int max_items) {
		max_items = max_items;
	}
	ConcurrentBoundedQueue(const ConcurrentBoundedQueue &&) = delete;
	ConcurrentBoundedQueue(const ConcurrentBoundedQueue &) = delete;

	void put(SP_MandelbrotPointInfo item) {
		std::unique_lock<std::mutex> lock(mutex);
		while (deque.size() == max_items) {
			wait_full.wait(lock);
		}
		deque.push_back(item);
		wait_empty.notify_one();
	}
	SP_MandelbrotPointInfo get() {
		std::unique_lock<std::mutex> lock(mutex);
		while (deque.empty()) {
			wait_empty.wait(lock);
		}
		auto item = deque.front();
		deque.pop_front();
		wait_full.notify_one();
		return item;
	}
	std::condition_variable wait_empty;
	std::condition_variable wait_full;
private:
	int max_items;
	std::deque<SP_MandelbrotPointInfo> deque;
	std::mutex mutex;
};

#define CBQUEUE_H
#endif // CBQUEUE_H
