// Templates(generics, resolved at compile time)

#include <iostream>
#include <vector>

template <typename T>
T maxOf(const T& a, const T& b) { return a > b ? a: b;}

template <typename T>
class Stack {
    std::vector<T> items;
public:
    void push(T v) { items.push_back(std::move(v)); }
    T pop() { T v = std::move(items.back()); items.pop_back(); return v;}
    bool empty() const { return items.empty(); }
};

int main() {
    std::cout << maxOf(3, 7) << ' ' << maxOf(2.5, 1.5) << '\n';
    Stack<std::string> s;
    s.push("hi");
    std::cout<< s.pop() << '\n';
}
