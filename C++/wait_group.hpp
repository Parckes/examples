#pragma once

#include <cstdlib>
#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/wait/sys.hpp>

class WaitGroup {
 public:
  // += count
  void Add(size_t count) {
    completed_.store(0);
    counter_.fetch_add(count);
  }

  // =- 1
  void Done() {
    auto key = twist::ed::PrepareWake(completed_);
    if (counter_.fetch_sub(1) == 1) {
      completed_.store(1);
      twist::ed::WakeAll(key);
    }
  }

  // == 0
  // One-shot
  void Wait() {
    while (counter_.load() != 0) {
      twist::ed::Wait(completed_, 0);
    }
  }

 private:
  twist::ed::stdlike::atomic<uint32_t> counter_{0};
  twist::ed::stdlike::atomic<uint32_t> completed_{1};
};
