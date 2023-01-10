template <typename T>
struct UniquePtr {
public:
    UniquePtr(): ptr(nullptr) {}


    UniquePtr(T* p): ptr(p) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;


    UniquePtr(UniquePtr&& p) noexcept: ptr(p.ptr) {
        p.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& p) noexcept {
        delete ptr;
        ptr = p.ptr;
        p.ptr = nullptr;
        return *this;
    }

    template<typename U>
    UniquePtr(UniquePtr<U>&& p) noexcept: ptr(p.ptr) {
        p.ptr = nullptr;
    }

    template<typename U>
    UniquePtr& operator=(UniquePtr<U>&& p) noexcept {
        delete ptr;
        ptr = p.ptr;
        p.ptr = nullptr;
        return *this;
    }

    T* Release() {
        T* t = ptr;
        ptr = nullptr;
        return t;
    }

    void Reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    T* Get() const {
        return ptr;
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

    void Swap(UniquePtr& p) {
        T* t = p.ptr;
        p.ptr = ptr;
        ptr = t;
    }

    ~UniquePtr() {
        delete ptr;
    }

private:
    T* ptr;
};

template <typename T, typename... Args>
UniquePtr<T> MakeUnique (Args... args) {
    return UniquePtr<T>(new T(args...));
};
