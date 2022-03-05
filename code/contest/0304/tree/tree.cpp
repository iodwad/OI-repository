// by iodwad
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#define fi first
#define se second

using namespace std;

int read() {
  int x = 0;
  char ch = getchar();
  while (!isdigit(ch)) ch = getchar();
  while (isdigit(ch)) {
    x = x * 10 + ch - '0';
    ch = getchar();
  }
  return x;
}

const int MAXN = 5e5;

int n, K;
vector<int> e[MAXN + 5];

bool chkmin(int &x, int y) { return x > y ? x = y, true : false; }
bool chkmax(int &x, int y) { return x < y ? x = y, true : false; }

namespace NLCA {
  int tim, di;
  int dfn[MAXN + 5], idfn[MAXN + 5], siz[MAXN + 5], dep[MAXN + 5], maxs[MAXN + 5];
  int par[MAXN + 5][20], L[MAXN + 5], R[MAXN + 5];
  int st[20][MAXN * 2 + 5], lg[MAXN * 2 + 5];
  void pre_dfs(int x, int fa) {
    par[x][0] = fa;
    idfn[dfn[x] = ++di] = x; dep[x] = dep[fa] + 1; siz[x] = 1;
    L[x] = ++tim; st[0][tim] = x;
    for (int i = 1; i < 20; ++i) par[x][i] = par[par[x][i - 1]][i - 1];
    for (int to : e[x]) {
      if (to == fa) continue;
      pre_dfs(to, x);
      st[0][R[to] = ++tim] = x;
      siz[x] += siz[to];
      chkmax(maxs[x], siz[to]);
    }
  }
  int Min(int x, int y) { return dep[x] < dep[y] ? x : y; }
  void build_st() {
    lg[0] = -1;
    for (int i = 1; i <= tim; ++i) lg[i] = lg[i >> 1] + 1;
    for (int i = 1; i < 20; ++i)
      for (int j = 1; j + (1 << (i - 1)) - 1 <= tim; ++j)
        st[i][j] = Min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
  }
  int qmin(int l, int r) {
    int len = lg[r - l + 1];
    return Min(st[len][l], st[len][r - (1 << len) + 1]);
  }
  int LCA(int x, int y) {
    if (dfn[x] > dfn[y]) swap(x, y);
    return qmin(L[x], L[y]);
  }
  int dist(int x, int y) { return !x || !y ? 0 : dep[x] + dep[y] - 2 * dep[LCA(x, y)] + 1; }
  struct Info {
    int x, y;
    Info(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    friend Info operator+ (const Info &lhs, const Info &rhs) {
      if (!rhs.x) return lhs;
      if (!lhs.x) return rhs;
      Info res; res = lhs;
      int now = dist(lhs.x, lhs.y);
      if (chkmax(now, dist(rhs.x, rhs.y))) res = rhs;
      if (chkmax(now, dist(lhs.x, rhs.x))) res = {lhs.x, rhs.x};
      if (chkmax(now, dist(lhs.x, rhs.y))) res = {lhs.x, rhs.y};
      if (chkmax(now, dist(lhs.y, rhs.x))) res = {lhs.y, rhs.x};
      if (chkmax(now, dist(lhs.y, rhs.y))) res = {lhs.y, rhs.y};
      return res;
    }
  };
}

using namespace NLCA;

namespace SGT {
#define ls (o << 1)
#define rs (o << 1 | 1)
  Info info[MAXN << 2 | 1], val[MAXN << 2 | 1];
  // int tag[MAXN << 2 | 1], maxx[MAXN << 2 | 1];
  int tag[MAXN << 2 | 1];
  void push_up(int o, int l, int r) {
    if (tag[o]) val[o] = {0, 0};
    else if (l == r) val[o] = {idfn[l], idfn[l]};
    else val[o] = val[ls] + val[rs];
  }
  void build(int o = 1, int l = 1, int r = n) {
    if (l == r) return push_up(o, l, r);
    int mid = (l + r) >> 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    push_up(o, l, r);
  }
  void update(int ql, int qr, int v, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) {
      tag[o] += v;
      push_up(o, l, r);
      return;
    }
    int mid = (l + r) >> 1;
    if (ql <= mid) update(ql, qr, v, ls, l, mid);
    if (mid < qr) update(ql, qr, v, rs, mid + 1, r);
    push_up(o, l, r);
  }
  Info query(int ql, int qr, int o = 1, int l = 1, int r = n) {
    if (tag[o]) return {0, 0};
    if (ql <= l && r <= qr) return val[o];
    int mid = (l + r) >> 1; Info res(0, 0);
    if (ql <= mid) res = res + query(ql, qr, ls, l, mid);
    if (mid < qr) res = res + query(ql, qr, rs, mid + 1, r);
    return res;
  }
  int Query(int x) {
    Info tmp = query(dfn[x], dfn[x] + siz[x] - 1);
    return dist(tmp.x, tmp.y);
  }
#undef ls
#undef rs
}

using namespace SGT;

int idx[MAXN + 5];

void pre_work() {
  pre_dfs(1, 0);
  build_st();
  build();
  for (int i = 1; i <= n; ++i) idx[i] = i;
  sort(idx + 1, idx + n + 1, [&](int x, int y) { return maxs[x] < maxs[y]; });
}

int jump(int x, int lim) {
  for (int i = 19; ~i; --i) {
    int jp = par[x][i];
    if (!jp || Query(jp) >= lim) continue;
    x = jp;
  }
  x = par[x][0];
  return x && Query(x) >= lim ? x : 0;
}

int Solve(int lim) {
  static set<pair<int, int>> Set0, Set1;
  static int p = 1;
  while (p <= n && maxs[idx[p]] < lim) {
    Set0.emplace(siz[idx[p]], idx[p]);
    ++p;
  }
  Set1.clear();
  for (auto it = Set0.rbegin(); it != Set0.rend(); ++it) {
    if (it->fi < lim) break;
    Set1.emplace(dep[it->se], it->se);
  }
  static vector<int> dot; dot.clear();
  while (!Set1.empty()) {
    int now = (--Set1.end())->se; Set1.erase(--Set1.end());
    if (Query(now) >= lim) {
      update(dfn[now], dfn[now] + siz[now] - 1, 1);
      dot.push_back(now);
    }
    int nxt = jump(now, lim);
    if (nxt) Set1.emplace(dep[nxt], nxt);
  }
  for (int i : dot) update(dfn[i], dfn[i] + siz[i] - 1, -1);
  return dot.size();
}

int main() {
  freopen("tree.in", "r", stdin);
  freopen("tree.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  n = read(); K = read();
  for (int i = 1, u, v; i < n; ++i) {
    u = read(); v = read();
    e[u].push_back(v); e[v].push_back(u);
  }
  pre_work();
  static vector<int> stk;
  for (int t = K, lasans = 0; t <= n; t += K) {
    int opt = read();
    if (opt == 1) {
      int x = (read() ^ lasans) % n + 1; stk.push_back(x);
      update(dfn[x], dfn[x] + siz[x] - 1, 1);
    } else {
      int x = stk.back(); stk.pop_back();
      update(dfn[x], dfn[x] + siz[x] - 1, -1);
    }
    lasans = Solve(t);
    cout << lasans << "\n";
  }
  return 0;
}
