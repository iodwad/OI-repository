// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int MAXN = 10;
const int MAXM = 100;

int n, m;
int T[1 << MAXN];
__float128 ans, C[MAXM + 5][MAXM + 5], f[1 << MAXN][MAXM + 5], g[1 << MAXN][MAXM + 5];
vector<int> e[MAXN + 5];

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n >> m;
  for (int i = 1, u, v; i <= m; ++i) {
    cin >> u >> v;
    e[u].push_back(v);
    e[v].push_back(u);
  }
  C[0][0] = 1;
  for (int i = 1; i <= m; ++i) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j) C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
  }
  for (int S = 1; S < (1 << n); ++S) {
    int now = __builtin_ctz(S) + 1, las = S & (S - 1);
    T[S] = T[las];
    for (int i : e[now]) if ((las >> (i - 1)) & 1) ++T[S];
  }
  for (int S = 1; S < (1 << n); ++S) {
    int ns = S & (S - 1), lb = 1 << __builtin_ctz(S);
    for (int i = 0; i <= T[S]; ++i) {
      for (int ss = ns; ~ss; ss = (ss - 1) & ns) {
        for (int j = 0; j <= i; ++j) 
          f[S][i] += g[ss | lb][j] * C[T[S ^ (ss | lb)]][i - j];
        if (!ss) break;
      }
      g[S][i] = C[T[S]][i] - f[S][i];
    }
  }
  for (int i = 0; i <= m; ++i) ans += f[(1 << n) - 1][i] / C[m][i];
  ans /= m + 1;
  printf("%.6lf\n", double(ans));
  return 0;
}

