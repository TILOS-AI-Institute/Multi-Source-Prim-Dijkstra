
#ifndef MYMEASURE_H
#define MYMEASURE_H

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class CMeasure {
public:
  clock_t begin;
  vector<string> tasks;
  vector<double> elapsed_secs;

  // functions
  void process_mem_usage();
  void start_clock();
  void stop_clock(string task);
  void print_clock();
  void printMemoryUsage(void);
};

#endif
