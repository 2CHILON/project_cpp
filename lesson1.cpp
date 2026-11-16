// References vs pointers
#include <iostream>

void addOne(int& x) { x++; } // reference :alias , cant be null
void addOnePtr(int* x) { (*x)++; } // pointer: can be null, can be reassigned

int main() {
    int a = 5;
    addOne(a);
    addOnePtr(&a);
    std::cout << a << '\n';
}