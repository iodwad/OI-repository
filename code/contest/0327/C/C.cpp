// by iodwad
#include <cstdio>
#include <iostream>
#include <vector> 
#include <queue>
#include <numeric>
#include <algorithm>

#define int long long

using namespace std;

const int MAXN = 2e5;

struct Edge {
  int u, v, w;
  Edge(int _u = 0, int _v = 0, int _w = 0) : u(_u), v(_v), w(_w) {}
  friend bool operator== (const Edge &lhs, const Edge &rhs) { return lhs.u == rhs.u && lhs.v == rhs.v && lhs.w == rhs.w; }
};

int n;
vector<Edge> ae;
vector<pair<int, int>> e[MAXN + 5];

int all_size, rt, maxs[MAXN + 5], siz[MAXN + 5];
bool vis[MAXN + 5];

void get_root(int x, int fa) {
  siz[x] = 1; maxs[x] = 0;
  for (auto it : e[x]) {
    int to = it.first;
    if (to == fa || vis[to]) continue;
    get_root(to, x);
    siz[x] += siz[to];
    maxs[x] = max(maxs[x], siz[to]);
  }
  maxs[x] = max(maxs[x], all_size - siz[x]);
  if (maxs[x] < maxs[rt]) rt = x;
}

int dep[MAXN + 5];
vector<int> dot[MAXN + 5];

void dfs(int x, int fa, int frm) {
  dot[frm].push_back(x);
  for (auto it : e[x]) {
    int to = it.first, w = it.second;
    if (to == fa || vis[to]) continue;
    dep[to] = dep[x] + w;
    dfs(to, x, frm);
  }
}

struct Heap {
  int x;
  Heap(int _x = 0) : x(_x) {}
  friend bool operator< (const Heap &lhs, const Heap &rhs) { return dot[lhs.x].size() > dot[rhs.x].size(); }
};

int sgn(int x) { return x <= 0 ? 0 : 1; }

void merge(vector<int> &a, vector<int> &b) {
  int asiz = a.size(), bsiz = b.size();
  static vector<int> now[2]; now[0].clear(); now[1].clear();
  for (int i = 0, j = 0; i < asiz || j < bsiz; ) {
    int aa = i < asiz ? a[i] : -1, bb = j < bsiz ? b[j] : -1;
    if (i < asiz && (j == bsiz || -dep[aa] <= dep[bb])) {
      if (j > 0) ae.emplace_back(aa, b[j - 1], dep[aa] + dep[b[j - 1]]);
      for (int k : now[1]) ae.emplace_back(aa, k, dep[aa] + dep[k]);
      now[1].clear();
      now[0].push_back(aa);
      ++i;
    } else {
      if (i > 0) ae.emplace_back(bb, a[i - 1], dep[bb] + dep[a[i - 1]]);
      for (int k : now[0]) ae.emplace_back(bb, k, dep[bb] + dep[k]);
      now[0].clear();
      now[1].push_back(bb);
      ++j;
    }
  }
}

vector<int> Merge(vector<int> &x, vector<int> &y) {
  static vector<int> vec;
  vec.clear();
  int xsiz = x.size(), ysiz = y.size();
  for (int i = 0, j = 0; i < xsiz || j < ysiz; ) {
    if (i < xsiz && (j == ysiz || abs(dep[x[i]]) < abs(dep[y[j]]))) {
      vec.push_back(x[i++]);
    } else {
      vec.push_back(y[j++]);
    }
  }
  return vec;
}

void solve(int x, int y = 0) {
  static priority_queue<Heap> pq;
  vis[x] = 1;
  for (auto it : e[x]) {
    int to = it.first, w = it.second;
    if (vis[to]) continue;
    dep[to] = w; dot[to].clear();
    dfs(to, x, to);
    pq.emplace(to);
    sort(dot[to].begin(), dot[to].end(), [&](int x, int y) { return abs(dep[x]) < abs(dep[y]); });
  }
  while (pq.size() > 1) {
    int t[2];
    t[0] = pq.top().x; pq.pop();
    t[1] = pq.top().x; pq.pop();
    static vector<int> v1, v2; 
    for (int i = 0; i < 2; ++i) {
      v1.clear(); v2.clear();
      for (int j : dot[t[i]]) if (sgn(dep[j]) == 1) v2.push_back(j);
      for (int j : dot[t[i ^ 1]]) if (sgn(dep[j]) == 0) v1.push_back(j);
      merge(v1, v2);
    }
    static vector<int> tmp;
    tmp = Merge(dot[t[0]], dot[t[1]]);
    dot[t[1]] = tmp;
    pq.emplace(t[1]);
  }
  int tmp = 0;
  if (!pq.empty()) {
    tmp = pq.top().x; 
    pq.pop();
  }
  for (int i : dot[tmp]) ae.emplace_back(i, x, dep[i]);
  int lst_size = all_size;
  for (auto it : e[x]) {
    int to = it.first;
    if (vis[to]) continue;
    all_size = siz[to] < siz[x] ? siz[to] : lst_size - siz[x];
    rt = 0; get_root(to, x);
    solve(rt, y + 1);
  }
}

int fset[MAXN + 5];
int find(int x) { return fset[x] == x ? x : fset[x] = find(fset[x]); }

int Kruskal() {
  for (auto &i : ae) i.w = i.w < 0 ? -i.w : i.w;
  sort(ae.begin(), ae.end(), [&](Edge x, Edge y) { return x.w < y.w; });
  ae.erase(unique(ae.begin(), ae.end()), ae.end());
  iota(fset, fset + n + 1, 0);
  int res = 0;
  for (auto i : ae) {
    int u = i.u, v = i.v, w = i.w;
    if (find(u) == find(v)) continue;
    fset[find(u)] = find(v);
    res += w;
  }
  return res;
}

signed main() {
  ios::sync_with_stdio(false); cin.tie(0);
  freopen("C.in", "r", stdin);
  freopen("C.out", "w", stdout);
  cin >> n;
  for (int i = 1, u, v, w; i < n; ++i) {
    cin >> u >> v >> w;
    e[u].emplace_back(v, w);
    e[v].emplace_back(u, w);
  }
  all_size = maxs[0] = n; get_root(1, 0);
  solve(rt);
  cout << Kruskal() << "\n";
  return 0;
}
