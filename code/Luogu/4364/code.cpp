// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

const int MAXN = 5e5;

int n, a[MAXN + 5], ans[MAXN + 5], siz[MAXN + 5], par[MAXN + 5];
vector<int> p, e[MAXN + 5];
double K;

namespace SGT {
#define ls (o << 1)
#define rs (o << 1 | 1)
  int mnv[MAXN << 2 | 1], mxv[MAXN << 2 | 1], tag[MAXN << 2 | 1];
  void push_tag(int o, int v) {
    mnv[o] += v;
    mxv[o] += v;
    tag[o] += v;
  }
  void push_down(int o) {
    if (!tag[o]) return;
    push_tag(ls, tag[o]);
    push_tag(rs, tag[o]);
    tag[o] = 0;
  }
  void push_up(int o) { 
    mnv[o] = min(mnv[ls], mnv[rs]); 
    mxv[o] = max(mxv[ls], mxv[rs]); 
  }
  void update(int ql, int qr, int v, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return push_tag(o, v);
    int mid = (l + r) >> 1;
    push_down(o);
    if (ql <= mid) update(ql, qr, v, ls, l, mid);
    if (mid < qr) update(ql, qr, v, rs, mid + 1, r);
    push_up(o);
  }
  void modify(int pos, int v) {
    update(1, pos, v);
  }
  int query(int lim, int o = 1, int l = 1, int r = n) {
    if (l == r) return mnv[o] >= lim ? l : -1;
    int mid = (l + r) >> 1;
    push_down(o);
    // if (mnv[rs] < lim) return query(lim, ls, l, mid);
    // return query(lim, rs, mid + 1, r);
    // if (mnv[rs] < lim) return query(lim, ls, l, mid);
    // return query(lim, rs, mid + 1, r);
    if (mnv[ls] >= lim) {
      int t = query(lim, rs, mid + 1, r);
      return ~t ? t : mid;
    }
    return query(lim, ls, l, mid);
  }
#undef ls
#undef rs
}

using namespace SGT;

void Discretize(int *l, int *r) {
  for (int *p = l; p != r; p = p + 1) ::p.push_back(*p);
  sort(::p.begin(), ::p.end());
  ::p.erase(unique(::p.begin(), ::p.end()), ::p.end());
  for (int *p = l; p != r; p = p + 1) *p = lower_bound(::p.begin(), ::p.end(), *p) - ::p.begin() + 1;
}

void dfs(int x) {
  siz[x] = 1;
  for (int to : e[x]) {
    par[to] = x;
    dfs(to);
    siz[x] += siz[to];
  }
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n >> K;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1; i <= n; ++i) e[(int)(i / K)].push_back(i);
  dfs(0);
  Discretize(a + 1, a + n + 1);
  sort(a + 1, a + n + 1);
  for (int i = 1; i <= n; ++i) modify(a[i], 1);
  for (int i = 1; i <= n; ++i) {
    if (par[i]) modify(ans[par[i]], siz[i]);
    int pos = query(siz[i]);
    modify(pos, -siz[i]);
    ans[i] = pos;
  }
  for (int i = 1; i <= n; ++i) cout << p[ans[i] - 1] << " \n"[i == n];
  return 0;
}
