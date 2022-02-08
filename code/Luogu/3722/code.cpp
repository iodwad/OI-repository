// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>

#define int long long

using namespace std;

const int MAXN = 2e5;

int n, m, p1, p2;
int a[MAXN + 5], ql[MAXN + 5], qr[MAXN + 5], ans[MAXN + 5];
int top, stk[MAXN + 5];
vector<int> qs[MAXN + 5];

struct SGT {
#define ls (o << 1) 
#define rs (o << 1 | 1)
  int sum[MAXN << 2 | 1], tag[MAXN << 2 | 1];
  void push_tag(int o, int l, int r, int v) {
    sum[o] += (r - l + 1) * v;
    tag[o] +=v;
  }
  void push_down(int o, int l, int r) {
    if (!tag[o]) return;
    int mid = (l + r) >> 1;
    push_tag(ls, l, mid, tag[o]);
    push_tag(rs, mid + 1, r, tag[o]);
    tag[o] = 0;
  }
  void push_up(int o) { sum[o] = sum[ls] + sum[rs]; }
  void update(int ql, int qr, int v, int o = 1, int l = 1, int r = n) {
    if (ql > qr) return;
    if (ql <= l && r <= qr) {
      push_tag(o, l, r, v);
      return;
    }
    int mid = (l + r) >> 1;
    push_down(o, l, r);
    if (ql <= mid) update(ql, qr, v, ls, l, mid);
    if (mid < qr) update(ql, qr, v, rs, mid + 1, r);
    push_up(o);
  }
  int query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return sum[o];
    int mid = (l + r) >> 1, res = 0;
    push_down(o, l, r);
    if (ql <= mid) res = query(ql, qr, ls, l, mid);
    if (mid < qr) res += query(ql, qr, rs, mid + 1, r);
    return res;
  }
#undef ls
#undef rs
} t;

signed main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m >> p1 >> p2;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1; i <= m; ++i) cin >> ql[i] >> qr[i];
  for (int i = 1; i <= m; ++i) qs[qr[i]].push_back(i);
  for (int i = 1; i <= n; ++i) {
    int las = i;
    while (top > 0 && a[i] > a[stk[top]]) {
      t.update(stk[top] + 1, las - 1, p2); 
      t.update(stk[top], stk[top], p1);
      las = stk[top];
      --top;
    }
    t.update(top ? stk[top] + 1 : 1, las - 1, p2);
    stk[++top] = i;
    for (int x : qs[i]) ans[x] += t.query(ql[x], n);
  }
  memset(t.sum, 0, sizeof(t.sum));
  memset(t.tag, 0, sizeof(t.tag));
  for (int i = 1; i <= n; ++i) qs[i].clear();
  for (int i = 1; i <= m; ++i) qs[ql[i]].push_back(i);
  top = 0;
  for (int i = n; i >= 1; --i) {
    int las = i;
    while (top > 0 && a[i] > a[stk[top]]) {
      t.update(las + 1, stk[top] - 1, p2); 
      t.update(stk[top], stk[top], p1);
      las = stk[top];
      --top;
    }
    t.update(las + 1, top ? stk[top] - 1 : n, p2);
    stk[++top] = i;
    for (int x : qs[i]) ans[x] += t.query(1, qr[x]);
  }
  for (int i = 1; i <= m; ++i) cout << ans[i] << "\n";
  return 0;
}
