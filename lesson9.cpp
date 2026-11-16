#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<int> nums = { 5, 2, 9, 1, 7};

    std::sort(nums.begin(), nums.end(), [](int a , int b) {return a > b; });

    int total = std::accumulate(nums.begin(), nums.end(), 0);

    auto it = std::find_if(nums.begin(), nums.end(), [](int n ) { return n < 5; });
    if (it != nums.end()) std::cout << "first < 5: " << *it << '\n';

    std::unordered_map<std::string, int> counts;
    for (std::string w: {"a", "b", "a"}) counts[w]++;

    for (const auto& [word, n] : counts) // structured bindings
        std::cout << word << "=" << n <<'\n';

    std::cout << "sum=" << total << '\n';
}