// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

using ui64 = unsigned long long;

const int MAXN = 2e5;
const int B = 400;

int n, Q, len, a[MAXN + 5];
int tim, dfn[MAXN + 5], idfn[MAXN + 5], siz[MAXN + 5];
int tot, id[MAXN + 5], mbs[MAXN + 5];
ui64 sd;
vector<int> e[MAXN + 5];

ui64 pw2[64];
struct Bitset {
  ui64 bit[3126];
  Bitset() { memset(bit, 0, sizeof(bit)); }
  void reset() { memset(bit, 0, sizeof(bit)); }
  void set0(int x) { bit[x >> 6] &= ~pw2[x & 63]; }
  void set1(int x) { bit[x >> 6] |= pw2[x & 63]; }
  void flip(int x) { bit[x >> 6] ^= pw2[x & 63]; }
  bool operator[] (int x) { return !!(bit[x >> 6] & pw2[x & 63]); }
  ui64 val(int x) {
    if (x > n) return 0;
    int p = x & 63, u = x >> 6;
    ui64 res = bit[u] >> p;
    if (u < 3125) res |= (bit[u + 1] & (pw2[p] - 1)) << (64 - p);
    return res;
  }
} bs[B];


int top;
pair<int, int> stk[MAXN + 5];

void dfs() {
  stk[++top] = {1, 0}; 
  while (top > 0) {
    int x = stk[top].first, &i = stk[top].second;
    if (!i) idfn[dfn[x] = ++tim] = x;
    if (i != int(e[x].size())) {
      stk[++top] = {e[x][i], 0};
      ++i;
      continue;
    }
    siz[x] = 1;
    for (int to : e[x]) {
      siz[x] += siz[to];
      if (siz[mbs[to]] > siz[mbs[x]]) mbs[x] = mbs[to];
    }
    if (siz[x] - siz[mbs[x]] > 2 * n / B) {
      mbs[x] = x; id[x] = ++tot;
      for (int i = dfn[x]; i < dfn[x] + siz[x]; ++i) 
        bs[tot].set1(a[idfn[i]]);
    }
    --top;
  }
}

Bitset get_subtree(int x) {
  static Bitset res; 
  int u = mbs[x];
  res = bs[id[u]];
  for (int i = dfn[x]; i < dfn[u]; ++i) res.set1(a[idfn[i]]);
  for (int i = max(dfn[u] + siz[u], dfn[x]); i < dfn[x] + siz[x]; ++i) res.set1(a[idfn[i]]);
  return res;
}

int main() {
  freopen("ds.in", "r", stdin);
  freopen("ds.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  pw2[0] = 1;
  for (int i = 1; i < 64; ++i) pw2[i] = pw2[i - 1] << 1;
  cin >> n >> Q >> sd >> len;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 2, fa; i <= n; ++i) {
    cin >> fa;
    e[fa].push_back(i);
  }
  dfs();
  while (Q--) {
    int p, k; cin >> p >> k;
    p = (sd + p) % n + 1;
    static Bitset now; now = get_subtree(p);
    for (int t = 1; t <= len; ++t) {
      ui64 v = now.val(sd) & now.val(sd + k);
      sd = (sd + v + 123) % n + 1;
    }
    cout << sd << "\n";
  }
  return 0;
}
