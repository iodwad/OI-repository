// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>

#define int long long

using namespace std;

const int MAXN = 1e5;

int n, tim, K, p[MAXN + 5];
int dfn[MAXN + 5], dep[MAXN + 5];
vector<int> e[MAXN + 5];

void dfs(int x) {
  dfn[x] = ++tim; 
  for (int to : e[x]) {
    dep[to] = dep[x] + 1;
    dfs(to);
  }
}

signed main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n;
  for (int i = 1; i <= n; ++i) cin >> p[i];
  for (int i = n; i >= 1; --i) e[p[i] + 1].push_back(i);
  dfs(n + 1);
  K = 2 * n;
  cout << K << "\n";
  for (int i = 1; i <= n; ++i)
    cout << (dep[1] - dep[i] + 1) * K + (n - dfn[i]) << "\n";
  return 0;
}

