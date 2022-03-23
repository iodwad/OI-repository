// by iodwad
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>

#define int long long

using namespace std;

const int MAXN = 18;
const int MAXL = 1 << 18;

int n;
int e[MAXN + 5][MAXN + 5];
int dp[MAXL][MAXN + 5], g[MAXN + 5][MAXL], ans[MAXL];
map<vector<int>, int> buc;

void FMT(int *f) { // pre sum
  // for (int S = MAXL - 1; ~S; --S) 
  for (int i = 1; i <= n; ++i)
    for (int S = 1; S < (1 << n); ++S)
      if (S & (1 << (i - 1)))
        f[S] += f[S ^ (1 << (i - 1))];
}

void IFMT(int *f) { // suf sum
  for (int i = 1; i <= n; ++i) 
    for (int S = 0; S < (1 << n); ++S) 
      if (!(S & (1 << (i - 1)))) 
        f[S] -= f[S | (1 << (i - 1))];
}

vector<int> get_num(int S) {
  static vector<int> res; res.clear();
  res.push_back(0);
  for (int i = 1; i < n; ++i) {
    if (S & (1 << (i - 1))) ++res[res.size() - 1];
    else if (res.back()) res.push_back(0);
  }
  if (res.back() == 0) res.pop_back();
  sort(res.begin(), res.end());
  return res;
}

vector<int> num;
int f[MAXL];

int calc() {
  int res = 0, S = (1 << n) - 1;
  for (int T = 0; T <= S; ++T) {
    if (__builtin_popcount(S ^ T) & 1) res -= f[T];
    else res += f[T];
  }
  return res;
}

void dfs(int x, int las = 1) {
  // buc[num] = calc();
  if (!x) {
    int res = calc();
    buc[num] = res;
    // for (int i : num) cout << i << " ";
    // cout << "res = " << res << "\n";
    return;
  }
  // vector<int> bak = f;
  int bak[MAXL]; memcpy(bak, f, sizeof(int) * (1 << n));
  for (int i = las; i <= x; ++i) {
    if (i > 1) num.push_back(i - 1);
    for (int S = 0; S < (1 << n); ++S) f[S] = f[S] * g[i][S];
    dfs(x - i, i);
    if (i > 1) num.pop_back(); 
    memcpy(f, bak, sizeof(int) * (1 << n));
  }
}

signed main() {
  // ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n;
  static char str[MAXN + 5];
  for (int i = 1; i <= n; ++i) {
    cin >> str;
    for (int j = 1; j <= n; ++j) e[i][j] = str[j - 1] - '0';
  }
  for (int i = 1; i <= n; ++i) dp[1 << (i - 1)][i] = 1;
  for (int S = 1; S < MAXL; ++S) {
    for (int i = 1; i <= n; ++i) {
      if (!dp[S][i]) continue;
      for (int j = 1; j <= n; ++j) {
        if (S & (1 << (j - 1))) continue;
        if (!e[i][j]) continue;
        dp[S | (1 << (j - 1))][j] += dp[S][i];
      }
    }
  }
  for (int S = 1; S < MAXL; ++S) 
    for (int i = 1; i <= n; ++i)
      g[__builtin_popcount(S)][S] += dp[S][i];
  for (int i = 1; i <= n; ++i) FMT(g[i]);
  for (int i = 0; i < (1 << n); ++i) f[i] = 1;
  dfs(n);
  for (int S = 0; S < (1 << (n - 1)); ++S) ans[S] = buc[get_num(S)];
  IFMT(ans);
  for (int i = 0; i < (1 << (n - 1)); ++i) cout << ans[i] << " ";
  return 0;
}

