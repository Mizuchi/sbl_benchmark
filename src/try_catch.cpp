#include "sbl/benchmark.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>

int main() {
  std::list<char> s(1e6), s1, s2;
  std::generate(s.begin(), s.end(), std::rand);

  auto with_try = [&]() {
    try {
      s1.clear();
      for (char c : s)
        s1.push_back(c);
    }
    catch (std::exception &e) {
      std::cerr << "This string should exists in binary, "
                   "otherwise the compiler may optimize away exception"
                << e.what() << std::endl;
      abort();
    }
    sbl::keep(&s1);
  };

  auto without_try = [&]() {
    s2.clear();
    for (char c : s)
      s2.push_back(c);
    sbl::keep(&s2);
  };

  std::cout << SBL_FORMATTED_BENCHMARK(with_try, without_try);
}
