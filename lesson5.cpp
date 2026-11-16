//classes and the rule of 0 3 5, 
// if your class holds no raw resources , write one of the special member functions
// rule of 0 . if it manages a raw resource, define all five

#include <algorithm>
#include <cstddef>
#include <utility>
#include<iostream>

class Buffer {
    int* data;
    std::size_t size;
public:
    explicit Buffer(std::size_t n) : data(new int[n]{}), size(n) {}
    ~Buffer() { delete[] data;}

    Buffer(const Buffer& o) : data(new int[o.size]), size(o.size) { //copy
        std::copy(o.data, o.data + size, data);
    }
    Buffer& operator=(const Buffer& o) {
        if (this != &o) { Buffer tmp(o); std::swap(data, tmp.data); std::swap(size, tmp.size); }
        return *this;
    }
    Buffer(Buffer&& o) noexcept : data(std::exchange(o.data, nullptr)),
                                    size(std::exchange(o.size, 0)) {} // move
    Buffer& operator=(Buffer&& o) noexcept {
        if (this != &o) { delete[] data; data = std::exchange(o.data, nullptr); size = std::exchange(o.size, 0);}
        return *this;
    }
};

int main() {
    Buffer a(5);
    Buffer b = a; //copy constructor
    Buffer c = std::move(a); // move constructor (a is now empty)

    Buffer d(3);
    d = b; //copy assignment
    d = std::move(c); // move assignment

    std::cout << "done\n";

}