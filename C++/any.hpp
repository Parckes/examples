#include <utility>
#include <typeinfo>

struct bad_any_cast : public std::bad_cast {
public:
    const char* what() const noexcept override {
        return "bad any cast";
    }
};

struct Any {
protected:
    template <typename T>
    friend T AnyCast (const Any& value);
private:


    struct Base {
        virtual Base* get_copy() = 0;
        virtual ~Base() {};

    };

    template <typename T>
    struct Derived: public Base {
        T value;

        Derived(const T& value): value(value) {}
        Derived(T&& value): value(std::move(value)){}
        Base* get_copy() override {
            return new Derived<T>(value);
        }
        ~Derived(){}

    };

    Base* storage = nullptr;

public:

    Any() {}

    template <typename U>
    Any(const U& value):storage(new Derived<U>(value)) {}


    Any (const Any& a): storage(a.storage->get_copy()) {}

    Any (Any&& a) noexcept: storage(std::move(a.storage)) {
          a.storage = nullptr;
    }


    template <typename U>
    Any& operator=(const U& value) {
        delete storage;
        storage = new Derived<U>(value);
        return *this;

    }

    Any& operator=(const Any& a) {
        Any(a).Swap(*this);
        return *this;
    }


    template <typename U, typename = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value>::type>
    Any& operator=(U&& value) noexcept {
        delete storage;
        storage = new Derived<U>(value);
        return *this;

    }

    Any& operator=(Any&& a) noexcept {
        Any(std::move(a)).Swap(*this);
        return *this;
    }

    bool HasValue() const {
        return storage != nullptr;
    }

    void Reset() {
        delete storage;
        storage = nullptr;
    }

    void Swap(Any& a) noexcept {
        std::swap(storage, a.storage);
    }


    ~Any() {
        delete storage;
    }
};

template <typename T, typename... Args>
Any MakeAny(Args... args) {
    return Any(T(args...));
}

template <typename T>
T AnyCast (const Any& value) {
    if (value.HasValue() && dynamic_cast<Any::Derived<T>*>(value.storage) != nullptr) {
        return dynamic_cast<Any::Derived<T>*>(value.storage)->value;
    }
    else {
        throw bad_any_cast();
    }
}
