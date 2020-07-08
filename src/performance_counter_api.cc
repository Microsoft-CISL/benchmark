//
// PAPI api for google benchmark
//
#define DEFAULT_EVENTS "TOT,CYC,L1-DCM"
#include <iostream>
#include "performance_counter_api.h"
using namespace std;

#ifdef BENCHMARK_HAS_PERFORMANCE_COUNTER

extern "C"
{
#include <papi.h>
}

namespace benchmark 
{
namespace internal 
{

namespace 
{

  //--------------------------------------------------------------------------------------------
  // Name: Init
  // PURPOSE:
  //   Initialize the PAPI library. This will check PAPI initialization status which can be one
  //   of four cases. 
  //
  bool Init()
  {
    auto retval = PAPI_is_initialized();
    
    if (retval == PAPI_NOT_INITED)
    {
      if (PAPI_VER_CURRENT != PAPI_library_init(PAPI_VER_CURRENT))
        return false;
    }

    if (retval == PAPI_LOW_LEVEL_INITED || retval == PAPI_HIGH_LEVEL_INITED)
    {
      if (PAPI_OK != PAPI_thread_init(pthread_self))
          return false;
    }

    return true;
  }
}

//--------------------------------------------------------------------------------------------
// Name: PerformanceCounter
// PURPOSE:
//   Constructor for PerformanceCounter class 
//       - Initialize events set and size
//       - checks papi lib initialization
//
PerformanceCounter::PerformanceCounter(const PerformanceCounterEvents& events)
    : event_set_(PAPI_NULL)
    , counters_(events.size())
{
  if (!Init())
    return;
  if (PAPI_OK != PAPI_create_eventset(&event_set_))
    return;
  for (const auto event: events)
  {
    char name[PAPI_MAX_STR_LEN];
    if (PAPI_OK != PAPI_event_code_to_name(event, name))
      continue;
    event_names_.push_back(std::string(name).substr(5));
    if (PAPI_OK != PAPI_add_event(event_set_, event))
      continue;
  }
}

//--------------------------------------------------------------------------------------------
// Name: PerformanceCounter
// PURPOSE:
//   Destructor for PerformanceCounter class. 
//
PerformanceCounter::~PerformanceCounter()
{
  PAPI_cleanup_eventset(event_set_);
  PAPI_destroy_eventset(&event_set_);
}

//--------------------------------------------------------------------------------------------
// Name: PerformanceCounter::Start
// PURPOSE:
//   Starts PAPI recording the events set. 
//
bool PerformanceCounter::Start()
{
  return PAPI_OK == PAPI_start(event_set_);
}

//--------------------------------------------------------------------------------------------
// Name: PerformanceCounter::Stop
// PURPOSE:
//   Stops PAPI to record.
//
bool PerformanceCounter::Stop()
{
  if (PAPI_OK != PAPI_accum(event_set_, counters_.data()))
    return false;
  std::vector<long long> dummies(counters_.size());
  return PAPI_OK == PAPI_stop(event_set_, dummies.data());
}

//--------------------------------------------------------------------------------------------
// Name: PerformanceCounter::IncrementCounters
// PURPOSE:
//   Increments the perf counter for each event in events set.
//
void PerformanceCounter::IncrementCounters(UserCounters& counters) const
{
  for (std::size_t i = 0; i < counters_.size(); ++i)
  {
    auto it = counters.find(event_names_[i]);
    if (it == counters.end())
      counters.emplace(event_names_[i], Counter(counters_[i], Counter::kAvgIterations));
    else
      it->second.value += counters_[i];
  }
}

//--------------------------------------------------------------------------------------------
// Name: PerformanceCounter::ReadEvents
// PURPOSE:
//   Reads the events in events set.
//
PerformanceCounterEvents PerformanceCounter::ReadEvents(const std::string& input, std::ostream& err_stream)
{
  if (input.empty())
  {
    return {};
  }

  if (!Init())
  {
    err_stream << "***WARNING*** Performance Counter Library (PAPI) could not initialized.\n";
    return {};
  }

  PerformanceCounterEvents events;
  std::string::size_type start = 0;
  bool help = false;
  std::string eachEvent;

  for (;;)
  {
    auto next = input.find(',', start);
    eachEvent = input.substr(start, next-start);

    const auto name = "PAPI_" + eachEvent;
    //const auto name = eachEvent;

    /*if ( eachEvent.rfind("PAPI_", 0) != 0 )
    {
      name = "PAPI_" + eachEvent;
    }
    std::cout << "name.data: " << name.data();
    */

    int code = 0;
    if (PAPI_OK != PAPI_event_name_to_code(name.data(), &code))
    {
      err_stream << "***WARNING*** There is unknown performance counter event, skipping: '" 
                 << name 
                 << "'\n";
      help = true;
    }
    else
      events.push_back(code);
    if (next >= input.size())
      break;
    start = next + 1;
  }
  if (help)
  {
    err_stream << "***HELP*** To know available events in Performance Counter Library, check output of papi_avail\n\n";
  }
  return events;
}

}  // namespace internal
}  // namespace benchmark

#else

namespace benchmark {
namespace internal {

PerformanceCounter::PerformanceCounter(const PerformanceCounterEvents&)
{
}

PerformanceCounter::~PerformanceCounter()
{
}

bool PerformanceCounter::Start()
{
  return true;
}

bool PerformanceCounter::Stop()
{
  return true;
}

void PerformanceCounter::IncrementCounters(UserCounters&) const
{
}

PerformanceCounterEvents PerformanceCounter::ReadEvents(const std::string& events, std::ostream& err_stream)
{
  if (!events.empty())
    err_stream << "HELLO***WARNING*** PerformanceCounters not supported\n";
  return PerformanceCounterEvents();
}

}  // namespace internal
}  // namespace benchmark

#endif
