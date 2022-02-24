// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int MAXN = 50;
const int MAXM = 3e6;

double P, A[MAXM + 5], B[MAXM + 5];
int n, m, tot, arr[MAXN + 5], cnt[MAXN + 5];

int dfs(int x, int y) {
  if (x <= 0) return 0;
  int o = ++tot;
  double sum = 1;
  int cntt = 0;
  for (int i = 1; i <= y; ++i) cntt += cnt[i];
  double P = x == n ? 0 : ::P;
  for (int i = 1; i <= y; ++i) {
    int to = dfs(x - arr[i], i);
    double p = (1 - P) * cnt[i] / cntt;
    sum -= p * A[to];
    B[o] += p * B[to];
  }
  B[o] = (1 + B[o]) / sum;
  A[o] = P / sum;
  return o;
}

int main() {
  freopen("normal.in", "r", stdin);
  freopen("normal.out", "w", stdout);
  cin >> P >> n >> m;
  for (int i = 1; i <= m; ++i) cin >> arr[i];
  sort(arr + 1, arr + m + 1);
  int mm = unique(arr + 1, arr + m + 1) - arr - 1;
  for (int i = 1; i <= mm; ++i) 
    for (int j = 1; j <= m; ++j) cnt[i] += arr[i] == arr[j];
  m = mm;
  dfs(n, mm);
  printf("%.3lf\n", B[1]);
  return 0;
}
