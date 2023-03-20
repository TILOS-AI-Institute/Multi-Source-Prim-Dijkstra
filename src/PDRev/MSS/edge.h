#ifndef EDGE_H
#define EDGE_H

#include "node.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Edge {
public:
  int idx;
  int head;
  int tail;
  int best_shape;       // 0 = lower L, 1 = upper L
  int final_best_shape; // 0 = lower L, 1 = upper L
  int best_ov;
  int lower_ov, upper_ov;
  vector<int> upper_best_config, lower_best_config;
  int lower_idx_of_cn_x, lower_idx_of_cn_y;
  int upper_idx_of_cn_x, upper_idx_of_cn_y;

  vector<Node> STNodes;
  vector<Node> lower_sps_to_be_added_x, lower_sps_to_be_added_y;
  vector<Node> upper_sps_to_be_added_x, upper_sps_to_be_added_y;

  Edge(){};
  Edge(int _idx, int _head, int _tail) {
    idx = _idx;
    head = _head;
    tail = _tail;
    lower_ov = 0;
    upper_ov = 0;
    best_ov = 0;
    best_shape = 5;
    final_best_shape = 5;
    lower_idx_of_cn_x = 9999999;
    lower_idx_of_cn_y = 9999999;
    upper_idx_of_cn_x = 9999999;
    upper_idx_of_cn_y = 9999999;
  };

  ~Edge() { STNodes.clear(); }
};

#endif
