## Performance Counter Collection
```bash
In MLOS, collection of performance counters from underlying hardware or software at various testing levels of an application can be extremely useful to give the context to the tunables choice. For example, developers can collect various performance metrics in (micro)benchmarks and use them to explore a parameter space for a tunable in a particular component of entire application. These perforamce metrics can be of these 3 types:
1. Hardware counters. For example, CPU cycles
2. Software counter. For example, context-switches
3. Application specific counters. For example, collision-counts in hashmap application

This is an effort to extend existing [GoogleBenchmark library](https://github.com/google/benchmark), to collect hardware and software counters while writing (micro)benchmarks.

In order to add performace counter collection in GBenchmark library, we integrated it with [PAPI](https://icl.utk.edu/papi/) (performance api). 

To use this extension follow below to setup and see usage with some examples.
```

## Setup 

* Setup GoogleBenchmark with Performance Counter changes
```bash
# Check out the library.
$ git clone --branch benchmark-with-performance-collection https://github.com/google/nhprog/benchmark.git
# Benchmark requires Google Test as a dependency. Add the source tree as a subdirectory.
$ git clone https://github.com/google/googletest.git benchmark/googletest
# Go to the library root directory
$ cd benchmark
# Make a build directory to place the build output.
$ mkdir build && cd build
# Generate a Makefile with cmake.
# Use cmake -G <generator> to generate a different file type.
$ cmake ../
# Build the library.
# Use make -j<number_of_parallel_jobs> to speed up the build process, e.g. make -j8 .
$ make
```
* Setup PAPI library
```bash
# CLone PAPI library
$ git clone https://bitbucket.org/icl/papi.git

$ cd papi
NOTE: the configure and make files are located in the papi/src directory.

General Installation

1.      % ./configure
        % make

2.      Check for errors. --> (OPTIONAL STEP)

        a) Run a simple test case: (This will run ctests/zero)

        % make test

        If you get good counts, you can optionally run all the test programs                                                                                                        with the included test harness. This will run the tests in quiet mode,
        which will print PASSED, FAILED, or SKIPPED. Tests are SKIPPED if the
        functionality being tested is not supported by that platform.

        % make fulltest (This will run ./run_tests.sh)

        To run the tests in verbose mode:

        % ./run_tests.sh -v

3.      Create a PAPI binary distribution or install PAPI directly.

        % make install
```


## Usage

# How to write simple microbencmark using this extension
```bash
$ Write a simple benchmark as below, for reference [GoogleBenchmark usage](https://github.com/google/benchmark#usage). Lets save it as test.cc
'static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);'

~Note: There is no change in the way benchmark is defined

$ Compile the change:
 'g++ test.cc -lbenchmark -lpthread -lpapi -o test'

$ Now run it with a parameter that will allow to pass the hardware and software events:
 './test1 --benchmark_perf_events=<event1>,<event2>'

   For example: ./test1 --benchmark_perf_events=papi_tot_cyc
   Note: to check available perf events, see output of PAPI_AVAIL

   Output:

   '-------------------------------------------------------------------------------------------
   Benchmark                                 Time             CPU   Iterations UserCounters..
   -------------------------------------------------------------------------------------------
   BM_StringCreation/min_time:0.500       12.8 ns         12.8 ns     46994730 TOT_CYC=23.0023
   '

```

# Complete microbenchmark test in application like SQLITE
```bash

```