// by iodwad
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;

using i64 = long long;

const int MOD = 998244353;
const int PR[] = {0, 2, 3, 5, 7, 11, 13, 17, 19};

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
int sub(int x, int y) { return x < y ? x - y + MOD : x - y; }
int fpow(int x, int y) {
  int res = 1;
  for (int i = x; y; y >>= 1, i = (i64)i * i % MOD)
    if (y & 1) res = (i64)res * i % MOD;
  return res;
}

i64 n, C, tc;

namespace Number {
  const int maxn = 2e7;
  // const int maxn = 1;
  int prr[9];
  int pn;
  int pr[maxn + 5], cnt[maxn + 5], num[maxn + 5], mul[maxn + 5];
  int sumf[maxn + 5];
  bool np[maxn + 5];
  void init() {
    prr[0] = 1;
    for (int i = 1; i <= 8; ++i) prr[i] = prr[i - 1] * PR[i];
    num[1] = 1; cnt[1] = 0; mul[1] = 1; sumf[1] = C;
    tc = (i64)C * fpow(sub(1, C), MOD - 2) % MOD;
    for (int i = 2; i <= maxn; ++i) {
      if (!np[i]) pr[++pn] = i, cnt[i] = 1, num[i] = 2, mul[i] = i;
      for (int j = 1; i * pr[j] <= maxn && j <= pn; ++j) {
        np[i * pr[j]] = 1;
        if (i % pr[j] == 0) {
          cnt[i * pr[j]] = cnt[i];
          mul[i * pr[j]] = mul[i];
          break;
        }
        cnt[i * pr[j]] = cnt[i] + 1;
        mul[i * pr[j]] = mul[i] * pr[j];
      }
      num[i] = num[i / mul[i]] * prr[cnt[i]];
      if (num[i] == i) {
        for (int j = 2; j * j <= i; ++j) {
          if (i % j) continue;
          sumf[i] = add(sumf[i], sumf[j]);
          if (j * j != i) sumf[i] = add(sumf[i], sumf[i / j]);
        }
        sumf[i] = add(sumf[i], C);
        sumf[i] = add(C, (i64)sumf[i] * tc % MOD);
      } else sumf[i] = sumf[num[i]];
    }
    for (int i = 1; i <= maxn; ++i) sumf[i] = add(sumf[i - 1], sumf[i]);
  }
}

using namespace Number;

unordered_map<i64, int> F;

int get_g(i64 x) {
  if (x == 0) return 0;
  int c = sub(0, tc);
  return 1 + (i64)c * ((x - 1) % MOD) % MOD;
}

int get_f(i64 x) {
  if (x <= maxn) return sumf[x];
  else if (F.count(x)) return F[x];
  int res = 0;
  res = (i64)C * (x % MOD) % MOD;
  for (i64 l = 2, r = 0; l <= x; l = r + 1) {
    r = x / (x / l);
    res = sub(res, (i64)sub(get_g(r), get_g(l - 1)) * get_f(x / l) % MOD);
  }
  return F[x] = res;
}

int main() {
  freopen("bigben.in", "r", stdin);
  freopen("bigben.out", "w", stdout);
  cin >> n >> C; C %= MOD;
  if (C == 1) {
    cout << (n <= 2 ? 1 : 0) << "\n";
    return 0;
  }
  init();
  int ans = add(fpow(sub(1, C), n % (MOD - 1)), (i64)fpow(sub(1, C), (n - 1) % (MOD - 1)) * get_f(n) % MOD);
  cout << ans << "\n";
  return 0;
}
