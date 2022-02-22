// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 3000;

int ans = 2e9;
int n, dep[MAXN + 5], cnt[MAXN + 5];
char col[MAXN + 5];
vector<int> e[MAXN + 5];

int L[MAXN + 5], R[MAXN + 5];
void dfs(int x, int fa) {
  dep[x] = dep[fa] + 1; L[x] = R[x] = cnt[x] = 0;
  for (int to : e[x]) {
    if (to == fa) continue;
    dfs(to, x);
    cnt[x] += cnt[to];
  }
  int maxp = 0, sum = 0, minp = 0, mmxp = 0;
  for (int to : e[x]) {
    if (to == fa) continue;
    if (R[to] >= R[maxp]) mmxp = maxp, maxp = to;
    if (L[to] > L[minp]) minp = to;
  }
  for (int to : e[x]) {
    if (to == fa) continue;
    if (to != maxp) sum += R[to];
    R[x] += R[to];
  }
  L[x] = max(R[mmxp], L[minp]) - sum;
  L[x] = max(0, L[x]);
  if (fa) {
    L[x] += cnt[x], R[x] += cnt[x];
    if (col[x] == '1') {
      ++L[x]; ++R[x];
      cnt[x] += 1;
    }
  }
}

int calc(int x) {
  dep[x] = 0; dfs(x, 0);
  if (L[x] == 0 && R[x] % 2 == 0) return R[x] / 2;
  else return 2e9;
}

int main() {
  freopen("E.in", "r", stdin);
  freopen("E.out", "w", stdout);
  // ios::sync_with_stdio(false); cin.tie(0);
  cin >> n;
  cin >> (col + 1);
  for (int i = 1, u, v; i < n; ++i) {
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  dep[0] = -1; 
  for (int i = 1; i <= n; ++i) ans = min(ans, calc(i));
  cout << (ans == 2e9 ? -1 : ans) << "\n";
  return 0;
}
