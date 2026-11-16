// smart pointers

#include <memory>
#include <iostream>

struct Node {
    int value;
    explicit Node(int v) : value(v) {}
};

int main() {
    auto a = std::make_unique<Node>(1); // sole owner
    // auto b = a; // error: can't copy
    auto b = std::move(a);  // error: can't copy

    auto s1 = std::make_shared<Node>(2); // referenced-counted
    auto s2 = s1;
    std::cout << s1.use_count() << '\n'; // 2
}
