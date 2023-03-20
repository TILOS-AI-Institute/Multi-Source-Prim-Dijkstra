#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#define f first
#define s second

int M, N, t;
vector<pair<int, int>> pointsets;

auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 mt(seed);

// Method which generates random pointsets in the [0, M] x [0, M] square.
void generatePoints(int cardinality, int max) {
  int random = rand();
  int pointX = mt() % (max + 1) + 1;
  int randomTwo = rand();
  int pointY = mt() % (max + 1) + 1;
  pair<int, int> coordinate;
  coordinate.first = pointX;
  coordinate.second = pointY;
  // Checks the see if a pair(coordinate) is in the vector pointsets. If it is,
  // it recurses back to the beginning of the function to generate a new point.
  if (!(find(pointsets.begin(), pointsets.end(), coordinate) !=
        pointsets.end())) {
    pointsets.push_back(coordinate);
  } else {
    return generatePoints(cardinality, max);
  }
}

int main(int argc, char *argv[]) {
  srand((int)time(
      0)); // is used to generate random numbers each time the program runs
  // srand((unsigned) time(NULL));
  auto start = chrono::steady_clock::now();

  M = stoi(argv[2]);
  N = stoi(argv[4]);
  t = stoi(argv[6]);

  // ofstream fout; fout.open("test.in");
  for (int T = 1; T <= t; T++) {
    for (int i = 0; i < N; i++) {
      generatePoints(N, M);
    }

    int lx = 1000000, rx = 0, ly = 1000000, ry = 0;
    for (auto x : pointsets) {
      lx = min(lx, x.f), rx = max(rx, x.f);
      ly = min(ly, x.s), ry = max(ry, x.s);
    }

    int cX = (lx + rx) / 2, cY = (ly + ry) / 2, dist = 1000000000, idx = 0;

    /* make center-most point be the root
    for (int i = 0; i < (int)pointsets.size(); i++) {
        pair<int, int> x = pointsets[i];
        if (abs(x.f - cX) + abs(x.s - cY) < dist) {
            dist = abs(x.f - cX) + abs(x.s - cY);
            idx = i;
        }
    }

    vector<pair<int, int> > b4, rst;
    for (int i = 0; i < (int)pointsets.size(); i++) {
        if (i < idx) b4.push_back(pointsets[i]);
        else rst.push_back(pointsets[i]);
    }
    pointsets.clear();

    // center-most point becomes the root
    for (auto x: rst) pointsets.push_back(x);
    for (auto x: b4) pointsets.push_back(x);
    */

    /* make center of bbox be the root */
    /*
    bool found = false;
    for (auto x: pointsets) {
        if (x.f == cX && x.s == cY) found = true;
    }

    if (!found) {
        pointsets.push_back(make_pair(cX, cY));
        reverse(begin(pointsets), end(pointsets));
    }
    else {

        // since the center exists, dist = 0
        for (int i = 0; i < (int)pointsets.size(); i++) {
            pair<int, int> x = pointsets[i];
            if (abs(x.f - cX) + abs(x.s - cY) < dist) {
                dist = abs(x.f - cX) + abs(x.s - cY);
                idx = i;
            }
        }

        vector<pair<int, int> > b4, rst;
        for (int i = 0; i < (int)pointsets.size(); i++) {
            if (i < idx) b4.push_back(pointsets[i]);
            else rst.push_back(pointsets[i]);
        }
        pointsets.clear();

        // center-most point becomes the root
        for (auto x: rst) pointsets.push_back(x);
        for (auto x: b4) pointsets.push_back(x);
    }
    */
    cout << "Net net" << T << " " << (int)pointsets.size() << "\n";

    for (int i = 0; i < pointsets.size(); i++) {
      cout << i << " " << pointsets[i].first << " " << pointsets[i].second
           << "\n";
    }
    cout << "\n";
    pointsets.clear();
  }

  auto stop = chrono::steady_clock::now();

  auto diff = stop - start;

  // cout << "This program took "<< chrono::duration <double, milli>
  // (diff).count() << " miliseconds to generate and print all the random
  // pointsets." << '\n';

  return 0;
}
