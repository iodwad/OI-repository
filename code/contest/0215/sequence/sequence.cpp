// by iodwad
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

const int MAXN = 500;

int n, ans = -1e9, a[MAXN + 5], b[MAXN + 5];
int cnt[MAXN + 5][MAXN + 5], maxx[MAXN + 5], maxp[MAXN + 5];

namespace Sieve {
  int pn, pr[MAXN + 5], lpf[MAXN + 5];
  bool np[MAXN + 5];
  void sieve() {
    for (int i = 2; i <= MAXN; ++i) {
      if (!np[i]) pr[++pn] = i, lpf[i] = i;
      for (int j = 1; i * pr[j] <= MAXN && j <= pn; ++j) {
        np[i * pr[j]] = 1;
        lpf[i * pr[j]] = pr[j];
        if (i % pr[j] == 0) break;
      }
    }
  }
}

using namespace Sieve;

void factor(int x) {
  int i = a[x];
  while (i != 1) {
    ++cnt[x][lpf[i]];
    maxp[x] = max(maxp[x], lpf[i]);
    i /= lpf[i];
  }
}

int tot;
int num[MAXN + 5];

int calc() {
  static int sum[MAXN + 5];
  memset(sum, 0, sizeof(sum));
  int res = 0;
  for (int i = 1; i <= n; ++i) {
    bool flag = true;
    for (int j = 1; j <= 8; ++j) 
      if (num[pr[j]] < cnt[i][pr[j]]) { flag = false; break; }
    if (!flag) continue;
    if (maxp[i] < 23) res += b[i];
    else sum[maxp[i]] += b[i];
  }
  for (int i = 1; i <= MAXN; ++i) res += sum[i] > 0 ? sum[i] : 0;
  return res;
}

void dfs(int x) {
  if (x == 20) {
    ++tot;
    ans = max(ans, calc());
    return;
  }
  if (np[x]) return dfs(x + 1);
  for (int i = 0; i <= maxx[x]; ++i) {
    num[x] = i;
    dfs(x + 1);
  }
}

int main() {
  freopen("sequence.in", "r", stdin);
  freopen("sequence.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  sieve();
  cin >> n;
  for (int i = 1; i <= n; ++i) cin >> a[i] >> b[i];
  for (int i = 1; i <= n; ++i) factor(i);
  for (int i = 1; i <= MAXN; ++i) 
    for (int j = 1; j <= n; ++j) maxx[i] = max(maxx[i], cnt[j][i]);
  dfs(1);
  cout << ans << "\n";
  return 0;
}
