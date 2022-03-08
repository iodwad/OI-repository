// by iodwad
#include <cstdio>
#include <iostream>

using namespace std;

using i64 = long long;

const int MOD = 998244353;
const int G = 3;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}
int Inv(int x) { return fpow(x, MOD - 2); }

int n, p, K, w;

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> p >> K;
  w = fpow(G, (MOD - 1) / K);
  int ans = 0;
  for (int i = 0; i < K; ++i) {
    int t = (i64)add(1, (i64)p * fpow(w, i) % MOD) * Inv(add(1, p)) % MOD;
    int res = 0;
    if (t == 1) res = n;
    else res = (i64)sub(1, fpow(t, n)) * Inv(sub(1, t)) % MOD;
    ans = add(ans, (i64)fpow(w, i) * res % MOD);
  }
  ans = (i64)ans * fpow(add(1, p), n - 1) % MOD * p % MOD * Inv(K) % MOD;
  cout << ans << "\n";
  return 0;
}
