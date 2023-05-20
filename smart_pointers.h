#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class SharedPtr {
   private:
    T* ptr;
    int* cnt;

   public:
    template <typename U>
    explicit SharedPtr(U* ptr) : ptr(dynamic_cast<T*>(ptr)), cnt(new int(1)) {}

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), cnt(other.cnt) {
        ++*cnt;
    }

    SharedPtr(SharedPtr&& other) : ptr(other.ptr), cnt(other.cnt) {
        other.cnt = nullptr;
        other.ptr = nullptr;
    }

    ~SharedPtr() {
        if (!cnt) return;
        --*cnt;
        if (*cnt == 0) {
            delete cnt;
            delete ptr;
        }
    }
};

template <typename T>
using WeakPtr = std::weak_ptr<T>;

template <typename T>
using EnableSharedFromThis = std::enable_shared_from_this<T>;

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename Alloc, typename... Args>
SharedPtr<T> allocateShared(const Alloc& alloc, Args&&... args) {
    return std::allocate_shared<T>(std::forward<const Alloc>(alloc),
                                   std::forward<Args>(args)...);
}