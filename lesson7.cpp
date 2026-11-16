//polymorphism

#include <iostream>
#include <memory>
#include <vector>

struct Shape {
    virtual double area() const = 0; // pure virtual (like  a Java abs method)
    virtual ~Shape() = default; //virtaul destructor is Required for base classes
};

struct Circle: Shape {
    double r;
    explicit Circle(double r) : r(r) {}
    double area() const override { return 3.14159 * r * r; }
};

struct Rect: Shape {
    double w, h;
    Rect(double w, double h) : w(w), h(h) {}
    double area() const override { return w * h; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rect>(3, 4));
    for (const auto& s: shapes) std::cout << s->area() << '\n';
}