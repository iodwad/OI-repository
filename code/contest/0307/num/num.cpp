// by iodwad
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

typedef long long i64;

const int N = 1e6 + 5, maxn = 1e6, T = 1 << 17;

i64 mod, n;
i64 w[N];
int g1[N], g2[N], id1[N], id2[N], p1[N], p2[N], prime[N], tot, cnt;
bool flag[N];
int BLOCK, pw[40][2][T + 5];

void Ckmod(int &a, int b) {
  a += b;
  (a >= mod ? a -= mod : 0);
}
struct Matrix {
  int m[2][2];
} I, E;
Matrix operator*(Matrix a, Matrix b) {
  Matrix c;
  memset(c.m, 0, sizeof c.m);
  c.m[0][0] = (1ll * a.m[0][0] * b.m[0][0] + 1ll * a.m[0][1] * b.m[1][0]) % mod;
  c.m[0][1] = (1ll * a.m[0][0] * b.m[0][1] + 1ll * a.m[0][1] * b.m[1][1]) % mod;
  c.m[1][0] = (1ll * a.m[1][0] * b.m[0][0] + 1ll * a.m[1][1] * b.m[1][0]) % mod;
  c.m[1][1] = (1ll * a.m[1][0] * b.m[0][1] + 1ll * a.m[1][1] * b.m[1][1]) % mod;
  return c;
}
Matrix operator+(Matrix a, Matrix b) {
  Ckmod(a.m[0][0], b.m[0][0]);
  Ckmod(a.m[0][1], b.m[0][1]);
  Ckmod(a.m[1][0], b.m[1][0]);
  Ckmod(a.m[1][1], b.m[1][1]);
  return a;
}

int Pw(int k, int p) {
  if (!p)
    return 1;
  else
    return 1ll * pw[k][0][p & (T - 1)] * pw[k][1][p >> 17] % mod;
}
void Pre() {
  for (int k = 1; k < 40; ++k) {
    pw[k][0][0] = pw[k][1][0] = 1;
    for (int i = 1; i <= T; ++i) pw[k][0][i] = 1ll * pw[k][0][i - 1] * k % mod;
    for (int i = 1; i <= T; ++i) pw[k][1][i] = 1ll * pw[k][1][i - 1] * pw[k][0][T] % mod;
  }
  flag[1] = p1[1] = p2[1] = 1;
  for (int i = 2; i <= maxn; ++i) {
    if (!flag[i]) {
      prime[++cnt] = i;
      p1[cnt] = 1, p2[cnt] = i;
      p1[cnt] = (p1[cnt - 1] + p1[cnt]) % mod;
      p2[cnt] = (p2[cnt - 1] + p2[cnt]) % mod;
    }
    for (int j = 1; j <= cnt; ++j) {
      if (i * prime[j] > maxn)
        break;
      flag[i * prime[j]] = 1;
      if (i % prime[j] == 0)
        break;
    }
  }
}
Matrix S(i64 x, int y) {
  if (prime[y] >= x)
    return E;
  int k = (x <= BLOCK ? id1[x] : id2[n / x]);
  Matrix ans;
  memset(ans.m, 0, sizeof ans.m);
  ans.m[0][1] = (0ll + g1[k] + g2[k] - p1[y] - p2[y] + mod + mod) % mod;
  ans.m[1][0] = ans.m[1][1] = (g1[k] - p1[y] + mod) % mod;
  Matrix M;
  memset(M.m, 0, sizeof M.m);
  M.m[1][0] = M.m[1][1] = 1;
  for (int i = y + 1; i <= cnt && 1ll * prime[i] * prime[i] <= x; ++i) {
    for (i64 p = prime[i], e = 1; p <= x; ++e, p *= prime[i]) {
      int s = (p + Pw(e, prime[i])) % mod;
      M.m[0][1] = s;
      ans = (ans + M * (S(x / p, i) + (e != 1 ? I : E)));
    }
  }
  return ans;
}
signed main() {
  memset(&I, 0, sizeof I);
  memset(&E, 0, sizeof E);
  I.m[0][0] = I.m[1][1] = 1;
  scanf("%lld%lld", &n, &mod);
  Pre();
  BLOCK = sqrt(n);
  for (i64 l = 1, r; l <= n; l = r + 1) {
    r = n / (n / l);
    w[++tot] = n / l;
    int X = w[tot] % mod;
    g1[tot] = X;
    if (w[tot] & 1ll)
      g2[tot] = 1ll * ((w[tot] + 1) / 2ll) % mod * (w[tot] % mod) % mod;
    else
      g2[tot] = 1ll * (w[tot] / 2ll) % mod * ((w[tot] + 1) % mod) % mod;
    --g1[tot], --g2[tot];
    if (n / l <= BLOCK)
      id1[n / l] = tot;
    else
      id2[n / (n / l)] = tot;
  }
  for (int i = 1; i <= cnt; ++i) {
    for (int j = 1; j <= tot && 1ll * prime[i] * prime[i] <= w[j]; ++j) {
      int k = (w[j] / prime[i] <= BLOCK ? id1[w[j] / prime[i]] : id2[n / (w[j] / prime[i])]);
      g1[j] = (g1[j] - (g1[k] - p1[i - 1] + mod) % mod + mod) % mod;
      g2[j] = (g2[j] - 1ll * prime[i] * (g2[k] - p2[i - 1] + mod) % mod + mod) % mod;
    }
  }
  Matrix ans = S(n, 0);
  int res = (ans.m[0][1] + ans.m[1][1]) % mod;
  printf("%lld\n", (res - n % mod + mod + 1) % mod);
  return 0;}
