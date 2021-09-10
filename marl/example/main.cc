#include "marl/defer.h"
#include "marl/event.h"
#include "marl/scheduler.h"
#include "marl/waitgroup.h"

#include <cstdio>

int main()
{

  // Create one instance of marl::Scheduler, use it for the lifetime of the process.
  // The marl::Scheduler constructor can be expensive as it may spawn a number of hardware threads.
  // Multiple marl::Schedulers may fight each other for hardware thread utilization.
  // For these reasons, it is recommended to create a single marl::Scheduler for the lifetime of your process.

  marl::Scheduler scheduler(marl::Scheduler::Config::allCores());  // create a marl scheduler using all the logical processors available to the process.


  // Bind the scheduler to externally created threads.
  // In order to call marl::schedule() the scheduler must be bound to the calling thread.
  // Failure to bind the scheduler to the thread before calling marl::schedule() will result in undefined behavior.
  // marl::Scheduler may be simultaneously bound to any number of threads,
  // and the scheduler can be retrieved from a bound thread with marl::Scheduler::get().

  scheduler.bind();  // bind this scheduler to the main thread so we can call marl::schedule().


  // Always remember to unbind the scheduler before terminating the thread.
  // Forgetting to unbind will result in the marl::Scheduler destructor blocking indefinitely.
  defer(scheduler.unbind());  // Automatically unbind before returning.


  constexpr int numTasks = 10;

  // Create an event that is manually reset.
  marl::Event sayHello(marl::Event::Mode::Manual);

  // Create a WaitGroup with an initial count of numTasks.
  marl::WaitGroup saidHello(numTasks);

  // Schedule some tasks to run asynchronously.
  for (int i = 0; i < numTasks; i++) {
    // Each task will run on one of the 4 worker threads.

    // All marl synchronization primitves aside from marl::ConditionVariable
    // should be lambda-captured by value.
    // [=] Good, [&] Bad.
    // Internally, these primitives hold a shared pointer to the primitive state.
    // By capturing by value we avoid common issues
    // where the primitive may be destructed before the last reference is used.

    marl::schedule([=] {  // all marl primitives are capture-by-value.

      // Decrement the WaitGroup counter when the task has finished.
      defer(saidHello.done());

      printf("Task %d waiting to say hello...\n", i);

      // Blocking in a task?
      // The scheduler will find something else for this thread to do.
      sayHello.wait();

      printf("Hello from task %d!\n", i);
    });
  }

  sayHello.signal();  // Unblock all the tasks.

  saidHello.wait();  // Wait for all tasks to complete.

  printf("All tasks said hello.\n");

  // All tasks are guaranteed to complete before the scheduler is destructed.

  // Destructing the marl::Scheduler requires waiting on all tasks to complete.
}