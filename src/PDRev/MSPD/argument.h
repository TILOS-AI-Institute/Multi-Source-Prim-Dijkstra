
#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <vector>

using namespace std;

class CArgument {
public:
  string outFile;
  int num_nets;
  int net_num;
  bool runOneNet;
  int num_terminals;
  int verbose;
  float alpha1;
  float alpha2;
  float alpha3;
  float alpha4;
  float margin;
  int seed;
  string input_file;
  int root_idx;
  int hvw_yes;
  int pdbu_yes;
  float beta;
  int distance;
  int delta;
  int lookAt;
  int KNN;
  // functions
  CArgument();
  bool argument(int argc, char *argv[]);
  void help();
};

#endif
