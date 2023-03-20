
#ifndef NODE_H
#define NODE_H

#include <iostream>
//#include "edge.h"
using namespace std;

class Node {
public:
  int idx;
  int x;
  int y;

  vector<int> N;
  vector<int> S;
  vector<int> E;
  vector<int> W;

  int parent;           // parent's node index
  vector<int> children; // immediate children's indices
  int size_of_chi;
  int min_dist; // source to sink manhattan dist
  int path_length;
  int detcost_edgePToNode; // Detour cost of edge, from parent to node
  int detcost_edgeNodeToP; // Detour cost of edge, from node to parent
  int cost_edgeToP;        // Cost of edge to parent
  int src_to_sink_dist; // source to sink distance tranversing from the node to
                        // the source through the tree
  int K_t;   // No. of downstream sinks
  int level; // Level in tree
  vector<int>
      nn_edge_detcost; // Detour cost of the edges to the nearest neighbours
  vector<float> nn_sw_cost;
  float sw_cost_min;
  int sw_cost_min_nn_idx;
  vector<vector<int>> swap_space;

  bool conn_to_par;
  vector<int> sp_chil;
  int idx_of_cn_x, idx_of_cn_y;
  int det_cost_node; // Detour cost of the node j = Sum of all the detour costs
                     // of the edges from the source to the node j
  int maxPLToChild;

  Node(){};
  Node(int _idx, int _x, int _y) {
    idx = _idx;
    x = _x;
    y = _y;
    parent = 0;
    min_dist = 0;
    path_length = 0;
    detcost_edgePToNode = -1;
    detcost_edgeNodeToP = -1;
    src_to_sink_dist = 0;
    K_t = 1;
    level = 0;
    conn_to_par = false;
    idx_of_cn_x = 105;
    idx_of_cn_y = 105;

    for (int oct = 0; oct < 8; oct++) {
      nn_edge_detcost.push_back(10000);
      nn_sw_cost.push_back(10000);
    }
  };
  ~Node() {
    nn_edge_detcost.clear();
    nn_sw_cost.clear();
    children.clear();
    for (int i = 0; i < swap_space.size(); ++i)
      swap_space[i].clear();
    swap_space.clear();
    sp_chil.clear();
    N.clear();
    S.clear();
    E.clear();
    W.clear();
  }
};

class Node1 {
public:
  int idx;
  int x;
  int y;
  int parent; // parent's node index

  Node1(){};
  Node1(int _idx, int _x, int _y) {
    idx = _idx;
    x = _x;
    y = _y;
    parent = 0;
  };

  ~Node1() {}
};

#endif
