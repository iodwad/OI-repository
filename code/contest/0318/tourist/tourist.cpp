// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

using i64 = long long;

const int MAXN = 5e5;
const int MOD = 998244353;
const int MAXL = 1 << 22;
const int G = 3;
const int GI = (MOD + 1) / 3;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}
int Inv(int x) { return fpow(x, MOD - 2); }
int sqr(int x) { return x * x; }

namespace Poly {
  void ntt(int *f, int len, int op) {
    static int rev[MAXL];
    for (int i = 0; i < len; ++i) {
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (len >> 1));
      if (i < rev[i]) swap(f[i], f[rev[i]]);
    }
    for (int i = 2; i <= len; i <<= 1) {
      int g = fpow(op == 1 ? G : GI, (MOD - 1) / i);
      for (int j = 0; j < len; j += i) {
        int gp = 1;
        for (int k = j; k < j + i / 2; ++k, gp = (i64)gp * g % MOD) {
          int tmp = (i64)f[k + i / 2] * gp % MOD;
          f[k + i / 2] = sub(f[k], tmp);
          f[k] = add(f[k], tmp);
        }
      }
    }
    if (op == -1) {
      int ninv = Inv(len);
      for (int i = 0; i < len; ++i) f[i] = (i64)f[i] * ninv % MOD;
    }
  }
  void poly_mul(int *f, int *g, int n, int m) {
    static int a[MAXL], b[MAXL];
    memcpy(a, f, sizeof(int) * n);
    memcpy(b, g, sizeof(int) * m);
    m += n - 1; n = 1;
    while (n < m) n <<= 1;
    ntt(a, n, 1); ntt(b, n, 1);
    for (int i = 0; i < n; ++i) a[i] = (i64)a[i] * b[i] % MOD;
    ntt(a, n, -1);
    memcpy(f, a, sizeof(int) * m);
    memset(a, 0, sizeof(int) * n);
    memset(b, 0, sizeof(int) * n);
  }
}

using namespace Poly;

int n, a[MAXN + 5], b[MAXN + 5], ans[MAXN + 5];
int par[MAXN + 5], lson[MAXN + 5], len[MAXN + 5], topf[MAXN + 5], dep[MAXN + 5];
vector<int> e[MAXN + 5];
vector<int> idx[MAXN + 5];

void pre_dfs1(int x) {
  dep[x] = dep[par[x]] + 1;
  for (int to : e[x]) {
    pre_dfs1(to);
    if (len[to] > len[lson[x]]) lson[x] = to;
  }
  len[x] = len[lson[x]] + 1;
}

void pre_dfs2(int x, int top) {
  topf[x] = top;
  idx[top].push_back(x);
  if (!lson[x]) return;
  pre_dfs2(lson[x], top);
  for (int to : e[x]) if (lson[x] != to) pre_dfs2(to, to);
}

void dfs(int x) {
  // cerr << x << "\n";
  static int f[MAXL], g[MAXL];
  int siz = len[x], tt = topf[par[x]];
  for (int i = 0; i < siz; ++i) 
    f[i] = sub(a[idx[x][i]], tt ? a[idx[tt][dep[idx[x][i]] - dep[tt]]] : 0);
  memcpy(g, b, sizeof(int) * siz);
  poly_mul(f, g, siz, siz);
  for (int i = 0; i < siz; ++i) {
    int now = idx[x][i];
    int res = tt ? ans[idx[tt][dep[now] - dep[tt]]] : 0;
    res = add(res, f[i]);
    ans[now] = res;
  }
  for (int i : idx[x]) 
    for (int to : e[i])
      if (to != lson[i]) dfs(to);
}

void dfs2(int x, int sum = 0) {
  // sum[x] = add(sum[x], add(sqr(a[x], b[dep[x]])));
  sum = add(sum, add(sqr(a[x]), sqr(b[dep[x] - 1])));
  ans[x] = sub(sum, 2ll * ans[x] % MOD);
  for (int to : e[x]) 
    dfs2(to, sum);
}

int main() {
  // ios::sync_with_stdio(false); cin.tie(0);
  freopen("tourist.in", "r", stdin);
  freopen("tourist.out", "w", stdout);
  cin >> n;
  for (int i = 2; i <= n; ++i) {
    cin >> par[i];
    e[par[i]].push_back(i);
  }
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 0; i < n; ++i) cin >> b[i];
  pre_dfs1(1);
  pre_dfs2(1, 1);
  dfs(1);
  dfs2(1);
  for (int i = 1; i <= n; ++i) cout << ans[i] << " ";
  return 0;
}

