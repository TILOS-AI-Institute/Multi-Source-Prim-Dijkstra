#include "argument.h"
#include "graph.h"
#include "mymeasure.h"
#include "mystring.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <omp.h>
#include <set>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;

typedef pair<float, float> pii;

#define sz(x) (int)x.size()
#define mp make_pair
#define pb push_back
#define f first
#define s second

const int INF = 2e9;

// ofstream tmpf("pareto.txt");

// O(N)
vector<pii> getPareto(set<pii> s) {
  vector<pii> res;
  float mn = numeric_limits<float>::max();
  for (auto x : s) {
    // does this point work?
    if (x.s >= mn)
      continue;
    res.pb(x);
    mn = x.s;
  }
  return res;
}

vector<float> getCosts(vector<pii> a, vector<pii> b) {

  vector<float> res;
  for (int i = 0; i < sz(b); i++)
    res.pb(0);

  set<pii> A;
  for (int i = 0; i < sz(a); i++)
    A.insert(a[i]);

  for (int i = 0; i < sz(b); i++) {
    if (binary_search(a.begin(), a.end(), b[i]))
      continue;

    float mn = numeric_limits<float>::max();
    for (int j = 0; j + 1 < sz(a); j++) {
      float y1 = a[j + 1].s;
      float y2 = a[j].s;
      float x1 = a[j + 1].f;
      float x2 = a[j].f;

      float m = (y2 - y1) / (x2 - x1);

      float nx = (b[i].s - y1) / m + x1;
      float ny = y1 + m * (b[i].f - x1);

      float deltaX = max(b[i].f, nx) - min(b[i].f, nx);
      float deltaY = max(b[i].s, ny) - min(b[i].s, ny);

      mn = min(mn, sqrt((deltaX + deltaY)));
    }
    res[i] = mn;
  }
  return res;
}

int main(int argc, char *argv[]) {

  CArgument argument;
  CMeasure measure;

  bool para = true;
  para = argument.argument(argc, argv);
  if (!para) {
    return 0;
  }

  class number {
  public:
    int val;
  };

  int currentNetNum = 0;
  vector<Graph *> my_graphs;
  int num_nets = argument.num_nets;

  // Read input files
  ifstream inputFile;
  ofstream fout;
  inputFile.open(argument.input_file.c_str(), fstream::in);
  fout.open(argument.outFile.c_str(), ofstream::out);
  cout << argument.input_file.c_str() << "\n";
  cout << argument.outFile.c_str() << "\n";
  fout << "[";
  fout << "[";
  fout << "{";

  string line, l, tmp;
  vector<int> x, y;
  int tmp_x = 0, tmp_y = 0;
  int tmp_t = 0;

  while (getline(inputFile, line)) {
    istringstream ss(line);
    vector<string> values;
    while (ss) {
      string next;
      if (!getline(ss, next, ' '))
        break;
      values.push_back(next);
    }
    if (values.size() == 3 && values[0] == "Net") {
      if (argument.verbose > 3) {
        fout << values[0] << " " << values[1] << " --- " << values[2] << endl;
      }
      int term_cnt = 0;
      while (1) {
        getline(inputFile, l);
        istringstream ss_l(l);
        if (l.empty()) {
          break;
        }
        ss_l >> tmp_t >> tmp_x >> tmp_y;
        if (argument.verbose > 3) {
          fout << tmp_x << " " << tmp_y << endl;
        }
        bool flag = false;
        for (int i = 0; i < x.size(); ++i) {
          if (tmp_x == x[i] && tmp_y == y[i]) {
            flag = true;
          }
        }
        if (flag)
          continue;
        x.push_back(tmp_x);
        y.push_back(tmp_y);
        term_cnt++;
      }

      vector<pii> coor;
      vector<vector<int>> ind;
      vector<pair<float, pair<pii, pii>>> res;
      vector<pair<int, pii>> seeds;

      if (currentNetNum) {
        fout << "]";
        fout << "},";
      }

      auto start = high_resolution_clock::now();

      vector<pair<int, int>> closestV;
      for (int i = 1; i < (int)x.size(); i++) {
        int dist = abs(x[0] - x[i]) + abs(y[0] - y[i]);
        closestV.push_back(make_pair(dist, i));
      }
      sort(begin(closestV), end(closestV));

      vector<pair<int, int>> VP;
      for (int i = 0; i + 1 < (int)x.size(); i++) {

        int w_v_i = 0;

        vector<int> dummy;
        for (int j = 0; j + 1 < (int)x.size(); j++)
          if (closestV[j].s != closestV[i].s) {
            int dX = abs(x[closestV[j].s] - x[closestV[i].s]);
            int dY = abs(y[closestV[j].s] - y[closestV[i].s]);
            dummy.push_back(dX + dY);
          }
        sort(begin(dummy), end(dummy));

        for (int k = 0; k < min((int)dummy.size(), argument.KNN); k++)
          w_v_i += dummy[k];

        VP.push_back(make_pair(w_v_i, closestV[i].s));
      }

      set<int> candidates;
      vector<pair<pair<int, pair<int, int>>, pair<int, int>>> smallestD;
      for (int i = 0; i < (int)VP.size(); i++) {
        candidates.insert(VP[i].s);
        for (int j = i + 1; j < sz(VP); j++) {
          int dX = abs(x[VP[i].s] - x[VP[j].s]);
          int dY = abs(y[VP[i].s] - y[VP[j].s]);
          smallestD.push_back(
              make_pair(make_pair(dX + dY, make_pair(VP[i].f, VP[j].f)),
                        make_pair(VP[i].s, VP[j].s)));
        }
      }
      sort(begin(smallestD), end(smallestD));

      cout << "Initial size: " << (int)candidates.size() << "\n";

      int ptr = 0;
      while (ptr < (int)smallestD.size() &&
             (int)candidates.size() > argument.lookAt) {
        while (!(candidates.count(smallestD[ptr].s.f) &&
                 candidates.count(smallestD[ptr].s.s)) &&
               ptr < (int)smallestD.size())
          ptr++;
        if (ptr == (int)smallestD.size())
          break;

        if (candidates.count(smallestD[ptr].s.f) &&
            candidates.count(smallestD[ptr].s.s)) {
          // remove the one with the smalles weight.
          if (smallestD[ptr].f.s.f < smallestD[ptr].f.s.s) {
            candidates.erase(smallestD[ptr].s.s);
          } else {
            candidates.erase(smallestD[ptr].s.f);
          }
        }
      }

      /* find the center most point by minimum manhattan distance to the center
       * of the bounding box. */
      int centerV = 0, smallestDist = INF;
      // bounding box of all points
      int L_x = INF, L_y = INF, R_x = 0, R_y = 0;
      for (int i = 0; i < (int)x.size(); i++) {
        L_x = min(L_x, x[i]);
        L_y = min(L_y, y[i]);
        R_x = max(R_x, x[i]);
        R_y = max(R_y, y[i]);
      }

      int idealX = (R_x + L_x) / 2, idealY = (R_y + L_y) / 2;
      for (int i = 1; i < (int)x.size(); i++) {
        int curD = abs(x[i] - idealX) + abs(y[i] - idealY);
        if (curD == smallestDist) {
          if (candidates.count(i))
            continue;
          centerV = i;
        } else if (curD < smallestDist) {
          centerV = i;
          smallestDist = curD;
        }
      }
      candidates.insert(centerV);

      vector<int> seedCandidates;
      cout << "Seed Candidates: \n";
      for (auto x : candidates) {
        cout << x << " ";
        seedCandidates.push_back(x);
      }
      cout << "\n";

      int curNum = 0;
      double nWL = 0, nSkew = 0;
      // cout << "Start No Seeds\n";
      for (int alp = 0; alp <= 12; alp++) {
        float alpt = ((float)alp / 10);
        if (alp == 11) alpt = 0.0;
        if (alp == 12) alpt = 1.0;
        my_graphs.push_back(new Graph(
            term_cnt, argument.verbose, alpt, alpt, alpt, alpt, 0,
            argument.beta, argument.margin, 0, 0, 0, argument.distance, x, y));

        if (alp == 0) {
          for (int z = 0; z < my_graphs[0]->nodes.size(); z++)
            coor.pb(mp(my_graphs[0]->nodes[z].x, my_graphs[0]->nodes[z].y));
        }

        // cout << "Building " << curNum << "s nearest neighbors.\n";
        my_graphs[0]->buildNearestNeighborsForSPT(my_graphs[0]->num_terminals);
        cout << "Running PD on " << curNum << ".\n";
        my_graphs[0]->run_PD_brute_force(my_graphs[0]->alpha1);
        if (alp <= 10) my_graphs[0]->doSteiner_HoVW();
        if (alp <= 10) my_graphs[0]->fix_max_dc();
        // cout << my_graphs[0]->alpha1 << "\n";
        // cout << my_graphs[0]->pd_wl << "\n";
        // cout << "Running PD-II on " << curNum  << ".\n";
        // my_graphs[0]-> PDBU_new_NN();

        vector<int> v;
        for (int k = 0; k < my_graphs[0]->nodes.size(); k++)
          v.pb(my_graphs[0]->nodes[k].parent);
        ind.pb(v);

        if (alp <= 10) {
          res.pb(mp(alpt, mp(mp(my_graphs[0]->pd_wl, my_graphs[0]->pd_skew),
                             mp(0, 0))));
          seeds.pb(mp(0, mp(0, 0)));
        }
        if (alp == 11) {
          nWL = (double)my_graphs[0]->pd_wl;
        }
        if (alp == 12) {
          nSkew = (double)my_graphs[0]->pd_skew;
        }
        /* Save Memory */
        delete my_graphs[my_graphs.size() - 1];
        my_graphs[my_graphs.size() - 1] = nullptr;
        my_graphs.pop_back();
        curNum++;
      }
      for (int i = 0; i <= 10; i++) {
        res[i].s.s.f = res[i].s.f.f / nWL;
        res[i].s.s.s = res[i].s.f.s / nSkew;
      }

      // cout << "End no seeds\n";

      curNum = 0;
      // cout << "Start seeding with 1\n";
      // create a graph seeding every vertex to the root.
      for (int i = 0; i < (int)seedCandidates.size(); i++) {
        for (int alp = 0; alp <= 10; alp++) {
          float alpt = ((float)alp / 10);
          my_graphs.push_back(new Graph(term_cnt, argument.verbose, alpt, alpt,
                                        alpt, alpt, 0, argument.beta,
                                        argument.margin, seedCandidates[i], 0,
                                        0, argument.distance, x, y));

          /* Run subroutines */
          // cout << "Building " << curNum << "s nearest neighbors.\n";
          my_graphs[0]->buildNearestNeighborsForSPT(
              my_graphs[0]->num_terminals);
          // cout << "Running PD on " << curNum  << ".\n";
          my_graphs[0]->run_PD_brute_force(my_graphs[0]->alpha1);
          my_graphs[0]->doSteiner_HoVW();
          my_graphs[0]->fix_max_dc();
          // cout << "Running PD-II on " << curNum  << ".\n";
          // my_graphs[0]-> PDBU_new_NN();

          vector<int> v;
          for (int k = 0; k < my_graphs[0]->nodes.size(); k++)
            v.pb(my_graphs[0]->nodes[k].parent);
          ind.pb(v);

          res.pb(mp(alpt, mp(mp(my_graphs[0]->pd_wl, my_graphs[0]->pd_skew),
                             mp(my_graphs[0]->pd_wl / nWL,
                                my_graphs[0]->pd_skew / nSkew))));
          seeds.pb(mp(my_graphs[0]->seed1, mp(0, 0)));

          /* Save Memory */
          delete my_graphs[my_graphs.size() - 1];
          my_graphs[my_graphs.size() - 1] = nullptr;
          my_graphs.pop_back();
          curNum++;
        }
      }
      // cout << "End seeding with 1\n";

      curNum = 0;
      // cout << "Start seeding with 2\n";
      // create a graph for every pair of seeds
      for (int i = 0; i < (int)seedCandidates.size(); i++) {
        for (int j = i + 1; j < (int)seedCandidates.size(); j++) {
          for (int alp = 0; alp <= 10; alp++) {
            float alpt = ((float)alp / 10);
            my_graphs.push_back(
                new Graph(term_cnt, argument.verbose, alpt, alpt, alpt, alpt, 0,
                          argument.beta, argument.margin, seedCandidates[i],
                          seedCandidates[j], 0, argument.distance, x, y));

            // cout << "Building " << curNum << "s nearest neighbors.\n";
            my_graphs[0]->buildNearestNeighborsForSPT(
                my_graphs[0]->num_terminals);
            // cout << "Running PD on " << curNum  << ".\n";
            my_graphs[0]->run_PD_brute_force(my_graphs[0]->alpha1);
            my_graphs[0]->doSteiner_HoVW();
            my_graphs[0]->fix_max_dc();
            // cout << "Running PD-II on " << curNum  << ".\n";
            // my_graphs[0]-> PDBU_new_NN();

            vector<int> v;
            for (int k = 0; k < my_graphs[0]->nodes.size(); k++)
              v.pb(my_graphs[0]->nodes[k].parent);
            ind.pb(v);

            res.pb(mp(alpt, mp(mp(my_graphs[0]->pd_wl, my_graphs[0]->pd_skew),
                               mp(my_graphs[0]->pd_wl / nWL,
                                  my_graphs[0]->pd_skew / nSkew))));
            seeds.pb(mp(my_graphs[0]->seed1, mp(my_graphs[0]->seed2, 0)));

            /* Save Memory */
            delete my_graphs[my_graphs.size() - 1];
            my_graphs[my_graphs.size() - 1] = nullptr;
            my_graphs.pop_back();
            curNum++;
          }
        }
      }
      // cout << "End seeding with 2\n";

      curNum = 0;
      // cout << "Start seeding with 3\n";
      // create a graph for every triplet of seeds
      for (int i = 0; i < (int)seedCandidates.size(); i++) {
        for (int j = i + 1; j < (int)seedCandidates.size(); j++) {
          for (int k = j + 1; k < (int)seedCandidates.size(); k++) {
            for (int alp = 0; alp <= 10; alp++) {
              float alpt = ((float)alp / 10);
              my_graphs.push_back(
                  new Graph(term_cnt, argument.verbose, alpt, alpt, alpt, alpt,
                            0, argument.beta, argument.margin,
                            seedCandidates[i], seedCandidates[j],
                            seedCandidates[k], argument.distance, x, y));

              /* Run subroutines */
              // cout << "Building " << curNum << "s nearest neighbors.\n";
              my_graphs[0]->buildNearestNeighborsForSPT(
                  my_graphs[0]->num_terminals);
              // cout << "Running PD on " << curNum  << ".\n";
              my_graphs[0]->run_PD_brute_force(my_graphs[0]->alpha1);
              my_graphs[0]->doSteiner_HoVW();
              my_graphs[0]->fix_max_dc();
              // cout << "Running PD-II on " << curNum  << ".\n";
              // my_graphs[0]-> PDBU_new_NN();

              vector<int> v;
              for (int k = 0; k < my_graphs[0]->nodes.size(); k++)
                v.pb(my_graphs[0]->nodes[k].parent);
              ind.pb(v);

              res.pb(mp(alpt, mp(mp(my_graphs[0]->pd_wl, my_graphs[0]->pd_skew),
                                 mp(my_graphs[0]->pd_wl / nWL,
                                    my_graphs[0]->pd_skew / nSkew))));
              seeds.pb(mp(my_graphs[0]->seed1,
                          mp(my_graphs[0]->seed2, my_graphs[0]->seed3)));

              /* Save Memory */
              delete my_graphs[my_graphs.size() - 1];
              my_graphs[my_graphs.size() - 1] = nullptr;
              my_graphs.pop_back();
              curNum++;
            }
          }
        }
      }

      // cout << "End seeding with 3\n";

      /* we now have every single pair of (normalized cost, normalized skew) */
      vector<pii> v;
      for (int i = 0; i < sz(res); i++)
        v.pb(res[i].s.s);

      set<pii> s;
      for (int i = 0; i < sz(v); i++)
        s.insert(v[i]);

      /* Get the pareto frontier of the current net. */
      vector<pii> pareto = getPareto(s);
      /* Get the costs to each solution. */
      vector<float> costs = getCosts(pareto, v);

      // tmpf << "Net #" << currentNetNum << "\n";
      // tmpf << "Pareto Points: \n";
      // for (auto x: pareto) tmpf << "  (" << x.f << ", " << x.s << ")\n";

      /* Print to the JSON File. */
      int curptr = 0;
      fout << "\"net" << currentNetNum << "\":{";

      fout << "\"coordinates\":[";
      for (int i = 0; i < sz(coor); i++) {
        if (!i)
          fout << "[" << coor[i].f << "," << coor[i].s << "],";
        else if (i + 1 == sz(coor))
          fout << "[" << coor[i].f << "," << coor[i].s << "]";
        else
          fout << "[" << coor[i].f << "," << coor[i].s << "],";
      }
      fout << "],";

      fout << "\"withoutSeeds\":[";
      for (int i = 0; i <= 10; i++) {
        fout << "{";
        fout << "\"parentIndicies\":[";
        for (int j = 0; j < (int)(ind[curptr].size()); j++) {
          if (j + 1 == (int)(ind[curptr].size()))
            fout << ind[curptr][j] << "],";
          else
            fout << ind[curptr][j] << ",";
        }
        fout << "\"alpha\": " << res[curptr].f << ",";
        fout << "\"wirelength\": " << res[curptr].s.f.f << ",";
        fout << "\"skew\": " << res[curptr].s.f.s << ", ";
        fout << "\"normalizedWirelength\": " << res[curptr].s.s.f << ",";
        fout << "\"normalizedSkew\": " << res[curptr].s.s.s << ",";
        fout << "\"cost\": " << costs[curptr];
        if (i == 10)
          fout << "}";
        else
          fout << "},";
        curptr++;
      }
      fout << "],";

      fout << "\"withOneSeed\":[";
      for (int i = 0; i < (int)seedCandidates.size(); i++) {
        for (int j = 0; j <= 10; j++) {
          fout << "{";
          fout << "\"parentIndicies\":[";
          for (int j = 0; j < (int)(ind[curptr].size()); j++) {
            if (j + 1 == (int)(ind[curptr].size()))
              fout << ind[curptr][j] << "],";
            else
              fout << ind[curptr][j] << ", ";
          }
          fout << "\"alpha\": " << res[curptr].f << ",";
          fout << "\"wirelength\": " << res[curptr].s.f.f << ",";
          fout << "\"skew\": " << res[curptr].s.f.s << ",";
          fout << "\"normalizedWirelength\": " << res[curptr].s.s.f << ",";
          fout << "\"normalizedSkew\": " << res[curptr].s.s.s << ",";
          fout << "\"cost\": " << costs[curptr] << ",";
          fout << "\"seededEdges\": [" << seeds[curptr].f << "]";
          if (i + 1 == (int)seedCandidates.size() && j == 10)
            fout << "}";
          else
            fout << "},";
          curptr++;
        }
      }
      fout << "],";

      fout << "\"withTwoSeeds\":[";
      for (int i = 0; i < (int)seedCandidates.size(); i++) {
        for (int j = i + 1; j < (int)seedCandidates.size(); j++) {
          for (int k = 0; k <= 10; k++) {
            fout << "{";
            fout << "\"parentIndicies\":[";
            for (int j = 0; j < (int)(ind[curptr].size()); j++) {
              if (j + 1 == (int)(ind[curptr].size()))
                fout << ind[curptr][j] << "],";
              else
                fout << ind[curptr][j] << ",";
            }
            fout << "\"alpha\": " << res[curptr].f << ",";
            fout << "\"wirelength\": " << res[curptr].s.f.f << ",";
            fout << "\"skew\": " << res[curptr].s.f.s << ",";
            fout << "\"normalizedWirelength\": " << res[curptr].s.s.f << ",";
            fout << "\"normalizedSkew\": " << res[curptr].s.s.s << ",";
            fout << "\"cost\": " << costs[curptr] << ",";
            fout << "\"seededEdges\":[" << seeds[curptr].f << ","
                 << seeds[curptr].s.f << "]";
            if (i + 1 == (int)seedCandidates.size() - 1 &&
                j + 1 == (int)seedCandidates.size() && k == 10)
              fout << "}";
            else
              fout << "},";
            curptr++;
          }
        }
      }
      fout << "],";

      fout << "\"withThreeSeeds\":[";
      for (int i = 0; i < (int)seedCandidates.size(); i++) {
        for (int j = i + 1; j < (int)seedCandidates.size(); j++) {
          for (int k = j + 1; k < (int)seedCandidates.size(); k++) {
            for (int z = 0; z <= 10; z++) {
              fout << "{";
              fout << "\"parentIndicies\":[";
              for (int j = 0; j < (int)(ind[curptr].size()); j++) {
                if (j + 1 == (int)(ind[curptr].size()))
                  fout << ind[curptr][j] << "],";
                else
                  fout << ind[curptr][j] << ",";
              }
              fout << "\"alpha\":" << res[curptr].f << ",";
              fout << "\"wirelength\":" << res[curptr].s.f.f << ",";
              fout << "\"skew\":" << res[curptr].s.f.s << ",";
              fout << "\"normalizedWirelength\":" << res[curptr].s.s.f << ",";
              fout << "\"normalizedSkew\":" << res[curptr].s.s.s << ",";
              fout << "\"cost\":" << costs[curptr] << ",";
              fout << "\"seededEdges\":[" << seeds[curptr].f << ", "
                   << seeds[curptr].s.f << ", " << seeds[curptr].s.s << "]";
              if (i + 1 == (int)seedCandidates.size() - 2 &&
                  j + 1 == (int)seedCandidates.size() - 1 &&
                  k + 1 == (int)seedCandidates.size() && z == 10)
                fout << "}";
              else
                fout << "},";
              curptr++;
            }
          }
        }
      }
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<milliseconds>(stop - start);
      cout << currentNetNum << " took " << ((duration.count()) / 1000)
           << " seconds to finish.\n";
      currentNetNum++;
      x.clear();
      y.clear();
    }
  }
  inputFile.close();
  fout << "]";
  fout << "}";
  fout << "}";
  fout << "]";
  fout << "]";
  fout.close();
  measure.printMemoryUsage();
  measure.print_clock();
  measure.printMemoryUsage();
  exit(1);
  return 1;
}
