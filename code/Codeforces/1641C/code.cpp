// by iodwad
#include <cstdio>
#include <iostream>
#include <set>
#include <cstring>

using namespace std;

const int MAXN = 2e5;

int n, Q;
set<int> Set;

namespace SGT {
#define ls (o << 1)
#define rs (o << 1 | 1)
  int mnv[MAXN << 2 | 1];
  void init() {
    memset(mnv, 0x3f, sizeof(mnv));
  }
  void update(int pos, int v, int o = 1, int l = 1, int r = n) {
    if (l == r) {
      mnv[o] = min(mnv[o], v);
      return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) update(pos, v, ls, l, mid);
    else update(pos, v, rs, mid + 1, r);
    mnv[o] = min(mnv[ls], mnv[rs]);
  }
  int query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return mnv[o];
    int mid = (l + r) >> 1, res = 1e9;
    if (ql <= mid) res = query(ql, qr, ls, l, mid);
    if (mid < qr) res = min(res, query(ql, qr, rs, mid + 1, r));
    return res;
  }
#undef ls
#undef rs
}

using namespace SGT;

void Update(int l, int r, int x) {
  if (x == 0) {
    while (1) {
      int pos = *Set.lower_bound(l);
      if (pos > r) break;
      Set.erase(pos);
    }
  } else {
    update(l, r);
  }
}

void Query(int pos) {
  if (*Set.lower_bound(pos) != pos) {
    cout << "NO" << "\n";
    return;
  }
  int l = *--Set.lower_bound(pos) + 1, r = *Set.upper_bound(pos) - 1;
  int res = query(l, pos);
  cout << (res <= r ? "YES" : "N/A") << "\n";
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  init();
  cin >> n >> Q;
  Set.insert(0); Set.insert(n + 1);
  for (int i = 1; i <= n; ++i) Set.insert(i);
  while (Q--) {
    int opt; cin >> opt;
    if (opt == 0) {
      int l, r, x; cin >> l >> r >> x;
      Update(l, r, x);
    } else {
      int pos; cin >> pos;
      Query(pos);
    }
  }
  return 0;
}
