#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;

const int MX = 1e5 + 5;
const int INF = 1e9;

typedef pair<int, int> pii;

#define f first
#define s second
#define mp make_pair
#define pb push_back
#define sz(x) (int)x.size()

ifstream fin;
ofstream fout;

int orig_num_nets, X[MX], Y[MX];
int key[MX], PL[MX], par[MX];
int parCnt[MX], visited[MX], cnt[MX], DT[MX];
int nWL, nSkew;
vector<pair<pii, int>> points;
set<int> adj[MX], subTree[MX];
vector<int> nn[MX], sN[MX];

void enterInput() {
  // erase everything before
  for (int i = 0; i < sz(points); i++) {
    sN[i].clear();
    adj[i].clear(), nn[i].clear(), subTree[i].clear();
    key[i] = PL[i] = INF;
    par[i] = visited[i] = cnt[i] = parCnt[i] = DT[i] = 0;
  }
  points.clear();
  for (int i = 0; i < orig_num_nets; i++)
    points.pb(mp(mp(X[i], Y[i]), i));
}

int getDist(int i, int j) { return abs(X[i] - X[j]) + abs(Y[i] - Y[j]); }

int calcWL() {
  int sum = 0;
  for (int i = 0; i < sz(points); i++) {
    for (auto x : adj[i])
      sum += getDist(i, x);
  }
  return sum;
}

void dfs(int at, int dist) {
  subTree[at].insert(at);
  PL[at] = dist, parCnt[at]++;
  cnt[at]++;
  for (auto x : adj[at])
    if (x != at) {
      dfs(x, dist + getDist(at, x));
      for (auto v : subTree[x])
        subTree[at].insert(v);
      cnt[at] += cnt[x];
    }
}

int calcSkew() {
  for (int i = 0; i < sz(points); i++) {
    cnt[i] = PL[i] = parCnt[i] = 0;
    subTree[i].clear();
  }
  dfs(0, 0);

  int mx = 0, mn = 1000000000;
  for (int i = 1; i < orig_num_nets; i++) {
    mx = max(mx, PL[i]), mn = min(mn, PL[i]);
  }

  return mx - mn;
}

void getDT(int at, int par) {
  DT[at] = PL[at] - getDist(at, 0);
  for (auto x : adj[at])
    if (x != par) {
      getDT(x, at);
    }
}

vector<int> getParents() {
  vector<int> ret;
  for (int i = 0; i < sz(points); i++) {
    ret.pb(par[i]);
    ret.pb(X[i]);
    ret.pb(Y[i]);
  }
  return ret;
}

void getNearestNeighbors() {
  int nSize = sz(points);
  vector<int> ur(nSize, numeric_limits<int>::max());
  vector<int> ul(nSize, numeric_limits<int>::min());
  vector<int> lr(nSize, numeric_limits<int>::max());
  vector<int> ll(nSize, numeric_limits<int>::min());

  // sort by y-coordinate
  vector<pair<pii, int>> sorted;
  // (y, x, idx)
  for (int i = 0; i < sz(points); i++)
    sorted.pb(mp(mp(points[i].f.s, points[i].f.f), points[i].s));
  for (int i = 0; i < sz(points); i++)
    nn[i].clear();
  sort(begin(sorted), end(sorted));

  for (int i = 0; i < sz(points); i++) {

    int a = sorted[i].s, x1 = sorted[i].f.s;

    for (int j = 0; j < i; j++) {
      int b = sorted[j].s, x2 = sorted[j].f.s;
      if (x2 <= x1 && x1 < ur[b]) {
        ur[b] = x1;
        nn[b].pb(a);
      } else if (ul[b] < x1 && x1 < x2) {
        ul[b] = x1;
        nn[b].pb(a);
      }
    }

    for (int j = i - 1; j >= 0; j--) {
      int b = sorted[j].s, x2 = sorted[j].f.s;
      if (x1 <= x2 && x2 < lr[a]) {
        lr[a] = x2;
        nn[a].pb(b);
      } else if (ll[a] < x2 && x2 < x1) {
        ll[a] = x2;
        nn[a].pb(b);
      }
    }
  }
}

vector<int> bestSteinerNode(int idx) {
  int v = idx, a = 0, b = 0, x = X[idx], y = Y[idx], best = 0;
  int bX = 0, bY = 0;

  for (auto A : adj[idx])
    if (A != idx) {

      for (auto B : adj[idx])
        if (B != idx && A != B) {

          int sX = X[idx], sY = Y[idx];

          if (min(X[A], X[B]) > X[idx]) {
            sX = min(X[A], X[B]);
          }

          else if (max(X[A], X[B]) < X[idx]) {
            sX = max(X[A], X[B]);
          }

          if (min(Y[A], Y[B]) > Y[idx]) {
            sY = min(Y[A], Y[B]);
          } else if (max(Y[A], Y[B]) < Y[idx]) {
            sY = max(Y[A], Y[B]);
          }

          int gain = abs(X[idx] - sX) + abs(Y[idx] - sY);
          if (gain > best) {
            best = gain, v = idx, a = A, b = B, bX = sX, bY = sY;
          }
        }
    }

  vector<int> ret;

  ret.pb(best);
  // par, child, child
  ret.pb(v);
  ret.pb(a);
  ret.pb(b);
  // x, y of new steiner
  ret.pb(bX);
  ret.pb(bY);
  return ret;
}

void steinerize() {
  set<vector<int>> pq;

  for (int i = 1; i < sz(points); i++) {
    vector<int> newNode = bestSteinerNode(i);
    pq.insert(newNode);
    sN[i] = newNode;
  }

  while (sz(pq)) {
    vector<int> cur = *pq.rbegin();
    pq.erase(cur);
    if (cur[0] == 0)
      break;

    int a = cur[2], b = cur[3];
    int steinerNode = sz(points);

    if (X[a] == cur[4] && Y[a] == cur[5])
      steinerNode = a;
    else if (X[b] == cur[4] && Y[b] == cur[5])
      steinerNode = b;
    else {
      points.pb(mp(mp(cur[4], cur[5]), steinerNode));
      X[steinerNode] = cur[4], Y[steinerNode] = cur[5];
    }

    if (a != steinerNode) {
      adj[cur[1]].erase(a);
      adj[cur[1]].insert(steinerNode);
      par[steinerNode] = cur[1];
      adj[steinerNode].insert(a);
      par[a] = steinerNode;
      par[steinerNode] = cur[1];
    }

    if (b != steinerNode) {
      adj[cur[1]].erase(b);
      adj[cur[1]].insert(steinerNode);
      par[steinerNode] = cur[1];
      adj[steinerNode].insert(b);
      par[b] = steinerNode;
      par[steinerNode] = cur[1];
    }

    vector<int> nS = bestSteinerNode(cur[1]);
    sN[cur[1]] = nS;
    pq.insert(nS);

    if (pq.count(sN[a]))
      pq.erase(sN[a]);
    if (pq.count(sN[b]))
      pq.erase(sN[b]);

    nS = bestSteinerNode(a);
    pq.insert(nS);
    sN[a] = nS;

    nS = bestSteinerNode(b);
    pq.insert(nS);
    sN[b] = nS;
  }
}

void DAS(int s1, int s2, int s3) {
  getNearestNeighbors();

  for (int i = 0; i < sz(points); i++)
    subTree[i].clear();
  for (int i = 0; i < sz(points); i++)
    parCnt[i] = cnt[i] = PL[i] = 0;
  dfs(0, 0);

  int mxPL = 0;
  for (int i = 0; i < orig_num_nets; i++)
    mxPL = max(mxPL, PL[i]);

  for (int i = 1; i < sz(points); i++)
    if (i != s1 && i != s2 && i != s3) {
      int edgeLen = getDist(par[i], i);
      int bestNN = par[i];

      if (bestNN >= orig_num_nets)
        continue;

      for (auto x : nn[i])
        if (x != 0) {
          if (getDist(x, i) <= edgeLen && PL[i] <= 0.5 * mxPL) {
            if (!subTree[i].count(x)) {
              bestNN = x;
              edgeLen = getDist(x, i);
            }
          }
        }

      adj[par[i]].erase(i);
      par[i] = bestNN;
      adj[par[i]].insert(i);

      for (int j = 0; j < sz(points); j++)
        subTree[j].clear();
      for (int j = 0; j < sz(points); j++)
        PL[j] = parCnt[j] = cnt[j] = 0;
      dfs(0, 0);
    }

  for (int i = 0; i < sz(points); i++) {
    subTree[i].clear();
    parCnt[i] = cnt[i] = PL[i] = 0;
  }

  dfs(0, 0);
  getDT(0, 0);

  mxPL = 0;
  int curDT = 0, curWL = calcWL();
  for (int i = 0; i < orig_num_nets; i++)
    mxPL = max(mxPL, PL[i]);
  for (int i = 0; i < sz(points); i++)
    curDT += DT[i];

  for (int i = 1; i < sz(points); i++)
    if (i != s1 && i != s2 && i != s3) {
      int newPar = par[i];

      if (newPar >= orig_num_nets)
        continue;

      for (auto x : nn[i])
        if (x != 0) {
          int newWL = curWL - getDist(i, par[i]) + getDist(i, x);
          int newDT =
              curDT - (getDist(i, par[i]) * cnt[i]) + (getDist(i, x) * cnt[i]);
          if (newWL <= curWL && newDT <= curDT) {
            if (!subTree[i].count(x)) {
              curWL = newWL, curDT = newDT;
              newPar = x;
            }
          }
        }

      adj[par[i]].erase(i);
      par[i] = newPar;
      adj[par[i]].insert(i);

      for (int j = 0; j < sz(points); j++) {
        subTree[j].clear();
        parCnt[j] = cnt[j] = PL[j] = 0;
      }
      dfs(0, 0);
      getDT(0, 0);
    }

  for (int i = 0; i < sz(points); i++) {
    subTree[i].clear();
    parCnt[i] = cnt[i] = PL[i] = 0;
  }
  dfs(0, 0);

  for (int i = 0; i < sz(points); i++)
    if (parCnt[i] != 1)
      cout << "error\n";
}

void PrimDijktra(float alp, int s1, int s2, int s3, int T) {

  // weight (w. alpha), pathlength, idx, par_idx
  set<pair<pii, pii>> pq;
  pq.insert(mp(mp(0, 0), mp(0, 0)));
  key[0] = PL[0] = par[0] = 0;
  for (auto x : {s1, s2, s3})
    if (x != 0) {
      key[x] = PL[x] = par[x] = 0;
      pq.insert(mp(mp(key[x], PL[x]), mp(x, 0)));
    }

  if (sz(pq) > 1) {
    visited[0] = 1;
    pq.erase(mp(mp(key[0], PL[0]), mp(0, 0)));
  }

  while (sz(pq)) {

    pair<pii, pii> fr = *pq.begin();
    pq.erase(pq.begin());
    visited[fr.s.f] = 1;

    for (auto x : nn[fr.s.f]) {
      int len = getDist(fr.s.f, x), pl = len + fr.f.s,
          weight = alp * fr.f.s + len;
      if (!visited[x] && weight <= key[x]) {
        if (pq.count(mp(mp(key[x], PL[x]), mp(x, par[x]))))
          pq.erase(mp(mp(key[x], PL[x]), mp(x, par[x])));
        key[x] = weight;
        PL[x] = pl;
        par[x] = fr.s.f;
        pq.insert(mp(mp(key[x], PL[x]), mp(x, par[x])));
      }
    }
  }

  for (int i = 0; i < orig_num_nets; i++)
    adj[par[i]].insert(i);

  if (T) {
    steinerize();
    DAS(s1, s2, s3);
  }
}

void solve() {
  fin >> orig_num_nets;
  for (int i = 0; i < orig_num_nets; i++) {
    int idx;
    fin >> idx;
    int x, y;
    fin >> x >> y;
    X[idx] = x, Y[idx] = y;
    nn[i].clear();
  }

  enterInput();
  enterInput();
  getNearestNeighbors();
  PrimDijktra(0, 0, 0, 0, 0);
  nWL = calcWL();

  enterInput();
  getNearestNeighbors();
  PrimDijktra(1, 0, 0, 0, 0);
  nSkew = calcSkew();

  // cout << nWL << " " << nSkew << "\n";

  fout << "\"withoutSeeds\": [\n";
  for (int alp = 0; alp <= 10; alp++) {
    float a = (float)alp / 10;
    enterInput();
    getNearestNeighbors();
    PrimDijktra(a, 0, 0, 0, 1);
    int wl = calcWL();
    int skew = calcSkew();
    vector<int> parInfo = getParents();
    fout << "{\n";
    fout << "\"parentIndicies\": [";
    for (int x = 0; x < sz(parInfo); x++) {
      fout << parInfo[x] << (x + 1 == sz(parInfo) ? "],\n" : ",");
    }
    fout << "\"alpha\": " << a << ",\n";
    fout << "\"wirelength\": " << wl << ",\n";
    fout << "\"skew\": " << skew << ",\n";
    fout << "\"normalizedWirelength\": " << (float)wl / nWL << ",\n";
    fout << "\"normalizedSkew\": " << (float)skew / nSkew << "\n";
    fout << "}" << (alp == 10 ? "\n" : ",\n");
  }
  fout << "],";

  fout << "\"withOneSeed\": [\n";
  for (int alp = 0; alp <= 10; alp++) {
    float a = (float)alp / 10;
    for (int i = 1; i < orig_num_nets; i++) {
      enterInput();
      getNearestNeighbors();
      PrimDijktra(a, i, 0, 0, 1);
      int wl = calcWL();
      int skew = calcSkew();
      vector<int> parInfo = getParents();
      fout << "{\n";
      fout << "\"parentIndicies\": [";
      for (int x = 0; x < sz(parInfo); x++) {
        fout << parInfo[x] << (x + 1 == sz(parInfo) ? "],\n" : ",");
      }
      fout << "\"alpha\": " << a << ",\n";
      fout << "\"wirelength\": " << wl << ",\n";
      fout << "\"skew\": " << skew << ",\n";
      fout << "\"normalizedWirelength\": " << (float)wl / nWL << ",\n";
      fout << "\"normalizedSkew\": " << (float)skew / nSkew << ",\n";
      fout << "\"seededEdges\": [" << i << "]\n";
      fout << "}" << (alp == 10 && i + 1 == orig_num_nets ? "\n" : ",\n");
      // vector<int>().swap(parInfo);
    }
  }
  fout << "],";

  fout << "\"withTwoSeeds\": [\n";
  for (int alp = 0; alp <= 10; alp++) {
    float a = (float)alp / 10;
    for (int i = 1; i < orig_num_nets; i++) {
      for (int j = i + 1; j < orig_num_nets; j++) {
        enterInput();
        getNearestNeighbors();
        PrimDijktra(a, i, j, 0, 1);
        int wl = calcWL();
        int skew = calcSkew();
        vector<int> parInfo = getParents();
        fout << "{\n";
        fout << "\"parentIndicies\": [";
        for (int x = 0; x < sz(parInfo); x++) {
          fout << parInfo[x] << (x + 1 == sz(parInfo) ? "],\n" : ",");
        }
        fout << "\"alpha\": " << a << ",\n";
        fout << "\"wirelength\": " << wl << ",\n";
        fout << "\"skew\": " << skew << ",\n";
        fout << "\"normalizedWirelength\": " << (float)wl / nWL << ",\n";
        fout << "\"normalizedSkew\": " << (float)skew / nSkew << ",\n";
        fout << "\"seededEdges\": [" << i << "," << j << "]\n";
        fout << "}"
             << (alp == 10 && i == orig_num_nets - 2 && j == orig_num_nets - 1
                     ? "\n"
                     : ",\n");
      }
    }
  }
  fout << "],";

  fout << "\"withThreeSeeds\": [\n";
  for (int alp = 0; alp <= 10; alp++) {
    float a = (float)alp / 10;
    for (int i = 1; i < orig_num_nets; i++) {
      for (int j = i + 1; j < orig_num_nets; j++) {
        for (int k = j + 1; k < orig_num_nets; k++) {
          enterInput();
          getNearestNeighbors();
          PrimDijktra(a, i, j, k, 1);
          int wl = calcWL();
          int skew = calcSkew();
          vector<int> parInfo = getParents();
          fout << "{\n";
          fout << "\"parentIndicies\": [";
          for (int x = 0; x < sz(parInfo); x++) {
            fout << parInfo[x] << (x + 1 == sz(parInfo) ? "],\n" : ",");
          }

          fout << "\"alpha\": " << a << ",\n";
          fout << "\"wirelength\": " << wl << ",\n";
          fout << "\"skew\": " << skew << ",\n";
          fout << "\"normalizedWirelength\": " << (float)wl / nWL << ",\n";
          fout << "\"normalizedSkew\": " << (float)skew / nSkew << ",\n";
          fout << "\"seededEdges\": [" << i << "," << j << "," << k << "]\n";
          fout << "}"
               << (alp == 10 && i == orig_num_nets - 3 &&
                           j == orig_num_nets - 2 && k == orig_num_nets - 1
                       ? "\n"
                       : ",\n");
        }
      }
    }
  }
  fout << "]\n";
}

int main(int argc, char *argv[]) {
  fin.open(argv[2]), fout.open(argv[4]);
  int T;
  fin >> T;
  fout << "[\n"
       << "[\n"
       << "{\n";
  for (int t = 0; t < T; t++) {
    fout << "\"net" << t << "\": {\n";
    solve();
    fout << "}" << (t + 1 == T ? "\n" : ",\n");
  }
  fout << "}\n"
       << "]\n"
       << "]\n";
  fin.close(), fout.close();
}
