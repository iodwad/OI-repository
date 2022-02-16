// by iodwad
#include <cstdio>
#include <iostream>

using namespace std;

using i64 = long long;

const int MOD = 1e9 + 7;
const int MAXN = 2e5;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}

int n, K, sum, ans;
int frac[MAXN + 5], ifrac[MAXN + 5];

int calc(int n, int m) {
  int res = 0;
  for (int i = 0; i <= m; ++i) {
    int tmp = (i64)ifrac[i] * ifrac[m - i] % MOD * fpow(m - i, n) % MOD;
    if (i & 1) res = sub(res, tmp);
    else res = add(res, tmp);
  }
  return res;
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  frac[0] = 1;
  for (int i = 1; i <= MAXN; ++i) frac[i] = (i64)frac[i - 1] * i % MOD;
  ifrac[MAXN] = fpow(frac[MAXN], MOD - 2);
  for (int i = MAXN - 1; ~i; --i) ifrac[i] = (i64)ifrac[i + 1] * (i + 1) % MOD;
  cin >> n >> K;
  for (int i = 1, v; i <= n; ++i) cin >> v, sum = add(sum, v);
  ans = (i64)sum * add(calc(n, K), (i64)(n - 1) * calc(n - 1, K) % MOD) % MOD;
  cout << ans << "\n";
  return 0;
}
