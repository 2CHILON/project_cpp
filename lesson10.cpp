// modern utility types
#include <optional>
#include <variant>
#include <string>
#include <iostream>

std::optional<int> parse(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return std::nullopt; }
}

int main() {
    if (auto n = parse("42")) std::cout << *n << '\n';

    std::variant<int, std::string> v = "hello";
    std::visit([](auto&& x) { std::cout << x << '\n'; }, v);
}