// by iodwad
#include <cstdio>
#include <iostream>
#include <numeric>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

const int MAXN = 5e3;
const int MAXM = 5e5;

int n, m, Q;
int col[MAXN + 5], tag[MAXN + 5], U[MAXM + 5], V[MAXM + 5];
bool ans[MAXN + 5][MAXN + 5];
char W[MAXN + 5];
vector<int> e[MAXN + 5];

struct DSU {
  int fset[MAXN + 5];
  DSU() { iota(fset, fset + MAXN + 1, 0); }
  int find(int x) { return fset[x] == x ? x : fset[x] = find(fset[x]); }
  bool merge(int x, int y) { 
    if (find(x) == find(y)) return false;
    return fset[find(x)] = find(y), true;
  }
} t0, t1;

void dfs(int x, int y) {
  col[x] = y;
  for (int to : e[x]) {
    if (W[x] != W[to]) continue;
    if (col[to] == col[x]) tag[abs(y)] = 1;
    else if (!col[to]) dfs(to, -y);
  }
}

queue<pair<int, int>> q;
void Push(int x, int y) {
  q.push({x, y});
  ans[x][y] = ans[y][x] = 1;
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m >> Q;
  cin >> (W + 1);
  for (int i = 1; i <= m; ++i) {
    cin >> U[i] >> V[i];
    e[U[i]].push_back(V[i]);
    e[V[i]].push_back(U[i]);
  }
  for (int i = 1; i <= n; ++i) if (!col[i]) dfs(i, i);
  for (int i = 1; i <= n; ++i) e[i].clear();
  for (int i = 1; i <= m; ++i) {
    int u = U[i], v = V[i];
    if (W[u] == W[v]) {
      if (t0.merge(u, v)) {
        e[u].push_back(v);
        e[v].push_back(u);
        Push(u, v);
      }
    } else {
      if (t1.merge(u, v)) {
        e[u].push_back(v);
        e[v].push_back(u);
      }
    }
  }
  for (int i = 1; i <= n; ++i) {
    Push(i, i);
    if (abs(col[i]) == i && tag[i]) {
      cerr << i << "\n";
      e[i].push_back(i);
    }
  }
  while (!q.empty()) {
    int u = q.front().first, v = q.front().second; q.pop();
    for (int to0 : e[u]) 
      for (int to1 : e[v]) 
        if (W[to0] == W[to1] && !ans[to0][to1]) 
          Push(to0, to1);
  }
  while (Q--) {
    int u, v; cin >> u >> v;
    cout << (ans[u][v] ? "YES" : "NO") << "\n";
  }
  return 0;
}
