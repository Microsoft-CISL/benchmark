//
// PAPI api for google benchmark
//

#ifndef BENCHMARK_PERFORMANCE_COUNTER_API_H_
#define BENCHMARK_PERFORMANCE_COUNTER_API_H_

#include <vector>
#include <string>
#include <ostream>
#include "benchmark/benchmark.h"

namespace benchmark 
{
namespace internal 
{

using PerformanceCounterEvents = std::vector<int>;

class PerformanceCounter
{
  public:
  explicit PerformanceCounter(const PerformanceCounterEvents& events);
  ~PerformanceCounter();

  bool Start();
  bool Stop();

  void IncrementCounters(UserCounters&) const;
  void CheckAvailCounters();

  static PerformanceCounterEvents ReadEvents(const std::string& input, std::ostream& err_stream);

private:
#ifdef BENCHMARK_HAS_PERFORMANCE_COUNTER
  int event_set_;
  std::vector<long long> counters_;
  std::vector<std::string> event_names_;
#endif
};

}  // namespace internal
}  // namespace benchmark

#endif  // #ifndef BENCHMARK_PERFORMANCE_COUNTER_API_H_
