// by iodwad
#include <cstdio>
#include <iostream>
#include <cassert>

#define fi first
#define se second

using namespace std;

using i64 = long long;

const int MAXN = 1e5;
const int MOD = 998244353;
const int _2inv = (MOD + 1) / 2;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }

int n, pos[MAXN + 5], ans[MAXN + 5];

struct Edge {
  int tot;
  int fst[MAXN + 5], to[MAXN + 5], c[MAXN + 5], x[MAXN + 5], nxt[MAXN + 5];
  Edge() {}
  void add(int u, int v, int _c, int _x) {
    ++tot;
    to[tot] = v; c[tot] = _c; x[tot] = _x;
    nxt[tot] = fst[u]; fst[u] = tot;
  }
} e;

int las[MAXN + 5];
int top, fail[MAXN + 5], len[MAXN + 5];
pair<int, int> stk[MAXN + 5];

int calc(int l, int r) { return (i64)(l + r) * (r - l + 1) % MOD * _2inv % MOD; }

int push(int c, int x) {
  fail[0] = 0;
  stk[++top] = {c, x};
  len[top] = len[top - 1] + x;
  if (top == 1) return calc(1, x - 1);
  int res = 0;
  int i = fail[top - 1], mx = 0;
  while (i) {
    if (stk[i + 1].fi == c && stk[i + 1].se > mx) {
      res = add(res, calc(len[i] + mx + 1, len[i] + min(stk[i + 1].se, x)));
      mx = min(stk[i + 1].se, x);
    }
    if (stk[i + 1] == stk[top]) break;
    if (fail[i] * 2 > i) {
      int j = fail[i];
      if (stk[j + 1].fi == c && stk[j + 1].se > mx) {
        res = add(res, calc(len[j] + mx + 1, len[j] + min(stk[j + 1].se, x)));
        mx = min(stk[j + 1].se, x);
      }
      if (stk[j + 1] == stk[top]) { i = j; break; }
      i = (i - 1) % (i - fail[i]) + 1;
    }
    else i = fail[i];
  }
  if (!i && stk[1].fi == c) {
    if (stk[1].se > mx) {
      res = add(res, calc(mx + 1, min(stk[1].se, x)));
      mx = min(stk[i + 1].se, x);
      res = add(res, (i64)mx * (x - mx) % MOD);
    } else res = add(res, (i64)stk[1].se * (x - mx) % MOD);
    fail[top] = stk[1].se <= x;
  } else {
    fail[top] = stk[i + 1] == stk[top] ? i + 1 : 0;
  }
  if (top - fail[top] == fail[top] - fail[fail[top]]) las[top] = las[fail[top]];
  else las[top] = fail[top];
  return res;
}

void dfs(int x, int ans) {
  ::ans[x] = ans;
  for (int k = e.fst[x]; k; k = e.nxt[k]) {
    int to = e.to[k], c = e.x[k], ch = e.c[k];
    dfs(to, add(ans, push(ch, c)));
    --top;
  }
}

int main() {
  ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n;
  for (int i = 1, now = 0; i <= n; ++i) {
    int opt; cin >> opt;
    if (opt == 1) {
      int x; char c; cin >> x >> c;
      e.add(now, i, c - 'a', x);
      now = i;
    } else {
      int x; cin >> x;
      now = pos[x];
    }
    pos[i] = now;
  }
  las[1] = 1;
  dfs(0, 0);
  for (int i = 1; i <= n; ++i) cout << ans[pos[i]] << "\n";
  return 0;
}
