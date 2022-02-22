// by iodwad
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const int MOD = 998244353;
const int MAXN = 10;
const int MAXM = 30;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }

int n, m, a[MAXM + 5], b[MAXM + 5], c[MAXM + 5], d[MAXM + 5];
int dp[3628800 + 5];
vector<int> s, t;
// map<vector<int>, int> dp;

int cantor(vector<int> p) {
  int res = 0;
  for (int i = n - 2, tmp = 1; i >= 0; tmp = tmp * (n - i), --i) {
    int cnt = 0;
    for (int j = i; j < n; ++j) if (p[i] > p[j]) ++cnt;
    res += cnt * tmp;
  }
  return res;
}

int dfs(vector<int> p) {
  int now = cantor(p);
  if (~dp[now]) return dp[now];
  if (p == t) return 1;
  static int pos[MAXN + 5];
  int res = 0;
  for (int i = 0; i < n; ++i) pos[p[i]] = i;
  for (int i = 1; i <= m; ++i) 
    if (pos[a[i]] < pos[b[i]] && pos[c[i]] > pos[d[i]]) return dp[now] = 0;
  for (int i = 0; i < n - 1; ++i) {
    if (p[i] < p[i + 1]) {
      swap(p[i], p[i + 1]);
      res = add(res, dfs(p));
      swap(p[i], p[i + 1]);
    }
  }
  return dp[now] = res;
}

int main() {
  freopen("perm.in", "r", stdin);
  freopen("perm.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> m;
  for (int i = 1; i <= m; ++i) cin >> a[i] >> b[i] >> c[i] >> d[i];
  for (int i = 1; i <= n; ++i) s.push_back(i);
  t = s; reverse(t.begin(), t.end());
  memset(dp, -1, sizeof(dp));
  cout << dfs(s) << "\n";
  return 0;
}
