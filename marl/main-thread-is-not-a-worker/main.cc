#include <iostream>
#include <cstdlib>

#include "marl/defer.h"
#include "marl/event.h"
#include "marl/scheduler.h"
#include "marl/waitgroup.h"

int main(int argc, char* argv[])
{
    // В приложении будет 3 потока: один главный и два рабочих.
    marl::Scheduler::Config workerThreads2;
    workerThreads2.setWorkerThreadCount(2);
    assert(2 == workerThreads2.workerThread.count);

    marl::Scheduler scheduler(workerThreads2);
    scheduler.bind();
    defer(scheduler.unbind());

    marl::Event evt(marl::Event::Mode::Manual);

    marl::schedule([evt] {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "thread " << this_id << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        evt.wait();
        std::cout << "end;" << std::endl;
    });

    marl::schedule([] {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "thread " << this_id << std::endl;
        while (true) {}
    });

    // Этот task запустится в первом рабочем потоке спустя 10 с,
    // когда в первом будет вызов wait.
    marl::schedule([] {
        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "thread " << this_id << std::endl;
        while (true) {}
    });

    // Тут будет пауза, но главный поток не пойдет выполнять запланированные задачи.
    evt.wait();

    std::cout << "end." << std::endl;
    return EXIT_SUCCESS;
}
