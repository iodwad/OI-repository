// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

using i64 = long long;

const int MAXN = 15000;
const int MAXQ = 100000;
const int INF = 0x3f3f3f3f;

int n, m, Q, a[MAXN + 5];
int L[MAXN + 5], R[MAXN + 5], X[MAXN + 5];
i64 ans[MAXQ + 5];

struct Query {
  int l, r, l1, r1, id;
  Query(int _l = 0, int _r = 0, int _l1 = 0, int _r1 = 0, int _id = 0) : l(_l), r(_r), l1(_l1), r1(_r1), id(_id) {}
  friend bool operator< (const Query &lhs, const Query &rhs) {
    return lhs.l > rhs.l;
  }
} que[MAXQ + 5];

struct SGT {
#define ls (o << 1)
#define rs (o << 1 | 1)
  int mx1[MAXN << 2 | 1], mx2[MAXN << 2 | 1], cnt[MAXN << 2 | 1], tag[MAXN << 2 | 1];
  i64 sum[MAXN << 2 | 1];
  SGT() { memset(tag, 0x3f, sizeof(tag)); }
  void push_up(int o) {
    sum[o] = sum[ls] + sum[rs];
    mx1[o] = max(mx1[ls], mx1[rs]);
    if (mx1[ls] == mx1[rs]) mx2[o] = max(mx2[ls], mx2[rs]);
    else mx2[o] = mx1[ls] < mx1[rs] ? max(mx1[ls], mx2[rs]) : max(mx2[ls], mx1[rs]);
    cnt[o] = 0;
    if (mx1[ls] == mx1[o]) cnt[o] += cnt[ls];
    if (mx1[rs] == mx1[o]) cnt[o] += cnt[rs];
  }
  bool push_tag(int o, int x, int y) {
    if (x >= mx1[o]) return false;
    if (x > mx2[o]) {
      tag[o] = x;
      sum[o] += (i64)(x - mx1[o]) * cnt[o];
      mx1[o] = x;
      return false;
    } else return true;
  }
  void push_down(int o) {
    if (tag[o] == INF) return;
    push_tag(ls, tag[o]);
    push_tag(rs, tag[o]);
    tag[o] = INF;
  }
  void build(int o = 1, int l = 1, int r = n) {
    tag[o] = INF;
    if (l == r) {
      mx1[o] = sum[o] = a[l]; mx2[o] = 0; cnt[o] = 1;
      return;
    }
    int mid = (l + r) >> 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    push_up(o);
  }
  void update(int ql, int qr, int v, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr && !push_tag(o, v)) return;
    int mid = (l + r) >> 1;
    push_down(o);
    if (ql <= mid) update(ql, qr, v, ls, l, mid);
    if (mid < qr) update(ql, qr, v, rs, mid + 1, r);
    push_up(o);
  }
  i64 query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return sum[o];
    int mid = (l + r) >> 1; i64 res = 0;
    push_down(o);
    if (ql <= mid) res = query(ql, qr, ls, l, mid);
    if (mid < qr) res += query(ql, qr, rs, mid + 1, r);
    return res;
  }
#undef ls
#undef rs
} t;

vector<int> gp[MAXN + 5];

void pre_work() {
  static int stk[MAXN + 5], top;
  for (int i = 1; i <= Q; ++i) {
    int p = upper_bound(stk + 1, stk + top + 1, -que[i].r) - stk;
    stk[p] = -que[i].r;
    if (p > top) ++top;
    gp[p].push_back(i);
  }
}

void work(int x) {
  t.build();
  int l = que[gp[x][0]].l, r = que[gp[x][0]].l - 1;
  for (int i : gp[x]) {
    while (l > que[i].l) --l, t.update(L[l], R[l], X[l]);
    while (r < que[i].r) ++r, t.update(L[r], R[r], X[r]);
    ans[que[i].id] = t.query(que[i].l1, que[i].r1);
  }
}

int main() {
  freopen("b.in", "r", stdin);
  freopen("b.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m >> Q;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1; i <= m; ++i) cin >> L[i] >> R[i] >> X[i];
  for (int i = 1; i <= Q; ++i) cin >> que[i].l >> que[i].r >> que[i].l1 >> que[i].r1, que[i].id = i;
  sort(que + 1, que + Q + 1);
  pre_work();
  for (int i = 1; i <= Q; ++i) {
    if (gp[i].empty()) break;
    work(i);
  }
  for (int i = 1; i <= Q; ++i) cout << ans[i] << "\n";
  return 0;
}
