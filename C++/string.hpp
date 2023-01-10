#include <cstring>
#include <algorithm>
#include <iostream>


struct String {
public:
    String(): size(0), capacity(0), str(nullptr) {}

    //String(size_t n): size(0), capacity(n), str(new char[n]) {}

    String(size_t n, char a): size(n), capacity(n) {
        str = new char[capacity + 1];
        for (size_t i = 0; i < n; ++i) str[i] = a;
        str[size] = '\0';
    }

    String(const char* inp, size_t n): size(n), capacity(n) {
        str = new char[capacity + 1];
        size_t len = strlen(inp);
        for (size_t i = 0; i < std::min(n, len); ++i) str[i] = inp[i];
        str[size] = '\0';
    }

    String(const char* inp): size(strlen(inp)), capacity(strlen(inp)) {
        str = new char[capacity + 1];
        for (size_t i = 0; i < size; ++i) str[i] = inp[i];
        str[size] = '\0';

    }

    String(const String& str2): size(str2.size), capacity(str2.capacity) {
        if (str2.str) {
            str = new char[capacity + 1];
            for (size_t i = 0; i < size; ++i) {
                str[i] = str2.str[i];
            }
            str[size] = '\0';
        }
        else str = nullptr;
    }

    String& operator=(const String& str2) {
        if (str != str2.str) {
            size = str2.size;
            capacity = str2.capacity;
            if (str) delete[] str;
            str = new char[capacity + 1];
            for (size_t i = 0; i < size; ++i) {
                str[i] = str2.str[i];
            }
            str[size] = '\0';
        }
        return *this;
    }

    const char& operator[](size_t i) const {
        return str[i];
    }

    char& operator[](size_t i) {
        return str[i];
    }

    char& Front() { return str[0]; };

    const char& Front() const { return str[0]; };

    char& Back() { return str[size - 1]; }

    const char& Back() const { return str[size - 1]; }

    const char* CStr() const { return str; };

    const char* Data() const { return str; };

    size_t Capacity() const { return capacity; };

    bool Empty() const { return !size; };

    size_t Size() const { return size; };

    size_t Length() const { return size; };

    void Clear() { size = 0; }

    void Swap(String& str2) {
        std::swap(str2.size, size);
        std::swap(str2.capacity, capacity);
        std::swap(str2.str, str);
     }

     void PopBack() {
        size = std::max(size_t(0), size - 1);
        str[size] = '\0';
      }

     void PushBack(char c) {
         if (!capacity) {
            if (str) delete[] str;
            ++capacity;
            str = new char[capacity + 1];
            str[0] = c;
            str[1] = '\0';
         }
         if (size < capacity) {
            str[size] = c;
            str[size + 1] = '\0';
         }
         else {
            capacity *= 2;
            char* str_t = new char[capacity + 1];
            for (size_t i = 0; i < size; ++i) {
                str_t[i] = str[i];
            }
            if (str) delete[] str;
            str = str_t;
            str[size] = c;
            str[size + 1] = '\0';
         }
         ++size;
     }

     String& operator+=(const String str2) {
        if (size + str2.size > capacity) {
            capacity = size + str2.size;
            char* str_t = new char[capacity + 1];
            for (size_t i = 0; i < size; ++i) str_t[i] = str[i];
            delete[] str;
            str = str_t;
        }
        for (size_t i = 0; i < str2.size; ++i) {
            str[size + i] = str2[i];
        }
        size += str2.size;
        str[size] = '\0';
        return *this;
     }

     void Resize(size_t new_size, char a) {
        if (new_size > size) {
            if (new_size > capacity) {
                capacity = new_size;
                char* str_t = new char[new_size + 1];
                for (size_t i = 0; i < size; ++i) str_t[i] = str[i];
                delete[] str;
                str = str_t;
            }
            for (size_t i = size; i < new_size; ++i) str[i] = a;
            size = new_size;
        }
        else {
            size = new_size;
        }
        str[size] = '\0';
     }

     void Reserve(size_t new_capacity) {
          if (capacity < new_capacity) {
              capacity = new_capacity;
              char* str_t = new char[capacity + 1];
              for (size_t i = 0; i < size; ++i) str_t[i] = str[i];
              delete[] str;
              str = str_t;
          }
     }

     void ShrinkToFit() {
          if (capacity > size) {
              capacity = size;
              char* str_t = new char[capacity];
              for (size_t i = 0; i < size; ++i) str_t[i] = str[i];
              delete[] str;
              str = str_t;
              str[size] = '\0';
          }
     }



    ~String() {
        delete[] str;
    }
private:
    char* str;
    size_t size;
    size_t capacity;

};

bool operator==(const String& str1, const String& str2) {
    return !strcmp(str1.Data(), str2.Data());
}

int operator<=>(const String& str1, const String& str2) {
    return strcmp(str1.Data(), str2.Data());
}

String operator+(const String& str1, const String& str2) {
   String res(str1);
   res += str2;
   return res;
}

std::ostream& operator<<(std::ostream& ostream, const String& str) {
    if (str.Data() == nullptr) return ostream << "";
    return ostream << str.Data();
}
