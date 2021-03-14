#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <cassert>

class ThreadPool {
public:
    ThreadPool(ThreadPool const &) = delete;
    void operator=(ThreadPool const &) = delete;

    static ThreadPool & GetInstance() {
        static ThreadPool instance;
        return instance;
    }

    void AllocateTask(std::function<void()> const &task) {
        {
            // Acquire a std::mutex (typically via std::lock_guard).
            // scoped_lock is strictly superior to the old lock_guard.
            std::scoped_lock<std::mutex> lock(_lock);

            // Perform the modification while the lock is held:
            _taskQueue.push(task);
            // Condition has changed.
        }

        // Note that the notifying thread is not required to own the mutex when calling a notification function.
        // In fact, the first thing any unblocked thread does is attempt to lock the mutex,
        // so releasing ownership prior to notification may actually improve performance.
        _condition.notify_one();  // unblocks only a single thread.
    }

private:

    ThreadPool() : _shouldTerminatePool(false) {

        // The static std::thread::hardware_concurrency() function returns the number of concurrent threads
        // (or an approximation thereof ) supported by the current hardware, or zero if this cannot be determined.
        // This number may be larger than the number of physical cores:
        // if the hardware, for instance, supports simultaneous multithreading (branded by Intel as Hyper-Threading),
        // this will be an even multiple of (typically twice) the number of cores.
        auto numThreads = std::thread::hardware_concurrency();
        assert(numThreads != 0);

        for (size_t i = 0; i < numThreads; i++)
            _workers.emplace_back(&ThreadPool::InfiniteLoop, this);
    }

    void InfiniteLoop() {
        std::function<void()> task;

        while (true) {
            {
                // A thread must first lock the corresponding mutex.
                // Acquire a std::unique_lock<std::mutex>,
                // on the same mutex as used to protect the shared variable:
                std::unique_lock<std::mutex> lock(_lock);

                // Wait using a condition_variable:

                // As wait() blocks the thread, it also unlocks the mutex:
                // this allows other threads to lock the mutex
                // in order to satisfy the shared condition.
                _condition.wait(lock, [this]() {
                    // When a waiting thread is woken up,
                    // before returning from wait(),
                    // it always first locks the mutex again using the unique_lock,
                    // making it safe to recheck the condition.
                    return !_taskQueue.empty() || _shouldTerminatePool;  // ready?
                });  // is equivalent to:
                // while (!ready) _condition.wait(lock);
                // while (_taskQueue.empty() && !_shouldTerminatePool) _condition.wait(lock);


                if (_shouldTerminatePool && _taskQueue.empty())
                    return;

                task = _taskQueue.front();
                _taskQueue.pop();
            }

            task();
        }
    }

    ~ThreadPool() {
        Terminate();
        for (auto &_worker : _workers)
            _worker.join();
    }

    void Terminate() {
        {
            std::scoped_lock<std::mutex> lock(_lock);
            _shouldTerminatePool = true;
            // Condition has changed.
        }
        // Execute notify_all on the std::condition_variable
        // (the lock does not need to be held for notification):
        _condition.notify_all();  // unblocks all threads waiting on a condition variable.
        // The order in which multiple waiting threads are woken up is unspecified.
    }

    std::vector<std::thread> _workers;

    std::queue<std::function<void()>> _taskQueue;

    std::mutex _lock;
    std::condition_variable _condition;

    bool _shouldTerminatePool;
};

#endif /* ThreadPool_hpp */
