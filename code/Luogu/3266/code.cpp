// by iodwad
#include <cstdio>
#include <iostream>

using namespace std;

using i64 = long long;

const int MOD = 1e9 + 7;
const int MAXN = 3e6;

int n, m;
int frac[MAXN + 5], ifrac[MAXN + 5];

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}
int Inv(int x) { return fpow(x, MOD - 2); }

int C(int x, int y) { return (i64)frac[x] * ifrac[y] % MOD * ifrac[x - y] % MOD; }
int F(int x0, int y0, int x, int y) { return C(x - x0 + y - y0, x - x0); }

pair<int, int> flip(int x, int y, int m) { return {y - m, x + m}; }

int main() {
  // ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  frac[0] = 1;
  for (int i = 1; i <= MAXN; ++i) frac[i] = (i64)frac[i - 1] * i % MOD;
  ifrac[MAXN] = Inv(frac[MAXN]);
  for (int i = MAXN - 1; ~i; --i) ifrac[i] = (i64)ifrac[i + 1] * (i + 1) % MOD;
  cin >> n >> m;
  int ans = F(0, 0, m + n, n);
  for (int i = 1, x = 0, y = 0; x <= m + n && y <= n; i ^= 1) {
    auto tmp = flip(x, y, i ? 2 : -(m + 1));
    x = tmp.first, y = tmp.second;
    if (x > m + n || y > n) break;
    if (i) ans = sub(ans, F(x, y, m + n, n));
    else ans = add(ans, F(x, y, m + n, n));
  }
  for (int i = 1, x = 0, y = 0; x <= m + n && y <= n; i ^= 1) {
    auto tmp = flip(x, y, !i ? 2 : -(m + 1));
    x = tmp.first, y = tmp.second;
    if (x > m + n || y > n) break;
    if (i) ans = sub(ans, F(x, y, m + n, n));
    else ans = add(ans, F(x, y, m + n, n));
  }
  cout << ans << "\n";
  return 0;
}
