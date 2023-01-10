#include <memory>

template <typename T>
struct WeakPtr;

template <typename T>
struct SharedPtr {
public:

    SharedPtr(): ptr(nullptr), count_weak(new size_t(0)), count_shared(new size_t(0)) {}
    SharedPtr(T* ptr): ptr(ptr), count_shared(new size_t(0)), count_weak(new size_t(0)) {
        if (ptr) ++(*count_shared);
    }

    SharedPtr(const SharedPtr& other): ptr(other.ptr), count_shared(other.count_shared)
                                                    , count_weak(other.count_weak) {
       if (ptr) ++(*count_shared);
       else {
            count_shared = new size_t(0);
            count_weak = new size_t(0);
       }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (other.ptr != ptr) {
            if (*count_shared == 1) {
                --(*count_shared);
                delete ptr;
                delete count_shared;
                delete count_weak;
            }
            else {
                --(*count_shared);
            }
            count_shared = other.count_shared;
            count_weak = other.count_weak;
            ptr = other.ptr;
            ++(*count_shared);
        }
        return *this;

    }

    SharedPtr(SharedPtr&& other) noexcept: ptr(other.ptr), count_weak(other.count_weak)
                                                         , count_shared(other.count_shared) {
        other.ptr = nullptr;
        other.count_shared = new size_t(0);
        other.count_weak = new size_t(0);
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {

        if (*count_shared == 1) {
            --(*count_shared);
            delete ptr;
            delete count_shared;
            delete count_weak;
        }
        else {
            --(*count_shared);
        }

        ptr = other.ptr;
        count_shared = other.count_shared;
        count_weak = other.count_weak;
        other.ptr = nullptr;
        other.count_shared = new size_t(0);
        other.count_weak = new size_t(0);
        return *this;
    }

    template<typename U>
    SharedPtr(SharedPtr<U>&& other) noexcept: ptr(other.ptr), count_weak(other.count_weak)
                                                            , count_shared(other.count_shared){
        other.ptr = nullptr;
        other.count_shared = new size_t(0);
        other.count_weak = new size_t(0);
    }

    template<typename U>
    SharedPtr& operator=(SharedPtr<U>&& other) noexcept {

        if (*count_shared == 1) {
            --(*count_shared);
            delete ptr;
            delete count_shared;
            delete count_weak;
        }
        else {
            --(*count_shared);
        }
        ptr = other.ptr;
        count_shared = other.count_shared;
        count_weak = other.count_weak;
        other.ptr = nullptr;
        other.count_weak = new size_t(0);
        other.count_shared = new size_t(0);
        return *this;
    }


    SharedPtr(const WeakPtr<T>& w) {
        if (w.Expired()) throw std::bad_weak_ptr();
        ptr = w.ptr;
        count_shared = w.count_shared;
        count_weak = w.count_weak;
        if (ptr)  ++(*count_shared);
     }


     void Reset(T* new_ptr = nullptr) {
         if (ptr) {
             if (*count_shared == 1) {
                 --(*count_shared);
                 delete ptr;
                 if (*count_weak) {
                     count_weak = new size_t(0);
                     count_shared = new size_t(0);
                 }
             }
             else {
                 --(*count_shared);
                 count_weak = new size_t(0);
                 count_shared = new size_t(0);
             }
         }

         ptr = new_ptr;

         if (new_ptr) {
             ++(*count_shared);
         }
     }

     void Swap(SharedPtr& other) {
        std::swap(other.ptr, ptr);
        std::swap(other.count_weak, count_weak);
        std::swap(other.count_shared, count_shared);
     }

     T* Get() const {
        return ptr;
     }

     size_t UseCount() const {
        return *count_shared;
     }

     T& operator*() const {
        return *ptr;
     }

     T* operator->() const {
        return ptr;
     }

     explicit operator bool() const {
        return ptr != nullptr;
     }



    ~SharedPtr() {
        if (*count_shared > 0) --(*count_shared);
        if (*count_shared == 0) {
            if (*count_weak == 0) {
                delete count_shared;
                delete count_weak;
            }
            if(ptr) delete ptr;
        }
    }


private:
    T* ptr;
    size_t* count_weak;
    size_t* count_shared;

    friend WeakPtr<T>;

};

template <typename T>
struct WeakPtr {
public:
    WeakPtr(): ptr(nullptr), count_weak(new size_t(0)), count_shared(new size_t(0)) {}
    WeakPtr(const WeakPtr& other): ptr(other.ptr), count_weak(other.count_weak)
                                                , count_shared(other.count_shared) {
        if(ptr) ++(*count_weak);
        else {
           count_shared = new size_t(0);
           count_weak = new size_t(0);
        }
    }
    WeakPtr& operator=(const WeakPtr& other) {
        if(ptr) --(*count_weak);
        ptr = other.ptr;
        count_weak = other.count_weak;
        count_shared = other.count_shared;
        if (ptr) ++(*count_weak);
        else {
            count_shared = new size_t(0);
            count_weak = new size_t(0);
        }
        return *this;
    }

    WeakPtr& operator=(const SharedPtr<T>& other) {
        if(ptr) --(*count_weak);
        ptr = other.ptr;
        count_weak = other.count_weak;
        count_shared = other.count_shared;
        if (ptr) ++(*count_weak);
        else {
            count_shared = new size_t(0);
            count_weak = new size_t(0);
        }
        return *this;
    }

    WeakPtr(WeakPtr&& other) noexcept: ptr(other.ptr), count_weak(other.count_weak)
                                            , count_shared(other.count_shared) {
        other.ptr = nullptr;
        other.count_weak = new size_t(0);
        other.count_shared = new size_t(0);
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (ptr) --(*count_weak);
        ptr = other.ptr;
        count_weak = other.count_weak;
        count_shared = other.count_shared;

        other.ptr = nullptr;
        other.count_weak = new size_t(0);
        other.count_shared = new size_t(0);
        return *this;
    }

    template<typename U>
    WeakPtr(WeakPtr<U>&& other) noexcept: ptr(other.ptr), count_weak(other.count_weak)
                                                        , count_shared(other.count_shared) {
        other.ptr = nullptr;
        other.count_weak = new size_t(0);
        other.count_shared = new size_t(0);
    }

    template<typename U>
    WeakPtr& operator=(WeakPtr<U>&& other) noexcept {
        if (ptr) --(*count_weak);
        ptr = other.ptr;
        count_weak = other.count_weak;
        count_shared = other.count_shared;

        other.ptr = nullptr;
        other.count_weak = new size_t(0);
        other.count_shared = new size_t(0);
        return *this;
    }

    WeakPtr(const SharedPtr<T> s): ptr(s.ptr), count_weak(s.count_weak)
                                             , count_shared(s.count_shared) {
        if (ptr) ++(*count_weak);
        else {
           count_shared = new size_t(0);
           count_weak = new size_t(0);
        }
    }

    bool Expired() const { return !(*count_shared); }

    SharedPtr<T> Lock() const {
        if (this->Expired()) return SharedPtr<T>(nullptr);
        else return SharedPtr<T>(*this);
    }


   size_t UseCount() const {
      return *count_shared;
   }

   void Reset() {
      if (ptr) {
          --(*count_weak);
          ptr = nullptr;
          count_shared = new size_t(0);
          count_weak = new size_t(0);
      }
   }

    ~WeakPtr() {
        if (*count_weak == 0 && *count_shared == 0) {
            delete count_weak;
            delete count_shared;
        }
        else {
            --(*count_weak);
        }

    }

private:
    T* ptr;
    size_t* count_shared;
    size_t* count_weak;

    friend SharedPtr<T>;
};


template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args... args) {
    return SharedPtr<T>(new T(args...));
}
