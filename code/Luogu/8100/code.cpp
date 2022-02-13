// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const int MAXN = 5000;
const int MOD = 1e9 + 7;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }

int n, a[MAXN + 5];
int pos0[MAXN + 5], pos1[MAXN + 5], tot0, tot1;
int dp[MAXN + 5][MAXN + 5];

int lg[MAXN + 5];
struct RMQ {
  int minv[13][MAXN + 5], maxv[13][MAXN + 5];
  void init(int v) {
    for (int i = 1; i <= n; ++i) {
      if (a[i] % 2 == v) minv[0][i] = maxv[0][i] = a[i];
      else minv[0][i] = 1e9, maxv[0][i] = -1e9;
    }
    for (int i = 1; i < 13; ++i)  {
      for (int j = 1; j + (1 << i) - 1 <= n; ++j) {
        minv[i][j] = min(minv[i - 1][j], minv[i - 1][j + (1 << (i - 1))]);
        maxv[i][j] = max(maxv[i - 1][j], maxv[i - 1][j + (1 << (i - 1))]);
      }
    }
  }
  int qmin(int l, int r) {
    if (l > r) return 1e9;
    int len = lg[r - l + 1];
    return min(minv[len][l], minv[len][r - (1 << len) + 1]);
  }
  int qmax(int l, int r) {
    if (l > r) return -1e9;
    int len = lg[r - l + 1];
    return max(maxv[len][l], maxv[len][r - (1 << len) + 1]);
  }
} t0, t1;

void Solve() {
  cin >> n;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  t0.init(0);
  t1.init(1);
  tot0 = tot1 = 0;
  for (int i = 1; i <= n; ++i) {
    if (a[i] % 2) pos1[++tot1] = i;
    else pos0[++tot0] = i;
  }
  for (int i = 0; i <= n; ++i) memset(dp[i], 0, sizeof(int) * (n + 1));
  dp[0][0] = 1;
  for (int i = 0; i <= tot0; ++i) {
    for (int j = 0; j <= tot1; ++j) {
      if (!dp[i][j]) continue;
      if (i + 1 <= tot0) {
        int now = pos0[i + 1];
        int minn = t1.qmin(now + 1, pos1[j]), maxx = t1.qmax(now + 1, pos1[j]);
        if (a[now] - 1 <= minn && maxx <= a[now] + 1) 
          dp[i + 1][j] = add(dp[i + 1][j], dp[i][j]);
      }
      if (j + 1 <= tot1) {
        int now = pos1[j + 1];
        int minn = t0.qmin(now + 1, pos0[i]), maxx = t0.qmax(now + 1, pos0[i]);
        if (a[now] - 1 <= minn && maxx <= a[now] + 1) 
          dp[i][j + 1] = add(dp[i][j + 1], dp[i][j]);
      }
    }
  }
  cout << dp[tot0][tot1] << "\n";
}

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  lg[0] = -1;
  for (int i = 1; i <= MAXN; ++i) lg[i] = lg[i >> 1] + 1;
  int cases; cin >> cases;
  while (cases--) {
    Solve();
  }
  return 0;
}
