// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

using i64 = long long;

const int MAXN = 200;
const int MAXK = 1000;
const int MOD = 998244353;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }
void Add(int &x, int y) { x = add(x, y); }

int n, K, a[MAXN + 5];
int f[2][MAXN + 5][MAXK + 5];

int main() {
  freopen("simple.in", "r", stdin);
  freopen("simple.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> K;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  sort(a + 1, a + n + 1);
  for (int i = n; i >= 1; --i) a[i] = a[i] - a[i - 1];
  f[0][0][0] = 1;
  for (int i = 0, o = 0; i < n; ++i, o ^= 1) {
    memset(f[o ^ 1], 0, sizeof(f[o ^ 1]));
    for (int j = 0; j <= i; ++j) {
      for (int k = 0; k + j * a[i + 1] <= K; ++k) {
        if (!f[o][j][k]) continue;
        int nk = k + j * a[i + 1];
        Add(f[o ^ 1][j + 1][nk], f[o][j][k]);
        Add(f[o ^ 1][j][nk], f[o][j][k]);
        if (j) {
          Add(f[o ^ 1][j][nk], (i64)f[o][j][k] * j % MOD);
          Add(f[o ^ 1][j - 1][nk], (i64)f[o][j][k] * j % MOD);
        }
      }
    }
  }
  int ans = 0;
  for (int i = 0; i <= K; ++i) Add(ans, f[n & 1][0][i]);
  cout << ans << "\n";
  return 0;
}
