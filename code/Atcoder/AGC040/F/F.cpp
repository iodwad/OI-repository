// by iodwad
#include <cstdio>
#include <iostream>

using namespace std;

using i64 = long long;

const int MOD = 998244353;
const int MAXN = 2e7;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}
int Inv(int x) { return fpow(x, MOD - 2); }

int n, A, B, ans;
int frac[MAXN + 5], ifrac[MAXN + 5];

int C(int x, int y) { return x < y || y < 0 ? 0 : (i64)frac[x] * ifrac[y] % MOD * ifrac[x - y] % MOD; }
int F(int x, int y) { return C(x + y - 1, y - 1); }

int main() {
  freopen("F.in", "r", stdin);
  freopen("F.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  frac[0] = 1;
  for (int i = 1; i <= MAXN; ++i) frac[i] = (i64)frac[i - 1] * i % MOD;
  ifrac[MAXN] = Inv(frac[MAXN]);
  for (int i = MAXN - 1; ~i; --i) ifrac[i] = (i64)ifrac[i + 1] * (i + 1) % MOD;
  cin >> n >> A >> B;
  if (!A && !B) {
    cout << 1 << "\n";
    return 0;
  }
  if (A < B) swap(A, B);
  if (A + B == n) {
    ans = (i64)sub(C(A - 1 + B, A - 1), C(A - 1 + B, A));
  }
  for (int i = 0; i <= A; ++i) {
    int res = sub(C(A - 1 + B - i, A - 1), C(A - 1 + B - i, A));
    res = (i64)res * F(n - A - (B - i) - 1, i + 1) % MOD;
    ans = add(ans, res);
  }
  cout << ans << "\n";
  return 0;
}

