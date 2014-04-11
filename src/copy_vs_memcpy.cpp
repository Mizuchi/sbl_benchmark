#include "sbl/benchmark.hpp"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<char> s(1e8), s1(s.size()), s2(s.size());
  std::generate(s.begin(), s.end(), std::rand);

  auto std_copy = [&]() {
    std::copy(s.begin(), s.end(), s1.begin());
    sbl::keep(&s1);
  };

  auto std_memcpy = [&]() {
    std::memcpy(s2.data(), s.data(), s.size());
    sbl::keep(&s2);
  };

  std::cout << SBL_FORMATTED_BENCHMARK(std_copy, std_memcpy) << std::endl;
}
