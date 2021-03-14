#include <iostream>
#include <cstdlib>
#include "ThreadPool.hpp"

int main()
{
  std::cout << "BEFORE" << std::endl;

  ThreadPool::GetInstance().AllocateTask([]() {
    std::cout << "Hello, World!" << std::endl;
  });

  std::cout << "AFTER" << std::endl;

  return EXIT_SUCCESS;
}

// BEFORE
// AFTER
// Hello, World!
