

#ifndef GRAPH_H
#define GRAPH_H

using namespace std;

#include "edge.h"
#include "node.h"
#include <tr1/unordered_map>
#include <vector>

class Graph {
public:
  vector<Node> nodes;
  vector<Edge> edges;
  vector<int> dag;
  int maxPL; // max source to sink pathlength
  float maxPLRatio;
  vector<vector<int>> ManhDist;
  float PLmargin;
  int verbose;
  int num_terminals;
  int orig_num_terminals;
  int root_idx;
  float alpha1;
  int seed1;
  int seed2;
  int seed3;
  float alpha2;
  float alpha3;
  float alpha4;
  float beta;
  int distance;
  float M;

  float pd_wl;
  float pd_pl;
  float pd_skew;
  float mst_wl;
  float spt_pl;
  float pd_dc;
  float pdbu_wl;
  float pdbu_pl;
  float pdbu_dc;
  float pdbu_skew;
  float st_wl;
  float st_pl;
  float st_dc;
  float daf_wl;
  float daf_pl;
  float daf_dc;

  vector<int> urux;
  vector<int> urlx;
  vector<int> ulux;
  vector<int> ullx;
  vector<int> lrux;
  vector<int> lrlx;
  vector<int> llux;
  vector<int> lllx;

  vector<Node> sheared;
  vector<Node> hanan;
  vector<vector<int>> nn;
  float avgNN;
  vector<vector<int>> nn_hanan;
  vector<int> sorted;
  vector<int> sorted_hanan;
  vector<int> aux;

  vector<int> heap_key;
  vector<int> heap_idx;
  vector<int> heap_elt;
  int heap_size;
  vector<vector<int>> tree_struct;
  vector<int> tree_struct_1darr;

  // functions
  Graph(){};
  Graph(int _num_terminals, int _verbose, float _alpha1, float _alpha2,
        float _alpha3, float _alpha4, int _root, float _beta, float _margin,
        int seed1, int seed2, int seed3, int distance, vector<int> &x,
        vector<int> &y);
  ~Graph();
  bool buildNearestNeighborsForSPT(int num_terminals);
  bool buildNearestNeighbors_single_node(int num_terminals, int idx);
  bool run_PD_brute_force(float alp);
  bool doSteiner_HoVW();
  bool fix_max_dc();
  bool find_max_dc_node(vector<float> &node_and_dc);
  int calc_overlap(vector<vector<Node>> &set_of_nodes);
  int calc_ov_x_or_y(vector<Node> &sorted, Node curr_node, char tag[]);
  bool get_overlap_lshape(vector<Node> &set_of_nodes, int index);
  void generate_permutations(vector<vector<int>> lists,
                             vector<vector<int>> &result, int depth,
                             vector<int> current);
  void update_edgecosts_to_parent(int child, int par);
  void update_node_detcost_Kt(int j);
  void get_level_in_tree();
  void PDBU_new_NN();
  void update_detourcosts_to_NNs(int j);
  void swap_and_update_tree(int min_node, int nn_idx, int distance, int i_node);
  float calc_tree_cost();

  void heap_insert(int p, int key);
  int heap_delete_min();
  void heap_decrease_key(int p, float new_key);

  void get_children_of_node();
  void print_tree();
  void print_tree_v2(ofstream &ofs);
  float calc_tree_det_cost();
  int calc_tree_wl_pd();
  int calc_tree_pl();
  int calc_tree_skew();
  void updateMinDist();
  void NESW_NearestNeighbors(int left, int right, int oct);
  void NESW_Combine(int left, int mid, int right, int oct);
  bool make_unique(vector<Node> &vec);

  void BuildDAG();
  void UpdateManhDist();
  bool IsSubTree(int cIdx, int tIdx);
  void UpdateMaxPLToChild(int cIdx);
  void PrintInfo();
  void refineSteiner();
  void refineSteiner2();
  void UpdateSteinerNodes();
  void GetSteinerNodes(int idx, vector<Node> &fSTNodes);
  void GetSteiner(int cIdx, int nIdx, vector<Node> &STNodes);
  bool IsParent(int cIdx, int nIdx);
  bool IsOnEdge(Node &tNode, int idx);
  Node GetCornerNode(int cIdx);
  void DupRemoval(vector<Node> &STNodes);
  void removeChild(Node &pNode, int idx);
  void addChild(Node &pNode, int idx);
  void UpdateEdges(vector<Node> &STNodes);
  void UpdateAllEdgesNSEW();
  void UpdateNSEW(Edge &e);
  int IdentLoc(int cIdx, int tIdx);
  void SortAll(Node &n);
  void SortN(Node &n);
  void SortS(Node &n);
  void SortE(Node &n);
  void SortW(Node &n);
  int DeltaN(int idx, int rIdx, bool isRemove);
  int DeltaS(int idx, int rIdx, bool isRemove);
  int DeltaE(int idx, int rIdx, bool isRemove);
  int DeltaW(int idx, int rIdx, bool isRemove);
  int ComputeWL(int cIdx, int pIdx, bool isRemove, int eShape);
  void AddNode(int cIdx, int pIdx, int eShape);
  void SortCNodes(vector<Node> &cNodes, int cIdx, int pIdx, int eShape);
  void constructSteiner();
  int IsAdded(Node &cN);
  void FreeManhDist();
  void removeNeighbor(int pIdx, int cIdx);
  void removeN(Node &pN, int idx);
  void removeS(Node &pN, int idx);
  void removeE(Node &pN, int idx);
  void removeW(Node &pN, int idx);
  void replaceChild(Node &pNode, int idx, int tIdx);
  void replaceParent(Node &pNode, int idx, int tIdx);
  void RemoveUnneceSTNodes();
  bool IsSameDir(int cIdx, int nIdx);
};

#endif
