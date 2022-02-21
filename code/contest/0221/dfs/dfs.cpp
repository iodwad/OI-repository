// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;

using i64 = long long;

const int MOD = 1e9 + 7;
const int MAXN = 36;
const int MAXL = 18;
const int _2inv = (MOD + 1) / 2;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
void Add(int &x, int y) { x = add(x, y); }
void Sub(int &x, int y) { x = sub(x, y); }

int n, m, C, L;
int e[MAXN + 5][MAXN + 5], pw[MAXN + 5];
int f[1 << MAXL][MAXN + 5], g[1 << MAXL][MAXN + 5], sum[1 << MAXL], ans[1 << MAXL];

bool is_in(int S, int x) { return !!(S & (1 << x)); }
int opp(int x) { return x ^ 1; }

int main() {
  freopen("dfs.in", "r", stdin);
  freopen("dfs.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m >> C;
  if (n % 2 == 1) ++n;
  pw[0] = 1;
  for (int i = 1; i <= n; ++i) pw[i] = (i64)pw[i - 1] * C % MOD;
  for (int i = 1, u, v; i <= m; ++i) {
    cin >> u >> v;
    --u; --v;
    e[u][v] = e[v][u] = 1;
  }
  L = n >> 1;
  for (int i = 0; i < n; ++i) f[1 << (i >> 1)][i] = 1;
  for (int S = 1; S < (1 << L); ++S) {
    for (int i = 0; i < n; ++i) {
      // if (!(S & (1 << (i >> 1)))) continue;
      if (!f[S][i]) continue;
      for (int j = 0; j < n; ++j) {
        if (is_in(S, j >> 1) || !e[i][j]) continue;
        Add(f[S | (1 << (j >> 1))][opp(j)], f[S][i]);
      }
    }
  }
  for (int i = 0; i < n; i += 2) g[1 << (i >> 1)][i + 1] = 1;
  for (int S = 1; S < (1 << L); ++S) {
    int lim = __lg(S & (-S));
    for (int i = 0; i < n; ++i) {
      if (!g[S][i]) continue;
      for (int j = lim << 1; j < n; ++j) {
        if (is_in(S, j >> 1) || !e[i][j]) continue;
        if (!e[i][j]) continue;
        Add(g[S | (1 << (j >> 1))][opp(j)], g[S][i]);
      }
      // cout << S << " " << i << " " << g[S][i] << "\n";
    }
  }
  // memset(g, 0, sizeof(g));
  for (int S = 1; S < (1 << L); ++S) {
    int low = __lg(S & (-S)), pc = __builtin_popcount(S);
    low = low << 1;
    for (int i = 0; i < n; ++i) {
      if (pc > 0) f[S][i] = (i64)f[S][i] * _2inv % MOD;
      Add(sum[S], (i64)f[S][i] * pw[pc - 1] % MOD);
      if (e[low][i]) 
        Add(sum[S], (i64)g[S][i] * pw[pc] % MOD);
    }
  }
  ans[0] = 1;
  for (int S = 1; S < (1 << L); ++S) {
    /*
    for (int S0 = S; S0; S0 = (S0 - 1) & S) {
      Add(ans[S], (i64)ans[S ^ S0] * sum[S] % MOD);
    }
    */
    int lb = S & (-S);
    // ans[S] = sum[S];
    for (int S0 = S ^ lb; ~S0; S0 = (S0 - 1) & (S ^ lb)) {
      Add(ans[S], (i64)ans[S ^ (S0 | lb)] * sum[S0 | lb] % MOD);
      if (!S0) break;
    }
  }
  cout << add(ans[(1 << L) - 1], 0) << "\n";
  return 0;
}

