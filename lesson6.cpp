// const correctness

#include <string>
#include <iostream>

class User{
    std::string name_;
public:
    explicit User(std::string n) : name_(std::move(n)) {}
    const std::string& name() const { return name_;} // const method: wont modify the object
};

// void print(const User& u) { // const:ref: no copy, no modification
//     //u.name() works; calling a non-const method would not compile
// }


void print(const User& u) {
    std::cout << "User: " << u.name() << '\n';
}
int main() {
    User u("Alice");
    print(u);
    std::cout << u.name() << '\n';
}

