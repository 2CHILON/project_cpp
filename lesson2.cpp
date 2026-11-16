// Resource allocation
// Resources (memory , files , locks) are tied to object lifetime.
// The destructor runs automatically when the object leaves the scope
// There is no finally and no GC

#include <iostream>
#include <string>

class FileGuard {
    std::string name;
public:
    explicit FileGuard(std::string n ) : name(std::move(n)) {
        std::cout << "open " << name << '\n';
    }
    ~FileGuard() { std::cout << "close " << name << '\n';}
};

int main() {
    FileGuard f("data.txt");
    std::cout << "working...\n";
} // "close data.txt" printed automatically here
