// by iodwad
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>

#define int long long

using namespace std;

using i64 = long long;

const int MAXN = 50;

int P, Q, R, D;
int a[MAXN + 5][MAXN + 5][MAXN + 5];

namespace Flow {
  const int maxn = MAXN * MAXN * MAXN + 10;
  const int maxm = maxn * 10;
  struct Edge {
    int edge;
    int fst[maxn], to[maxm], f[maxm], nxt[maxm];
    Edge() {
      edge = -1;
      memset(fst, -1, sizeof(fst));
      memset(nxt, -1, sizeof(nxt));
    }
    void _add(int u, int v, int _f) {
      ++edge;
      to[edge] = v; f[edge] = _f;
      nxt[edge] = fst[u]; fst[u] = edge;
    }
    void add(int u, int v, int f) {
      _add(u, v, f);
      _add(v, u, 0);
    }
  } e;
  int S, T, cure[maxn], dep[maxn];
  bool bfs() {
    static queue<int> q;
    memset(dep, 0, sizeof(dep));
    q.push(S); dep[S] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int k = e.fst[u]; ~k; k = e.nxt[k]) {
        int to = e.to[k], f = e.f[k];
        if (f > 0 && !dep[to]) {
          dep[to] = dep[u] + 1;
          q.push(to);
        }
      }
    }
    return dep[T] != 0;
  }
  int Augment(int x, int flow) {
    if (x == T) return flow;
    int rest = flow;
    for (int &k = cure[x]; ~k && flow; k = e.nxt[k]) {
      int to = e.to[k], f = e.f[k];
      if (f > 0 && dep[to] == dep[x] + 1) {
        int tmp = Augment(to, min(rest, f));
        if (!tmp) dep[to] = -1;
        e.f[k] -= tmp;
        e.f[k ^ 1] += tmp;
        rest -= tmp;
      }
    }
    return flow - rest;
  }
  int Dinic() {
    int res = 0;
    while (bfs()) {
      memcpy(cure, e.fst, sizeof(cure));
      int tmp = 0;
      while ((tmp = Augment(S, 1e18))) res += tmp;
    }
    return res;
  }
}

using namespace Flow;

int ID(int x, int y, int z) {
  --x; --y; --z;
  return x * Q * R + y * R + z + 1;
}

signed main() {
  // ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> P >> Q >> R;
  cin >> D;
  for (int z = 1; z <= R; ++z)
    for (int x = 1; x <= P; ++x)
      for (int y = 1; y <= Q; ++y)
        cin >> a[x][y][z];
  for (int x = 1; x <= P; ++x)
    for (int y = 1; y <= Q; ++y)
      for (int z = 1; z <= R; ++z)
        cerr << "x = " << x << " y = " << y << " z = " << z << "id = " << ID(x, y, z) << "\n";
  S = P * Q * R + 1; T = S + 1;
  for (int x = 1; x <= P; ++x) {
    for (int y = 1; y <= Q; ++y) {
      e.add(S, ID(x, y, 1), 1e18);
      for (int z = 1; z <= R; ++z) {
        if (z < R) e.add(ID(x, y, z), ID(x, y, z + 1), a[x][y][z] + 1e9);
        if (z > D) {
          for (int xx = max(1ll, x - 1); xx <= min(P, x + 1); ++xx) {
            for (int yy = max(1ll, y - 1); yy <= min(Q, y + 1); ++yy) {
              if (abs(x - xx) + abs(y - yy) != 1) continue;
              e.add(ID(x, y, z), ID(xx, yy, z - D), 1e18);
            }
          }
        }
      }
      e.add(ID(x, y, R), T, a[x][y][R] + 1e9);
    }
  }
  cout << Dinic() - P * Q * 1e9;
  return 0;
}
