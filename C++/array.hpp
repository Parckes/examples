#include <stdexcept>
#pragma once

template <typename T, int N>
class Array {
public:
    Array() = default;

    Array(std::initializer_list<T> lst) {  // Do not touch.
      std::copy(lst.begin(), lst.end(), arr_);
    }

    T& Front()             { return arr_[0]; };
    const T& Front() const { return arr_[0]; };

    T& Back()              { return arr_[N - 1]; };
    const T& Back()  const { return arr_[N - 1]; };


    int Size()       const { return N; };

    bool Empty()     const { return !N; };

    T* Data()              { return arr_; };
    const T* Data()  const { return arr_; };

    T& operator[](int i)   { return arr_[i]; };
    const T& operator[](int i) const { return arr_[i]; };

    void Swap(Array<T, N>& cur) {
        for (int i = 0; i < N; ++i) {
            T temp = cur[i];
            cur[i] = arr_[i];
            arr_[i] = temp;
        }
    };

private:
    T arr_[N];
};
