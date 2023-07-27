#include <tp/thread_pool.hpp>
#include <tp/queue.hpp>

#include <twist/ed/local/ptr.hpp>

#include <wheels/core/panic.hpp>
#include <twist/ed/stdlike/condition_variable.hpp>
#include <chrono>

#include <iostream>

#include <wheels/test/framework.hpp>

namespace tp {

TWISTED_THREAD_LOCAL_PTR(ThreadPool, pool)

ThreadPool::ThreadPool(size_t threads)
    : num_threads_(threads) {
}

void ThreadPool::Start() {
  for (std::size_t i = 0; i < num_threads_; ++i) {
    workers_.emplace_back(([this] {
      Worker();
    }));
  }
}

ThreadPool::~ThreadPool() {
  ASSERT_FALSE(!workers_.empty());
}

void ThreadPool::Submit(Task task) {
  wait_group_.Add(1);
  queue_.Put(std::move(task));
}

ThreadPool* ThreadPool::Current() {
  return pool;
}

void ThreadPool::WaitIdle() {
  wait_group_.Wait();
}

void ThreadPool::Stop() {
  queue_.Close();
  working_ = false;
  for (auto& worker : workers_) {
    worker.join();
  }
  workers_.clear();
}

void ThreadPool::Worker() {
  pool = this;
  while (working_) {
    auto task = queue_.Take();
    if (task) {
      task.value()();
      wait_group_.Done();
    }
  }
}

}  // namespace tp
