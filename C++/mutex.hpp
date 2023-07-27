#pragma once

#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/wait/sys.hpp>

#include <cstdlib>

namespace stdlike {

class Mutex {
 public:
  void Lock() {
    uint32_t compare_with = 0;

    if (condition_.compare_exchange_strong(compare_with, 1)) {
      return;
    }
    do {
      compare_with = 1;
      if (condition_ == 2 ||
          condition_.compare_exchange_strong(compare_with, 2)) {
        twist::ed::Wait(condition_, 2);
      }
      compare_with = 0;
    } while (!condition_.compare_exchange_strong(compare_with, 2));
  }

  void Unlock() {
    auto key = twist::ed::PrepareWake(condition_);
    if (condition_.exchange(0) == 2) {
      twist::ed::WakeOne(key);
    }
  }

 private:
  // 0 - free
  // 1 - busy, but no waiting thread
  // 2 - busy and there are waiting thr
  twist::ed::stdlike::atomic<uint32_t> condition_{0};
};

}  // namespace stdlike

/*
 * class Mutex {
public:
 void Lock() {
   waiting_.fetch_add(1);
   uint32_t expected = 0;
   while (!locked_.compare_exchange_strong(expected, 1)) {
     twist::ed::Wait(locked_, 1);
     expected = 0;
   }
 }

void Unlock() {
 auto key = twist::ed::PrepareWake(locked_);
 locked_.store(2);
 if (waiting_.fetch_sub(1) != 1) {
   twist::ed::WakeOne(key);
 }
 locked_.store(0);
}

void lock() {  // NOLINT
 Lock();
}

void unlock() {  // NOLINT
 Unlock();
}

private:
twist::ed::stdlike::atomic<uint32_t> waiting_{0};
// 0 - not busy 1 - true busy 2 - end of busy (unlocking)
twist::ed::stdlike::atomic<uint32_t> locked_{0};
};
 */