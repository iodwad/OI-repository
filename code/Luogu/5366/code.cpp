// by iodwad
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

using i64 = long long;

const int MOD = 1e9 + 7;
const int MAXL = 16;
const int MAXS = 1 << 16;
const int _2inv = (MOD + 1) / 2;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
void Add(int &x, int y) { x = add(x, y); }
void Sub(int &x, int y) { x = sub(x, y); }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}

int n, G, L, Q, U, tot, maxl, maxs;
int f[MAXS], g[MAXS], ans[MAXS], pr[MAXL + 5];
map<int, int> cnt1, cnt2, cnt3, bak;

void FMT(int *f, int op) {
  if (op == 1) {
    for (int i = 0; i < maxl; ++i)
      for (int j = 0; j < maxs; ++j) 
        if ((j >> i) & 1) Add(f[j], f[j ^ (1 << i)]);
  } else {
    for (int i = 0; i < maxl; ++i)
      for (int j = (maxs - 1); ~j; --j)
        if ((j >> i) & 1) Sub(f[j], f[j ^ (1 << i)]);
  }
}

void factor(int x, map<int, int> &cnt) {
  for (int i = 2; i * i <= x; ++i) {
    while (x % i == 0) {
      ++cnt[i];
      x /= i;
    }
  }
  if (x > 1) ++cnt[x];
}

void pre_work() {
  static vector<int> fac;
  for (int i = 1; i * i <= L; ++i) {
    if (L % i) continue;
    fac.push_back(i);
    if (L / i != i) fac.push_back(L / i);
  }
  factor(G, cnt1); 
  factor(L, cnt2);
  for (auto it : cnt2) pr[tot++] = it.first;
  for (int i : fac) {
    if (i % G || i > n) continue;
    cnt3.clear();
    factor(i, cnt3);
    int num = 0;
    for (int i = 0; i < tot; ++i) if (cnt3[pr[i]] == cnt1[pr[i]]) num |= (1 << i);
    for (int i = 0; i < tot; ++i) if (cnt3[pr[i]] == cnt2[pr[i]]) num |= (1 << (tot + i));
    ++f[num];
  }
  maxl = tot << 1; maxs = 1 << maxl;
  FMT(f, 1);
  for (int i = 0; i < maxs; ++i) f[i] = fpow(2, f[i]);
  FMT(f, -1); cerr << f[maxs - 1] << "\n"; FMT(f, 1);
  for (int i = 0; i < maxs; ++i) {
    int res = 0;
    for (int j = i; j < maxs; j = (j + 1) | i) {
      int tmp = (i64)f[j] * _2inv % MOD;
      if (__builtin_popcount((maxs - 1) ^ j) & 1) Sub(res, tmp);
      else Add(res, tmp);
    }
    ans[i] = res;
  }
}

int Solve(int x) {
  if (x % G || L % x || x > n) return 0;
  if (bak.count(x)) return bak[x];
  cnt3.clear(); factor(x, cnt3);
  int num = 0;
  for (int i = 0; i < tot; ++i) if (cnt3[pr[i]] == cnt1[pr[i]]) num |= (1 << i);
  for (int i = 0; i < tot; ++i) if (cnt3[pr[i]] == cnt2[pr[i]]) num |= (1 << (tot + i));
  return bak[x] = ans[num];
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> G >> L >> Q;
  if (L % G) {
    while (Q--) cout << 0 << "\n";
    return 0;
  }
  pre_work();
  while (Q--) {
    int x; cin >> x;
    cout << Solve(x) << "\n";
  }
  return 0;
}

