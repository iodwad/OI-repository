// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>

#define int long long

using namespace std;

const int MAXN = 3e5;

int n, Q;

namespace SGT {
#define ls (o << 1)
#define rs (o << 1 | 1)
  int sumv[MAXN << 2 | 1], lazy[MAXN << 2 | 1];
  void push_tag(int o, int l, int r, int v) {
    sumv[o] += (r - l + 1) * v;
    lazy[o] += v;
  }
  void push_down(int o, int l, int r) {
    if (!lazy[o]) return;
    int mid = (l + r) >> 1;
    push_tag(ls, l, mid, lazy[o]);
    push_tag(rs, mid + 1, r, lazy[o]);
    lazy[o] = 0;
  }
  void push_up(int o) {
    sumv[o] = sumv[ls] + sumv[rs];
  }
  void update(int ql, int qr, int v, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return push_tag(o, l, r, v);
    int mid = (l + r) >> 1;
    push_down(o, l, r);
    if (ql <= mid) update(ql, qr, v, ls, l, mid);
    if (mid < qr) update(ql, qr, v, rs, mid + 1, r);
    push_up(o);
  } 
  int query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return sumv[o];
    int mid = (l + r) >> 1, res = 0;
    push_down(o, l, r);
    if (ql <= mid) res = query(ql, qr, ls, l, mid);
    if (mid < qr) res += query(ql, qr, rs, mid + 1, r);
    return res;
  }
#undef ls
#undef rs
}

using namespace SGT;

int tim, dfn[MAXN + 5], idfn[MAXN + 5], siz[MAXN + 5], dep[MAXN + 5], son[MAXN + 5], topf[MAXN + 5], par[MAXN + 5][19];
vector<int> e[MAXN + 5];

void pre_dfs1(int x, int fa) {
  siz[x] = 1; dep[x] = dep[fa] + 1; par[x][0] = fa;
  for (int i = 1; i < 19; ++i) par[x][i] = par[par[x][i - 1]][i - 1];
  for (int to : e[x]) {
    if (to == fa) continue;
    pre_dfs1(to, x);
    siz[x] += siz[to];
    if (siz[to] > siz[son[x]]) son[x] = to;
  }
}

void pre_dfs2(int x, int ftop) {
  idfn[dfn[x] = ++tim] = x;
  topf[x] = ftop;
  if (!son[x]) return;
  pre_dfs2(son[x], ftop);
  for (int to : e[x]) if (to != par[x][0] && to != son[x]) pre_dfs2(to, to);
}

void Update1(int x, int w) {
  update(dfn[x], dfn[x] + siz[x] - 1, w); 
}

void Update2(int u, int v, int w) {
  while (topf[u] != topf[v]) {
    if (dep[topf[u]] < dep[topf[v]]) swap(u, v);
    update(dfn[topf[u]], dfn[u], w);
    u = par[topf[u]][0];
  }
  if (dep[u] > dep[v]) swap(u, v);
  update(dfn[u], dfn[v], w);
}

int Query() {
  int l = 1, r = n, pos = 0, sum = query(1, n);
  while (l <= r) {
    int mid = (l + r) >> 1;
    if (query(1, mid) * 2 >= sum) {
      pos = mid;
      r = mid - 1;
    } else l = mid + 1;
  }
  pos = idfn[pos];
  // cout << pos << "\n";
  for (int i = 18; ~i; --i) {
    int u = par[pos][i];
    if (u && query(dfn[u], dfn[u] + siz[u] - 1) * 2 <= sum) pos = u;
  }
  return query(dfn[pos], dfn[pos] + siz[pos] - 1) * 2 <= sum ? par[pos][0] : pos;
}

signed main() {
  freopen("yyl.in", "r", stdin);
  freopen("yyl.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n;
  for (int i = 1, u, v; i < n; ++i) {
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  pre_dfs1(1, 0);
  pre_dfs2(1, 1);
  cin >> Q;
  while (Q--) {
    int opt; cin >> opt;
    if (opt == 1) {
      int x, w; cin >> x >> w;
      Update1(x, w);
    } else {
      int u, v, w;
      cin >> u >> v >> w;
      Update2(u, v, w);
    }
    // int res = Query();
    cout << Query() << "\n";
  }
  return 0;
}

