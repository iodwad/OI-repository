// by iodwad
#include <cstdio>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstring>

#define fi first
#define se second

using namespace std;

using i64 = long long;

const int MAXN = 100;
const int MOD = 19260817;
const int BASE = 10007;
const int MAXC = 1e8;
const int MAXS = 7.5e6;

int n, m, mc, D, tot;
int a[MAXN + 5], w[MAXN + 5], maxx[MAXS + 5], dp[MAXN + 5][MAXN + 5];
priority_queue<int> pq;
pair<int, int> sta[MAXS + 5];

bool chkmax(int &x, int y) { return x < y ? x = y, true : false; }

int get_D() {
  int res = 0;
  memset(dp, 0xcf, sizeof(dp));
  dp[0][mc] = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = a[i + 1]; j <= mc; ++j) {
      chkmax(dp[i + 1][j - a[i + 1]], dp[i][j] + 1);
      chkmax(dp[i + 1][min(mc, j - a[i + 1] + w[i + 1])], dp[i][j]);
    }
  }
  for (int i = 1; i <= n; ++i) for (int j = 0; j <= mc; ++j) chkmax(res, dp[i][j]);
  return res;
}

map<pair<int, int>, bool> MAP;

struct Hash {
  int tot;
  int nxt[MAXS + 5], fst[MOD + 5];
  i64 num[MAXS + 5];
  void insert(int x, int y) {
    i64 s = i64(x) * BASE + y, ss = s % MOD;
    ++tot;
    nxt[tot] = fst[ss]; num[tot] = s; fst[ss] = tot;
  }
  bool query(int x, int y) {
    i64 s = i64(x) * BASE + y, ss = s % MOD;
    for (int i = fst[ss]; i; i = nxt[i])
      if (num[i] == s) return true;
    return false;
  }
} Map;

struct Node {
  int f, l, s;
  Node(int _f = 0, int _l = 0, int _s = 0) : f(_f), l(_l), s(_s) {}
};

void bfs() {
  static queue<Node> q;
  static unordered_map<int, bool> vis;
  auto insert = [&](int f, int l, int s) {
    q.emplace(f, l, s); 
    Map.insert(f, l);
    if (!vis.count(f)) sta[++tot] = {f, s + 1}, vis[f] = 1;
  };
  sta[++tot] = {0, 0};
  insert(1, 0, 0);
  while (!q.empty()) {
    Node frm = q.front(); q.pop();
    int f = frm.f, l = frm.l, s = frm.s;
    if (s == 100) continue;
    if (l && i64(f) * l <= MAXC && !Map.query(f * l, l)) insert(f * l, l, s + 1);
    if (i64(f) * (l + 1) <= MAXC && !Map.query(f, l + 1)) insert(f, l + 1, s + 1);
  }
}

int Query(int x) {
  for (int p1 = 1, p2 = tot; p1 <= tot; ++p1) {
    while (p2 > 0 && sta[p1].fi + sta[p2].fi > x) --p2;
    if (!p2) return 0;
    if (D + sta[p1].fi - sta[p1].se + maxx[p2] >= x) return 1;
  }
  return 0;
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m >> mc;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1; i <= n; ++i) cin >> w[i];
  D = get_D();
  bfs();
  sort(sta + 1, sta + 1 + tot, [&](const pair<int, int> &lhs, const pair<int, int> &rhs) { return lhs.fi < rhs.fi; } );
  for (int i = 1; i <= tot; ++i) maxx[i] = max(maxx[i - 1], sta[i].fi - sta[i].se);
  while (m--) {
    int C; cin >> C;
    cout << Query(C) << "\n";
  }
  return 0;
}
