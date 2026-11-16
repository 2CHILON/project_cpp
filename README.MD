# C++ Core Concepts: Lessons 1 to 11

Eleven small, runnable programs covering the core of modern C++ (C++20). Each lesson is one file with its own `main()`.

## Setup (Windows 11, VS Code terminal)

Check that you have a compiler:

```powershell
g++ --version
```

If it is not recognized, install MSYS2 (https://www.msys2.org), then in the **MSYS2 UCRT64** terminal run:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-gcc
```

Add `C:\msys64\ucrt64\bin` to your Windows `PATH`, then restart VS Code.

## How to run any lesson

Files must use the `.cpp` extension (not `.c++`). From the folder containing the file:

```powershell
g++ -std=c++20 lessonN.cpp -o lessonN; .\lessonN
```

Replace `N` with the lesson number. Recompile after every edit.

## Troubleshooting

| Error | Cause | Fix |
|---|---|---|
| `undefined reference to WinMain` or `main` (then `ld returned 1 exit status`) | The file has no `main()` | Add one (every lesson below already includes one) |
| `multiple definition of main` | Two `main()` functions in one file | Keep only one |
| `cannot open output file ... Permission denied` | The old `.exe` is still running | Close it, or `taskkill /f /im lessonN.exe` |
| `'string' is not a member of 'std'` | Missing include | Add `#include <string>` |

## Contents

| # | File | Concept |
|---|------|---------|
| 1 | `lesson1.cpp` | References vs pointers |
| 2 | `lesson2.cpp` | RAII |
| 3 | `lesson3.cpp` | Smart pointers |
| 4 | `lesson4.cpp` | Move semantics |
| 5 | `lesson5.cpp` | Rule of 0/3/5 |
| 6 | `lesson6.cpp` | `const` correctness |
| 7 | `lesson7.cpp` | Polymorphism |
| 8 | `lesson8.cpp` | Templates |
| 9 | `lesson9.cpp` | STL, algorithms, lambdas |
| 10 | `lesson10.cpp` | `optional` and `variant` |
| 11 | `lesson11.cpp` | Exceptions |

---

## Lesson 1: References vs pointers

A reference is an alias for an existing object and cannot be null. A pointer holds an address, can be null, and can be reassigned. Use a reference when you need an existing object, and a pointer only when "nothing" is a valid value.

```cpp
#include <iostream>

void addOne(int& x) { x++; }        // reference
void addOnePtr(int* x) { (*x)++; }  // pointer

int main() {
    int a = 5;
    addOne(a);       // a == 6
    addOnePtr(&a);   // a == 7
    std::cout << a << '\n';
}
```

**Output**

```
7
```

---

## Lesson 2: RAII

Resources (memory, files, locks) are tied to object lifetime. The destructor runs automatically when the object leaves scope, even when an exception is thrown. C++ has no `finally` and no garbage collector.

```cpp
#include <iostream>
#include <string>
#include <utility>

class FileGuard {
    std::string name;
public:
    explicit FileGuard(std::string n) : name(std::move(n)) {
        std::cout << "open " << name << '\n';
    }
    ~FileGuard() { std::cout << "close " << name << '\n'; }
};

int main() {
    FileGuard f("data.txt");
    std::cout << "working...\n";
}   // destructor runs here
```

**Output**

```
open data.txt
working...
close data.txt
```

**Experiment:** wrap `f` and the `working...` line in an inner `{ }` block and print `after scope` after it. `close data.txt` now appears before `after scope`.

---

## Lesson 3: Smart pointers

Never write `new` or `delete` by hand.

- `unique_ptr`: sole owner, cannot be copied, only moved. Use this by default.
- `shared_ptr`: reference-counted shared ownership.
- `weak_ptr`: non-owning observer, used to break `shared_ptr` cycles.

```cpp
#include <iostream>
#include <memory>

struct Node {
    int value;
    explicit Node(int v) : value(v) {}
};

int main() {
    auto a = std::make_unique<Node>(1);   // sole owner
    // auto b = a;                        // error: cannot copy
    auto b = std::move(a);                // ownership moves; a is now null

    auto s1 = std::make_shared<Node>(2);  // reference-counted
    auto s2 = s1;                         // both own it
    std::cout << s1.use_count() << '\n';
}
```

**Output**

```
2
```

---

## Lesson 4: Move semantics

Copying can be expensive. Moving steals the internals of a temporary. `std::move` itself moves nothing; it casts to an rvalue so the move constructor or move assignment is chosen. After a move, the source is valid but unspecified.

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::string> v;
    std::string s = "a long string that lives on the heap......";

    v.push_back(s);             // copies s
    v.push_back(std::move(s));  // moves s

    std::cout << v.size() << '\n';
}
```

**Output**

```
2
```

---

## Lesson 5: Rule of 0/3/5

If your class owns no raw resources, define none of the special member functions (**Rule of 0**). If it owns a raw resource, define all five: destructor, copy constructor, copy assignment, move constructor, move assignment (**Rule of 5**). In real code, prefer `std::vector` and get all of this for free.

This version prints a message from each function so you can see which one fires.

```cpp
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
    int* data;
    std::size_t size;
public:
    explicit Buffer(std::size_t n) : data(new int[n]{}), size(n) {
        std::cout << "ctor\n";
    }
    ~Buffer() {
        std::cout << "dtor\n";
        delete[] data;
    }

    Buffer(const Buffer& o) : data(new int[o.size]), size(o.size) {
        std::cout << "copy ctor\n";
        std::copy(o.data, o.data + size, data);
    }
    Buffer& operator=(const Buffer& o) {
        std::cout << "copy assign\n";
        if (this != &o) {
            Buffer tmp(o);
            std::swap(data, tmp.data);
            std::swap(size, tmp.size);
        }
        return *this;
    }

    Buffer(Buffer&& o) noexcept
        : data(std::exchange(o.data, nullptr)), size(std::exchange(o.size, 0)) {
        std::cout << "move ctor\n";
    }
    Buffer& operator=(Buffer&& o) noexcept {
        std::cout << "move assign\n";
        if (this != &o) {
            delete[] data;
            data = std::exchange(o.data, nullptr);
            size = std::exchange(o.size, 0);
        }
        return *this;
    }
};

int main() {
    Buffer a(5);
    Buffer b = a;              // copy constructor
    Buffer c = std::move(a);   // move constructor (a is now empty)

    Buffer d(3);
    d = b;                     // copy assignment
    d = std::move(c);          // move assignment

    std::cout << "done\n";
}
```

**Output** (the `ctor`/`dtor` lines and the temporary inside copy assignment make it long; watch for these key lines in order)

```
ctor
copy ctor
move ctor
ctor
copy assign
copy ctor
dtor
move assign
done
dtor
dtor
dtor
dtor
```

---

## Lesson 6: `const` correctness

A `const` method promises not to modify the object. A `const T&` parameter avoids a copy and forbids modification. Pass big read-only objects by `const T&` and small ones (such as `int`) by value.

```cpp
#include <iostream>
#include <string>
#include <utility>

class User {
    std::string name_;
public:
    explicit User(std::string n) : name_(std::move(n)) {}
    const std::string& name() const { return name_; }  // const method
    void setName(std::string n) { name_ = std::move(n); }  // non-const method
};

void print(const User& u) {
    std::cout << "User: " << u.name() << '\n';
    // u.setName("Bob");   // uncomment: compile error, u is const
}

int main() {
    User u("Alice");
    print(u);
    std::cout << u.name() << '\n';
}
```

**Output**

```
User: Alice
Alice
```

**Experiment:** uncomment the `u.setName("Bob");` line. The compiler rejects it, which is `const` doing its job.

---

## Lesson 7: Polymorphism

A pure virtual function (`= 0`) makes a class abstract, like a Java abstract method. A base class with virtual functions **must** have a virtual destructor. Methods are not virtual by default in C++, and `override` makes the compiler catch typos.

```cpp
#include <iostream>
#include <memory>
#include <vector>

struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape {
    double r;
    explicit Circle(double r) : r(r) {}
    double area() const override { return 3.14159 * r * r; }
};

struct Rect : Shape {
    double w, h;
    Rect(double w, double h) : w(w), h(h) {}
    double area() const override { return w * h; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rect>(3, 4));
    for (const auto& s : shapes) std::cout << s->area() << '\n';
}
```

**Output**

```
12.5664
12
```

---

## Lesson 8: Templates

Templates are generics resolved at compile time. The compiler generates separate code for each type, so they cost nothing at runtime (unlike Java's type erasure).

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename T>
T maxOf(const T& a, const T& b) { return a > b ? a : b; }

template <typename T>
class Stack {
    std::vector<T> items;
public:
    void push(T v) { items.push_back(std::move(v)); }
    T pop() { T v = std::move(items.back()); items.pop_back(); return v; }
    bool empty() const { return items.empty(); }
};

int main() {
    std::cout << maxOf(3, 7) << ' ' << maxOf(2.5, 1.5) << '\n';
    Stack<std::string> s;
    s.push("hi");
    std::cout << s.pop() << '\n';
}
```

**Output**

```
7 2.5
hi
```

---

## Lesson 9: STL containers, algorithms, and lambdas

```cpp
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<int> nums = {5, 2, 9, 1, 7};

    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; });
    // nums is now 9 7 5 2 1

    int total = std::accumulate(nums.begin(), nums.end(), 0);

    auto it = std::find_if(nums.begin(), nums.end(), [](int n) { return n < 5; });
    if (it != nums.end()) std::cout << "first < 5: " << *it << '\n';

    std::unordered_map<std::string, int> counts;
    for (std::string w : {"a", "b", "a"}) counts[w]++;

    for (const auto& [word, n] : counts)   // structured bindings (C++17)
        std::cout << word << "=" << n << '\n';

    std::cout << "sum=" << total << '\n';
}
```

**Output** (the order of the two `unordered_map` lines is not guaranteed)

```
first < 5: 2
a=2
b=1
sum=24
```

---

## Lesson 10: `optional` and `variant`

`std::optional<T>` replaces "return -1 or null on failure". `std::variant` is a type-safe union, and `std::visit` handles whichever type it currently holds.

```cpp
#include <iostream>
#include <optional>
#include <string>
#include <variant>

std::optional<int> parse(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return std::nullopt; }
}

int main() {
    if (auto n = parse("42")) std::cout << *n << '\n';
    if (!parse("abc")) std::cout << "parse failed\n";

    std::variant<int, std::string> v = "hello";
    std::visit([](auto&& x) { std::cout << x << '\n'; }, v);
}
```

**Output**

```
42
parse failed
hello
```

---

## Lesson 11: Exceptions

Catch by `const` reference. Thanks to RAII, destructors run automatically as the stack unwinds, so cleanup is handled for you.

```cpp
#include <iostream>
#include <stdexcept>

int divide(int a, int b) {
    if (b == 0) throw std::invalid_argument("divide by zero");
    return a / b;
}

int main() {
    try {
        std::cout << divide(10, 2) << '\n';
        divide(1, 0);
    }
    catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
    }
}
```

**Output**

```
5
error: divide by zero
```

---

## Common beginner traps

- Returning a reference or pointer to a local variable (it dies when the function returns).
- Using iterators after `push_back` invalidates them.
- Forgetting a `virtual` destructor on a base class.
- Slicing: copying a derived object into a base-class object by value.
- Using a moved-from object as if it still held its old value.

## What to learn next

1. Concurrency: `std::thread`, `std::mutex`, `std::atomic`
2. `constexpr` and compile-time computation
3. Concepts (C++20)
4. The C++ memory model
5. Build systems: CMake