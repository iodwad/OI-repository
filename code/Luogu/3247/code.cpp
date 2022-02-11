// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>

using namespace std;

const int MAXN = 5e4;
const int MAXM = 1e5;
const int BLK = 320;

int n, m, Q;
int U[MAXM + 5], V[MAXM + 5], A[MAXM + 5], B[MAXM + 5], ans[MAXM + 5], qa[MAXM + 5];
int bel[MAXM + 5], bl[BLK + 5], br[BLK + 5];
vector<int> es[MAXM + 5];

struct Query {
  int u, v, a, b, id;
  Query() {}
  friend bool operator< (const Query &lhs, const Query &rhs) { 
    return bel[lhs.a] < bel[rhs.a] || (bel[lhs.a] == bel[rhs.a] && lhs.b < rhs.b); 
  }
} que[MAXM + 5];

vector<int> des;
int discretize(int *pl, int *pr) {
  des.clear();
  for (int *i = pl; i != pr; ++i) des.push_back(*i);
  sort(des.begin(), des.end());
  des.erase(unique(des.begin(), des.end()), des.end());
  for (int *i = pl; i != pr; ++i) *i = lower_bound(des.begin(), des.end(), *i) - des.begin() + 1;
  return des.size();
}

int top;
pair<int *, int> stk[MAXM * 8 + 5];
void setv(int &x, int y) { stk[++top] = {&x, x}; x = y; }
void roll_back(int lim) { while (top > lim) *(stk[top].first) = stk[top].second, --top; }

int G;

struct DSU {
  int fset[MAXN + 5], size[MAXN + 5], maxa[MAXN + 5], maxb[MAXN + 5];
  DSU() {
    iota(fset, fset + MAXN + 1, 0);
    fill(size, size + MAXN + 1, 1);
  }
  int find(int x) { return fset[x] == x ? x : find(fset[x]); }
  void merge(int x, int y, int a, int b) {
    x = find(x); y = find(y);
    if (size[x] < size[y]) swap(x, y);
    if (x != y) {
      setv(fset[y], x);
      setv(size[x], size[x] + size[y]);
    }
    setv(maxa[x], max({maxa[x], maxa[y], a}));
    setv(maxb[x], max({maxb[x], maxb[y], b}));
  }
} t;

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m;
  for (int i = 1; i <= m; ++i) cin >> U[i] >> V[i] >> A[i] >> B[i];
  for (int i = 1; i <= m; ++i) des.push_back(A[i]);
  cin >> Q;
  for (int i = 1; i <= Q; ++i) {
    cin >> que[i].u >> que[i].v >> que[i].a >> que[i].b;
    que[i].id = i;
  }
  discretize(A + 1, A + m + 1);
  for (int i = 1; i <= Q; ++i) {
    int p = lower_bound(des.begin(), des.end(), que[i].a) - des.begin();
    if (p == (int)des.size() || des[p] != que[i].a) ans[i] = -1; 
    else que[i].a = p + 1;
    qa[i] = que[i].a;
  }
  discretize(B + 1, B + m + 1);
  for (int i = 1; i <= Q; ++i) {
    int p = lower_bound(des.begin(), des.end(), que[i].b) - des.begin();
    if (p == (int)des.size() || des[p] != que[i].b) ans[i] = -1; 
    else que[i].b = p + 1;
  }
  for (int i = 1; i <= m; ++i) bel[i] = (i - 1) / BLK + 1;
  for (int i = 1; i <= bel[m]; ++i) {
    bl[i] = br[i - 1] + 1;
    br[i] = bl[i] + BLK - 1;
  }
  br[bel[m]] = m;
  static int a[MAXM + 5];
  for (int i = 1; i <= m; ++i) {
    a[i] = i;
    es[B[i]].push_back(i);
  }
  sort(a + 1, a + m + 1, [&](int x, int y) { return A[x] < A[y]; });
  sort(que + 1, que + Q + 1, [&](Query lhs, Query rhs) { return lhs.a < rhs.a; });
  for (int i = 1, p = 0; i <= Q; ++i) {
    while (p + 1 <= m && A[a[p + 1]] <= que[i].a) ++p;
    que[i].a = p;
  }
  sort(que + 1, que + Q + 1);
  auto inserta = [&](int x, int y) {
    int cur = a[x];
    if (B[cur] <= y) t.merge(U[cur], V[cur], A[cur], B[cur]);
  }; 
  auto insertb = [&](int x, int y) {
    for (int i : es[y]) 
      if (A[i] <= A[a[x]]) t.merge(U[i], V[i], A[i], B[i]);
  };
  for (int i = 1, l = 0, r = 0, lasb = 0, lasa = 0; i <= Q; ++i) {
    if (ans[que[i].id] == -1) continue;
    if (bel[que[i].a] != lasa) {
      lasa = bel[que[i].a];
      roll_back(lasb);
      r = 0;
      while (l < bl[bel[que[i].a]]) inserta(++l, 0);
      lasb = top;
    } 
    assert(r <= que[i].b && l <= que[i].a);
    while (r < que[i].b) insertb(l, ++r);
    int las = top;
    while (l < que[i].a) inserta(++l, r);
    int u = t.find(que[i].u), v = t.find(que[i].v);
    ans[que[i].id] = u == v && t.maxa[u] == qa[que[i].id] && t.maxb[v] == que[i].b;
    roll_back(las);
    l = bl[bel[que[i].a]];
  }
  for (int i = 1; i <= Q; ++i) cout << (ans[i] > 0 ? "Yes" : "No") << "\n";
  return 0;
}
