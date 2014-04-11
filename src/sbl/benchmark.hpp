#pragma once
#include <map>
#include <string>
#include <chrono>

namespace sbl {
namespace detail {
template <class Result> void benchmark_run(Result &) {}
template <class Result, class Func, class... Args>
void benchmark_run(Result &result, const std::string &name, Func &&func,
                   Args &&... args) {
  benchmark_run(result, std::forward<Args>(args)...);
  auto now = []() { return std::chrono::high_resolution_clock::now(); };
  auto start = now();
  func();
  auto ans = now() - start;
  result.emplace(ans, name);
}
} // namespace detail

template <class... Args> auto benchmark(Args &&... args) {
  std::multimap<std::chrono::duration<double>, std::string> result;
  detail::benchmark_run(result, std::forward<Args>(args)...);
  detail::benchmark_run(result, std::forward<Args>(args)...);
  result.clear();
  detail::benchmark_run(result, std::forward<Args>(args)...);
  return result;
}

template <class Result> auto format_benchmark_result(const Result &result) {
  std::string p;
  size_t m = 0;
  for (auto &kv : result)
    m = std::max(m, kv.second.size());
  for (auto &kv : result) {
    auto name = kv.second;
    while (name.size() < m)
      name += ' ';
    p += name + '\t' + std::to_string(kv.first.count()) + '\n';
  }
  return p;
};

template <class... Args> auto formatted_benchmark(Args &&... args) {
  return format_benchmark_result(benchmark(std::forward<Args>(args)...));
}

void keep(void *);
} // namespace sbl

#define SBL_BENCHMARK_1(FUNC) #FUNC, FUNC
#define SBL_BENCHMARK_2(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_1(__VA_ARGS__)
#define SBL_BENCHMARK_3(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_2(__VA_ARGS__)
#define SBL_BENCHMARK_4(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_3(__VA_ARGS__)
#define SBL_BENCHMARK_5(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_4(__VA_ARGS__)
#define SBL_BENCHMARK_6(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_5(__VA_ARGS__)
#define SBL_BENCHMARK_7(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_6(__VA_ARGS__)
#define SBL_BENCHMARK_8(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_7(__VA_ARGS__)
#define SBL_BENCHMARK_9(FUNC, ...) #FUNC, FUNC, SBL_BENCHMARK_8(__VA_ARGS__)
#define SBL_BENCHMARK_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

#define SBL_BENCHMARK(...)                                                     \
  sbl::benchmark(SBL_BENCHMARK_N(                                              \
      __VA_ARGS__, SBL_BENCHMARK_9, SBL_BENCHMARK_8, SBL_BENCHMARK_7,          \
      SBL_BENCHMARK_6, SBL_BENCHMARK_5, SBL_BENCHMARK_4, SBL_BENCHMARK_3,      \
      SBL_BENCHMARK_2, SBL_BENCHMARK_1)(__VA_ARGS__))

#define SBL_FORMATTED_BENCHMARK(...)                                           \
  sbl::format_benchmark_result(SBL_BENCHMARK(__VA_ARGS__))
