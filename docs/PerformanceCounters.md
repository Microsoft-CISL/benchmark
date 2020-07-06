# How to setup 

* Setup GoogleBenchmark with Performance Counter changes
```bash
# Check out the library.
$ git clone --branch benchmark-papi-integration https://github.com/google/nhprog/benchmark.git
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

* Run a quick performance counter usage test
```bash
$ cd performance_counter_test
$ g++ test1.cc -lbenchmark -lpthread -lpapi -o test1
$ ./test1 --benchmark_perf_events=<event1>, <event2>
   example ./test1 --benchmark_perf_events=tot_cyc
   Note: to check available perf events, see output of PAPI_AVAIL
```