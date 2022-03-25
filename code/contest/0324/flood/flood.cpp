// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>

#define int long long

using namespace std;

int n, m, Q, MOD, w, g;

int fpow(int x, int y, int mod = MOD) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = i * i % MOD)
    if (y & 1) res = res * i % MOD;
  return res;
}

int get_g(int MOD) {
  static vector<int> pr; pr.clear();
  int tmp = MOD - 1;
  for (int i = 2; i * i <= tmp; ++i) {
    if (tmp % i) continue;
    pr.push_back(i);
    while (tmp % i == 0) tmp /= i;
  }
  if (tmp) pr.push_back(tmp);
  for (int i = 2; ; ++i) {
    bool flag = false;
    for (int j : pr) if (fpow(i, (MOD - 1) / j) == 1) { flag = true; break; }
    if (!flag) return i;
  }
}

int solve(int r, int m) {
  int res = 0;
  for (int i = 0; i <= m; ++i) 
    res = (res + fpow(fpow(w, MOD - 2), i * r) * fpow(1 + fpow(w, i), 2 * n)) % MOD;
  return res * fpow(m + 1, MOD - 2) % MOD;
}

void Solve() {
  cin >> m; ++m;
  w = fpow(g, (MOD - 1) / (m + 1));
  int res = (solve(n, m) - solve(n + 1, m) + MOD) % MOD;
  cout << res << "\n";
}

signed main() {
  // ios::sync_with_stdio(false); cin.tie(0);
  freopen("flood.in", "r", stdin);
  freopen("flood.out", "w", stdout);
  cin >> n >> Q >> MOD;
  g = get_g(MOD);
  // int g  = fpow(w, (MOD - 1) / m + 1);
  n -= 2;
  while (Q--) {
    Solve();
  }
  return 0;
}
