#include <vector>
#include <string>
#include <utility>
#include <iostream>

int main() {
    std::vector<std::string> v;
    std::string s = "a long string that lives on the heap.....";

    v.push_back(s); //copies s
    v.push_back(std::move(s)); // moves s; s is now valid but unused

    std::cout << v.size() << '\n';
}