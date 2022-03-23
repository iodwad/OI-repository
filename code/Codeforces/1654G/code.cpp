// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 2e5;

int n, a[MAXN + 5];
int h[MAXN + 5], d[MAXN + 5];
bool tag[MAXN + 5];
vector<int> e[MAXN + 5];
vector<int> V;

void BFS() {
  memset(h, -1, sizeof(h));
  static queue<int> q;
  for (int i = 1; i <= n; ++i) {
    if (a[i] == 1) {
      q.push(i);
      h[i] = 0;
    }
  }
  while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int to : e[u]) {
      if (h[to] == -1) {
        q.push(to);
        h[to] = h[u] + 1;
      }
    }
  }
}

void BFS2(vector<int> &now, int *c) {
  static bool vis[MAXN + 5];
  static queue<int> q;
  for (int x : now) vis[x] = 1, q.push(x);
  while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int to : e[u]) {
      if (h[to] == h[u]) {
        c[to] = min(c[to], c[u] + 1);
        if (!vis[to]) vis[to] = 1, q.push(to), now.push_back(to);
      }
    }
  }
  for (int x : now) vis[x] = 0;
}

int main() {
  // ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1, u, v; i < n; ++i) {
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  BFS();
  // for (int i = 1; i <= n; ++i) cout << h[i] << " \n"[i == n];
  for (int i = 1; i <= n; ++i) {
    for (int to : e[i]) {
      if (h[to] == h[i]) { 
        V.push_back(h[i]); 
        tag[i] = true;
        break; 
      }
    }
  }
  sort(V.begin(), V.end());
  V.erase(unique(V.begin(), V.end()), V.end());
  for (int i = 1; i <= n; ++i) d[i] = h[i];
  for (size_t t = 0; t < V.size(); ++t) {
    static vector<int> now, nxt; 
    static int c[MAXN + 5];
    memset(c, 0x3f, sizeof(c));
    for (int i = 1; i <= n; ++i) if (tag[i] && h[i] == V[t]) now.push_back(i), c[i] = 0;
    for (int i = V[t]; i < n - 1 && !now.empty(); ++i) {
      static bool vis[MAXN + 5];
      nxt.clear();
      BFS2(now, c);
      for (int x : now) {
        for (int to : e[x]) {
          if (h[to] == h[x] + 1) {
            c[to] = min(c[to], max(0, c[x] - 1));
            if (!vis[to]) vis[to] = 1, nxt.push_back(to);
          }
        }
      }
      for (int x : nxt) vis[x] = 0;
      swap(now, nxt);
    }
    for (int i = 1; i <= n; ++i) if (c[i] == 0) d[i] = min(d[i], V[t]);
  }
  for (int i = 1; i <= n; ++i) cout << 2 * h[i] - d[i] << " ";
  return 0;
}
