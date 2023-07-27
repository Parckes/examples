#pragma once

#include <twist/ed/stdlike/mutex.hpp>
#include <twist/ed/stdlike/condition_variable.hpp>
#include <twist/ed/stdlike/atomic.hpp>
#include <deque>

#include <optional>

namespace tp {

// Unbounded blocking multi-producers/multi-consumers (MPMC) queue

template <typename T>
class UnboundedBlockingQueue {
 public:
  bool Put(T a) {
    std::lock_guard guard(mutex_);
    if (static_cast<int>(turn_off_) == 1) {
      return false;
    }
    buffer_.push_back(std::move(a));
    not_empty_.notify_one();
    return true;
  }

  std::optional<T> Take() {
    std::unique_lock lock(mutex_);
    while (static_cast<int>(turn_off_) == 0 && buffer_.size() == 0) {
      not_empty_.wait(lock);
    }
    if (static_cast<int>(turn_off_) == 1 && buffer_.size() == 0) {
      return std::nullopt;
    }
    T res = std::move(buffer_.front());
    buffer_.pop_front();
    return res;
  }

  void Close() {
    std::lock_guard guard(mutex_);
    turn_off_ = true;
    not_empty_.notify_all();
  }

 private:
  std::deque<T> buffer_;
  twist::ed::stdlike::mutex mutex_;
  twist::ed::stdlike::condition_variable not_empty_;
  twist::ed::stdlike::atomic<bool> turn_off_{false};
};

}  // namespace tp
