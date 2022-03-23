// by iodwad
#include <cstdio>
#include <iostream>
#include <set>
#include <map>

#define int long long

using namespace std;

const int MAXN = 2e5;
const int INF = 0x3f3f3f3f;

int n, Q;

struct SGT {
#define ls (o << 1)
#define rs (o << 1 | 1)
  int sum[MAXN << 2 | 1], tag[MAXN << 2 | 1];
  void push_tag(int o, int l, int r, int v) {
    sum[o] += (r - l + 1) * v;
    tag[o] += v;
  }
  void push_down(int o, int l, int r) {
    if (!tag[o]) return;
    int mid = (l + r) >> 1;
    push_tag(ls, l, mid, tag[o]);
    push_tag(rs, mid + 1, r, tag[o]);
    tag[o] = 0;
  }
  void update(int ql, int qr, int v, int o = 1, int l = 1, int r = n) {
    if (ql <= l && r <= qr) return push_tag(o, l, r, v);
    int mid = (l + r) >> 1;
    push_down(o, l, r);
    if (ql <= mid) update(ql, qr, v, ls, l, mid);
    if (mid < qr) update(ql, qr, v, rs, mid + 1, r);
    sum[o] = sum[ls] + sum[rs];
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

void Update(int x, int v) {
  if (!x) return;
  t.update(1, 1, x * v);
  if (x > 1) t.update(2, x, -1 * v);
  if (x < n) t.update(x + 1, x + 1, -1 * v);
}

struct Node {
  int l, r, v;
  Node(int _l = 0, int _r = 0, int _v = 0) : l(_l), r(_r), v(_v) {}
  friend bool operator< (const Node &lhs, const Node &rhs) { return lhs.l < rhs.l; }
};

int tot;
map<int, int> ID;
// set<int> Set[MAXN + 5];

struct Odt {
  set<Node> odt;
  auto split(int x) {
    // if (x > n) return odt.end();
    auto it = --odt.upper_bound({x, 0, 0});
    if (it->l == x) return it;
    if (it->r < x) return ++it;
    int l = it->l, r = it->r, v = it->v;
    odt.erase(it);
    odt.emplace(l, x - 1, v);
    return odt.emplace(x, r, v).first;
  }
} o1, od[MAXN * 2 + 5];

void Insert(int l, int r, int v) {
  if (!ID.count(v)) {
    ID[v] = ++tot; v = tot;
    od[v].odt.emplace(0, 0, v);
    od[v].odt.emplace(n + 1, n + 1, v);
    Update(n, 1);
  } else v = ID[v];
  auto it = od[v].odt.emplace(l, r, v).first;
  Update(next(it)->l - prev(it)->r - 1, -1);
  Update(l - prev(it)->r - 1, 1);
  Update(next(it)->l - r - 1, 1);
  // od[v].odt.emplace(l, r, v);
}

void Erase(int l, int r, int v) {
  v = ID[v];
  auto itr = od[v].split(r + 1), itl = od[v].split(l);
  for (auto it = itl; it != itr; ++it) {
    Update(it->l - prev(it)->r - 1, -1);
  }
  Update(itr->l - prev(itr)->r - 1, -1);
  Update(itr->l - prev(itl)->r - 1, 1);
  od[v].odt.erase(itl, itr);
}

void assign(int l, int r, int v) {
  auto itr = r == n ? o1.odt.end() : o1.split(r + 1), itl = o1.split(l);
  for (auto it = itl; it != itr; ++it) {
    if (v == it->v) continue;
    Erase(it->l, it->r, it->v);
    Insert(it->l, it->r, v);
  }
  o1.odt.erase(itl, itr);
  o1.odt.emplace(l, r, v);
}

signed main() {
  ios::sync_with_stdio(false); cin.tie(0);
  // freopen("code.in", "r", stdin);
  // freopen("code.out", "w", stdout);
  cin >> n >> Q;
  for (int i = 1, a; i <= n; ++i) {
    cin >> a;
    Insert(i, i, a);
    o1.odt.emplace(i, i, a);
  }
  while (Q--) {
    int opt; cin >> opt;
    if (opt == 1) {
      int l, r, x; cin >> l >> r >> x;
      assign(l, r, x);
    } else {
      int x; cin >> x;
      cout << (n - x + 1) * tot - t.query(1, x) << "\n";
    }
  }
  return 0;
}
