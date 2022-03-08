// by iodwad
#include <cstdio>
#include <iostream>

using namespace std;

const int MAXN = 20;
const int MOD = 1e9 + 7;

int add(int x, int y ) { return x + y >= MOD ? x + y - MOD : x + y; }

int n, a[MAXN + 5];
int dp[1 << MAXN];

bool check(int S) {
  static int aa[MAXN + 5], bb[MAXN + 5]; int t0 = 0, t1 = 0;
  for (int i = 0; i < n; ++i) if (!(S & (1 << i))) aa[++t0] = i;
  int p = __builtin_popcount(S);
  for (int i = 0; i < n; ++i) if (a[i] > p) bb[++t1] = i;
  for (int i = 1; i <= t1; ++i) if (aa[i] < bb[i]) return 0;
  return 1;
}

int main() {
  freopen("permutation.in", "r", stdin);
  freopen("permutation.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n;
  for (int i = 0; i < n; ++i) cin >> a[i];
  dp[0] = 1;
  for (int S = 0; S < (1 << n); ++S) {
    if (!check(S)) continue;
    for (int i = 0; i < n; ++i) {
      if (S & (1 << i)) continue;
      int ns = S | (1 << i);
      dp[ns] = add(dp[ns], dp[S]);
    }
  }
  cout << dp[(1 << n) - 1];
  return 0;
}
