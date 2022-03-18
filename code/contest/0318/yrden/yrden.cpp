// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

using i64 = long long;
using ui64 = unsigned long long;

const int MAXN = 4e5;
const int MOD = 998244353;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}

int n, m, R;
int A[MAXN + 5], D[MAXN + 5];
vector<int> e[MAXN + 5];

namespace Task1 {
  const int maxn = 500;
  const int W = 64;
  const int LIM = maxn / W + 5;
  const ui64 L = -1;

  struct Bitset {
    ui64 num[LIM + 1];
    void clear() { memset(num, 0, sizeof(num)); }
    Bitset() { memset(num, 0, sizeof(num)); }
    ui64& operator[] (const int x) { return num[x]; }
    void set0(int x) { num[x / W] &= (L - (1ull << (x % W))); }
    void set1(int x) { num[x / W] |= (1ull << (x % W)); }
    friend Bitset operator& (const Bitset &lhs, const Bitset &rhs) {
      static Bitset bs; 
      for (int i = 0; i < LIM; ++i) bs[i] = lhs.num[i] & rhs.num[i];
      return bs;
    }
    friend Bitset operator| (const Bitset &lhs, const Bitset &rhs) {
      static Bitset bs; 
      for (int i = 0; i < LIM; ++i) bs[i] = lhs.num[i] | rhs.num[i];
      return bs;
    }
    friend Bitset operator^ (const Bitset &lhs, const Bitset &rhs) {
      static Bitset bs; 
      for (int i = 0; i < LIM; ++i) bs[i] = lhs.num[i] ^ rhs.num[i];
      return bs;
    }
    friend bool operator< (const Bitset &lhs, const Bitset &rhs) {
      for (int i = 0; i < LIM; ++i) 
        if (lhs.num[i] != rhs.num[i])
          return lhs.num[i] < rhs.num[i];
      return 0;
    }
    int count() {
      int res = 0;
      for (int i = 0; i < LIM; ++i) res += __builtin_popcountll(num[i]);
      return res;
    }
  } Bs[maxn * maxn * 5 + 5];

  int tot;
  int dp[2][maxn * maxn + 5], ans[maxn + 5];
  map<Bitset, int> ID;

  void find(int x, int fa, int d, Bitset &bs) {
    if (d == -1) return;
    bs.set1(x);
    for (int to : e[x]) if (to != fa) find(to, x, d - 1, bs);
  }

  int id(const Bitset &x) { 
    if (ID.count(x)) return ID[x];
    Bs[++tot] = x;
    return ID[x] = tot;
  }

  int main() {
    static Bitset bs;
    for (int i = 1; i <= n; ++i) bs.set1(i);
    dp[1][id(bs)] = 1;
    for (int i = 1; i <= m; ++i) {
      swap(dp[0], dp[1]);
      memset(dp[1], 0, sizeof(dp[1]));
      int a = A[i], d = D[i];
      bs.clear(); 
      find(a, 0, d, bs);
      for (int j = 1; j <= tot; ++j) {
        if (!dp[0][j]) continue;
        int tmp = id(Bs[j] & bs);
        dp[1][tmp] = add(dp[1][tmp], dp[0][j]);
        dp[1][j] = add(dp[1][j], dp[0][j]);
      }
    }
    for (int i = 1; i <= tot; ++i) {
      int now = Bs[i].count();
      ans[now] = add(ans[now], dp[1][i]);
    }
    for (int i = 0; i <= R; ++i) cout << ans[i] << " ";
    return 0;
  }
}

namespace Task2 {
  struct BIT {
    int sumv[MAXN + 5];
    BIT() {}
    void update(int x, int v) { 
      ++x;
      for (int i = x; i; i -= i & (-i))
        sumv[i] += v;
    }
    int query(int x) {
      ++x;
      int res = 0;
      for (int i = x; i <= n + 1; i += i & (-i))
        res += sumv[i];
      return res;
    }
  } t0;

  int all_size, rt, ans;
  int maxs[MAXN + 5], siz[MAXN + 5];
  int f[MAXN + 5];
  bool vis[MAXN + 5];
  vector<int> O[MAXN + 5];

  void get_root(int x, int fa) {
    siz[x] = 1; maxs[x] = 0;
    for (int to : e[x]) {
      if (to == fa || vis[to]) continue;
      get_root(to, x);
      siz[x] += siz[to];
      maxs[x] = max(maxs[x], siz[to]);
    }
    maxs[x] = max(maxs[x], all_size - siz[x]);
    if (maxs[x] < maxs[rt]) rt = x;
  }

  void add_tree(int x, int dep, int fa, int v) {
    if (vis[x]) return;
    for (int q : O[x]) if (dep <= D[q]) t0.update(D[q] - dep, v);
    for (int to : e[x]) {
      if (to == fa) continue;
      add_tree(to, dep + 1, x, v);
    }
  }

  void q_tree(int x, int dep, int fa) {
    if (vis[x]) return;
    f[x] += t0.query(dep);
    for (int to : e[x]) {
      if (to == fa) continue;
      q_tree(to, dep + 1, x);
    }
  }

  void DAC(int x) {
    vis[x] = 1;
    for (int q : O[x]) t0.update(D[q], 1);
    for (int to : e[x]) {
      q_tree(to, 1, x);
      add_tree(to, 1, x, 1);
    }
    f[x] += t0.query(0);
    for (int q : O[x]) t0.update(D[q], -1);
    for (int to : e[x]) {
      add_tree(to, 1, x, -1);
      q_tree(to, 1, x);
    }
    int las_size = all_size;
    for (int to : e[x]) {
      if (vis[to]) continue;
      all_size = siz[to] < siz[x] ? siz[to] : las_size - siz[x];
      rt = 0; get_root(to, x);
      DAC(rt);
    }
  }

  int buc[MAXN + 5]; 
  void dfs1(int x, int fa) {
    static int stk[MAXN + 5], top;
    stk[++top] = x;
    for (int to : e[x]) {
      if (to == fa) continue;
      dfs1(to, x);
    }
    for (int q : O[x]) if (top > D[q]) ++buc[stk[top - D[q]]];
    --top;
  }

  int main() {
    for (int i = 1; i <= m; ++i) O[A[i]].push_back(i);
    all_size = n; maxs[0] = 0x3f3f3f3f; get_root(1, 0);
    DAC(rt);
    dfs1(1, 0);
    for (int i = 1; i <= n; ++i) {
      ans = add(ans, fpow(2, f[i]));
      if (i != 1) ans = sub(ans, fpow(2, f[i] - buc[i]));
    }
    ans = sub(fpow(2, m), ans);
    cout << ans << "\n";
    return 0;
  }
}

int main() {
  ios::sync_with_stdio(false); cin.tie(0);
  freopen("yrden.in", "r", stdin);
  freopen("yrden.out", "w", stdout);
  cin >> n >> m >> R;
  for (int i = 1, u, v; i < n; ++i) {
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  for (int i = 1; i <= m; ++i) cin >> A[i];
  for (int i = 1; i <= m; ++i) cin >> D[i];
  if (n <= 500) return Task1::main();
  return Task2::main();
  return 0;
}
