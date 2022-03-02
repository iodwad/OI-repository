// by iodwad
#include <cstdio>
#include <iostream>

using namespace std;

using i64 = long long;

const int MAXN = 1e7;
const int MAXK = 100;
const int COF[] = {0, 2, 16, 272, 7936, 353792};

int n, K, MOD, ans;
int frac[MAXN + 5], ifrac[MAXN + 5], S[MAXK + 5][MAXK + 5];

int add(i64 x, i64 y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}
int Inv(int x) { return fpow(x, MOD - 2); }
int binom(int x, int y) {
  if (x < y || x < 0 || y < 0) return 0;
  int res = 1;
  for (int i = x - y + 1; i <= x; ++i) res = (i64)res * i % MOD;
  for (int i = 1; i <= y; ++i) res = (i64)res * Inv(i) % MOD;
  return res;
}

int a[MAXK + 5];

int calc(int x) {
  int res = 1, tot = n, cnt = 0;
  for (int i = 1; i <= x; ++i) tot -= 2 * a[i] + 1, cnt += a[i];
  if (tot < 0) return 0;
  res = (i64)binom(n, tot) * frac[n - tot] % MOD * frac[tot] % MOD;
  for (int i = 1; i <= x; ++i) res = (i64)res * ifrac[2 * a[i] + 1] % MOD;
  for (int i = 1; i <= x; ++i) res = (i64)res * COF[a[i]] % MOD;
  res = (i64)res * binom(tot + x, x) % MOD;
  res = (i64)res * S[K][cnt] % MOD * frac[cnt] % MOD;
  return res;
}

void dfs(int x, int y) {
  if (y > 1) ans = add(ans, calc(y - 1));
  if (x == 0) return;
  for (int i = 1; i <= x; ++i) {
    a[y] = i;
    dfs(x - i, y + 1);
  }
}

void pre_work() {
  S[0][0] = frac[0] = ifrac[0] = 1;
  for (int i = 1; i <= MAXN; ++i) frac[i] = (i64)frac[i - 1] * i % MOD;
  ifrac[MAXN] = Inv(frac[MAXN]);
  for (int i = MAXN - 1; ~i; --i) ifrac[i] = (i64)ifrac[i + 1] * (i + 1) % MOD;
  for (int i = 1; i <= MAXK; ++i) {
    for (int j = 1; j <= i; ++j) 
      S[i][j] = add(S[i - 1][j - 1], (i64)j * S[i - 1][j]);
  }
}

int main() {
  freopen("permutation.in", "r", stdin);
  freopen("permutation.out", "w", stdout);
  cin >> n >> K >> MOD;
  pre_work();
  dfs(K, 1);
  int tmp = 1;
  for (int i = 1; i <= n; ++i) tmp = (i64)tmp * i % MOD;
  cerr << ans << "\n";
  ans = (i64)ans * Inv(tmp) % MOD;
  cout << ans << "\n";
  return 0;
}
