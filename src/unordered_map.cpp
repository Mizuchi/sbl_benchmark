#include "sbl/benchmark.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>

int main() {
  std::vector<int> s(1e5);
  std::generate(s.begin(), s.end(), std::rand);
  std::set<int> s1;
  std::unordered_set<int> s2;

  auto f = [&s](auto c) {
    for (auto i : s)
      if (c.find(i) == c.end())
        c.insert(i);
    for (auto i : s)
      if (c.find(i) == c.end())
        c.erase(i);
    sbl::keep(&c);
  };
  std::cout << sbl::formatted_benchmark("set", std::bind(f, s1),
                                        "unordered_set", std::bind(f, s2))
            << std::endl;
}
