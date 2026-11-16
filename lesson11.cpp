#include <stdexcept>
#include <iostream>

int divide(int a, int b) {
    if (b == 0) throw std::invalid_argument("divide by zero");
    return a /b ;
}

int main() {
    try { divide(1, 0); }
    catch (const std::exception& e) { std::cerr << e.what() << e.what() << '\n'; } //catch by const ref
    
}