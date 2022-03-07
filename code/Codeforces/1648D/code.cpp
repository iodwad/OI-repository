// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

using i64 = long long;

const int MAXN = 5e5 + 5;
const i64 INF = 0x3f3f3f3f3f3f3f3f;

int n, m;
int L[MAXN + 5], R[MAXN + 5], K[MAXN + 5];
i64 a[4][MAXN + 5], sum[4][MAXN + 5];
i64 F[MAXN + 5], G[MAXN + 5], dp[MAXN + 5];
vector<int> vec[MAXN + 5];

struct SGT1 {
#define ls (o << 1)
#define rs (o << 1 | 1)
  i64 maxv[MAXN << 2 | 1];
  void modify(int pos, i64 v, int o = 1, int l = 1, int r = n) {
    if (l == r) {
      maxv[o] = v;
      return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) modify(pos, v, ls, l, mid);
    else modify(pos, v, rs, mid + 1, r);
    maxv[o] = max(maxv[ls], maxv[rs]);
  }
  i64 query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return maxv[o];
    int mid = (l + r) >> 1; i64 res = -INF;
    if (ql <= mid) res = query(ql, qr, ls, l, mid);
    if (mid < qr) res = max(res, query(ql, qr, rs, mid + 1, r));
    return res;
  }
#undef ls
#undef rs
} t1;

struct SGT2 {
#define ls (o << 1)
#define rs (o << 1 | 1)
  struct Info {
    i64 f, g, h;
    Info(i64 _f = 0, i64 _g = 0, i64 _h = 0) : f(_f), g(_g), h(_h) {}
    friend Info operator+ (const Info &lhs, const Info &rhs) {
      Info res;
      res.f = max(lhs.f, rhs.f);
      res.g = max(lhs.g, rhs.g);
      res.h = max({lhs.h, rhs.h, lhs.f + rhs.g});
      return res;
    }
  } v[MAXN << 2 | 1];
  void push_up(int o) {
    v[o] = v[ls] + v[rs];
  }
  void modify(int pos, i64 v1, i64 v2, int o = 1, int l = 1, int r = n) {
    if (l == r) {
      v[o] = Info(v1, v2, v1 + v2);
      return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) modify(pos, v1, v2, ls, l, mid);
    else modify(pos, v1, v2, rs, mid + 1, r);
    push_up(o);
  }
  Info query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return v[o];
    int mid = (l + r) >> 1;
    if (ql <= mid && mid < qr) return query(ql, qr, ls, l, mid) + query(ql, qr, rs, mid + 1, r);
    if (ql <= mid) return query(ql, qr, ls, l, mid);
    return query(ql, qr, rs, mid + 1, r);
  }
#undef ls
#undef rs
} t2;

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m;
  for (int i = 1; i <= 3; ++i) 
    for (int j = 1; j <= n; ++j) 
      cin >> a[i][j], sum[i][j] = sum[i][j - 1] + a[i][j];
  for (int i = 1; i <= m; ++i) {
    cin >> L[i] >> R[i] >> K[i];
    vec[R[i]].push_back(i);
  }
  for (int i = 1; i <= n; ++i) {
    F[i] = sum[1][i] - sum[2][i - 1];
    G[i] = sum[3][n] - sum[3][i - 1] + sum[2][i];
  }
  i64 ans = -INF;
  memset(dp, 0xcf, sizeof(dp));
  for (int i = 1; i <= n; ++i) {
    t1.modify(i, F[i]);
    for (int x : vec[i]) {
      dp[i] = max(dp[i], t1.query(L[x], i) - K[x]);
      if (L[x] > 1) dp[i] = max(dp[i], dp[L[x] - 1] - K[x]);
    }
    t1.modify(i, max(dp[i], F[i]));
  }
  for (int i = 1; i <= n; ++i) t2.modify(i, dp[i], G[i]);
  for (int i = 1; i <= m; ++i) ans = max(ans, t2.query(max(1, L[i] - 1), R[i]).h - K[i]);
  for (int i = 1; i <= n; ++i) t2.modify(i, F[i], G[i]);
  for (int i = 1; i <= m; ++i) ans = max(ans, t2.query(L[i], R[i]).h - K[i]);
  cout << ans << "\n";
  return 0;
}
