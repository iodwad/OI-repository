// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

using i64 = long long;
using ui32 = unsigned int;

const int MAXN = 1666;
const int MOD = 64123;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}
int Inv(int x) { return fpow(x, MOD - 2); }

int n, K, W, X;
int a[MAXN + 5];
vector<int> e[MAXN + 5];

namespace SGT {
  struct Info {
    ui32 a, b, c, d;
    Info(ui32 _a = 1, ui32 _b = 0, ui32 _c = 0, ui32 _d = 0) { a = _a; b = _b; c = _c; d = _d; }
    friend Info operator* (const Info &rhs, const Info &lhs) {
      // rhs on lhs
      return Info(lhs.a * rhs.a % MOD, (lhs.a * rhs.b + lhs.b) % MOD, (lhs.c * rhs.a + rhs.c) % MOD, (lhs.c * rhs.b + lhs.d + rhs.d) % MOD);
    } 
  } E;
  int ptot, root[MAXN + 5];
  int ls[MAXN * 60 + 5], rs[MAXN * 60 + 5];
  int top, trash[MAXN * 60 + 5];
  Info val[MAXN * 60 + 5];
  int newnode() {
    int o = top ? trash[top--] : ++ptot;
    ls[o] = rs[o] = 0;
    val[o] = E;
    return o;
  }
  void recover(int x) {
    trash[++top] = x;
  }
  void remove(int x) {
    if (!x) return;
    remove(ls[x]);
    remove(rs[x]);
    recover(x);
  }
  void push_tag(int o, const Info &x) {
    val[o] = val[o] * x;
  }
  void push_down(int o) {
    if (!ls[o]) ls[o] = newnode();
    if (!rs[o]) rs[o] = newnode();
    push_tag(ls[o], val[o]);
    push_tag(rs[o], val[o]);
    val[o] = E;
  }
  void update(int ql, int qr, Info v, int &o, int l = 1, int r = W) {
    if (!o) o = newnode();
    if (ql <= l && r <= qr) return push_tag(o, v);
    int mid = (l + r) >> 1;
    push_down(o);
    if (ql <= mid) update(ql, qr, v, ls[o], l, mid);
    if (mid < qr) update(ql, qr, v, rs[o], mid + 1, r);
  }
  int query(int pos, int o, int l = 1, int r = W) {
    if (!o) return 0;
    if (l == r) return val[o].d;
    int mid = (l + r) >> 1;
    push_down(o);
    if (pos <= mid) return query(pos, ls[o], l, mid);
    else return query(pos, rs[o], mid + 1, r);
  }
  int merge(int x, int y) {
    if (!ls[x] && !rs[x]) swap(x, y);
    if (!ls[y] && !rs[y]) {
      val[x] = val[x] * Info(val[y].b, 0, 0, val[y].d);
      recover(y);
      return x;
    }
    push_down(x); push_down(y);
    ls[x] = merge(ls[x], ls[y]);
    rs[x] = merge(rs[x], rs[y]);
    recover(y);
    return x;
  }
}

using namespace SGT;

void dfs(int x, int fa) {
  update(1, W, Info(0, 1, 0, 0), root[x]);
  for (int to : e[x]) {
    if (to == fa) continue;
    dfs(to, x);
    root[x] = merge(root[x], root[to]);
  }
  update(1, a[x], Info(X, 0, 0, 0), root[x]);
  update(1, W, Info(1, 0, 1, 0), root[x]);
  update(1, W, Info(1, 1, 0, 0), root[x]);
}

vector<int> operator+ (const vector<int> &lhs, const vector<int> &rhs) {
  int m = max(lhs.size(), rhs.size());
  static vector<int> res; res.clear(); 
  res.resize(m);
  for (size_t i = 0; (int)i < m; ++i)
    res[i] = add(i >= lhs.size() ? 0 : lhs[i], i >= rhs.size() ? 0 : rhs[i]);
  return res;
}

vector<int> operator* (const vector<int> &lhs, const vector<int> &rhs) {
  static vector<int> res; res.clear();
  res.resize(lhs.size() + rhs.size() - 1);
  for (size_t i = 0; i < lhs.size(); ++i)
    for (size_t j = 0; j < rhs.size(); ++j)
      res[i + j] = (res[i + j] + (i64)lhs[i] * rhs[j]) % MOD;
  return res;
}

vector<int> operator/ (const vector<int> &lhs, int rhs) {
  static vector<int> res; res.clear();
  res.resize(lhs.size() - 1);
  // res[0] = 
  int inv = Inv(rhs);
  res[0] = (i64)lhs[0] * inv % MOD;
  for (size_t i = 1; i < res.size(); ++i) 
    res[i] = (i64)sub(lhs[i], res[i - 1]) * inv % MOD;
  return res;
}

vector<int> Lagrange(vector<int> v) { // x = 1,2,...,n,n+1
  int m = v.size() - 1;
  static vector<int> res; res.clear(); res.resize(v.size() + 1);
  static vector<int> t({1});
  for (int i = 1; i <= m + 1; ++i) 
    t = t * vector<int>({sub(0, i), 1});
  for (int i = 1; i <= m + 1; ++i) {
    static vector<int> tmp;
    tmp = t / sub(0, i);
    int a = v[i - 1];
    for (int j = 1; j <= m + 1; ++j) {
      if (i == j) continue;
      a = (i64)a * Inv(sub(i, j)) % MOD;
    }
    for (int j = 0; j <= m; ++j) 
      res[j] = add(res[j], (i64)tmp[j] * a % MOD);
  }
  return res;
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> K >> W;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1, u, v; i < n; ++i) {
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  static vector<int> dot, poly;
  int ans = 0;
  for (X = 1; X <= n + 1; ++X) {
    remove(root[1]);
    for (int i = 1; i <= n; ++i) root[i] = newnode();
    dfs(1, 0);
    int now = 0;
    for (int i = 1; i <= W; ++i) now = add(now, query(i, root[1]));
    dot.push_back(now);
  }
  poly = Lagrange(dot);
  for (int i = K; i <= n; ++i) ans = add(ans, poly[i]);
  cout << ans << "\n";
  return 0;
}

