// by iodwad
#include <cstdio>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAXN = 18;

int n;
char S[(1 << MAXN) + 5];
int a[1 << MAXN], rnk[1 << MAXN];

int main() {
  ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n >> S;
  iota(a, a + (1 << n), 0);
  sort(a, a + (1 << n), [&](int x, int y) { return S[x] < S[y]; });
  for (int i = 1; i < (1 << n); ++i) rnk[a[i]] = rnk[a[i - 1]] + (S[a[i]] != S[a[i - 1]]);
  for (int k = 1; k <= n; ++k) {
    auto comp = [&](int x, int y) { 
      return rnk[x] < rnk[y] || (rnk[x] == rnk[y] && rnk[x ^ (1 << (k - 1))] < rnk[y ^ (1 << (k - 1))]); 
    };
    static int buc[1 << MAXN], tmp[1 << MAXN];
    memset(buc, 0, sizeof(buc));
    for (int i = 0; i < (1 << n); ++i) ++buc[rnk[i]];
    for (int i = 1; i < (1 << n); ++i) buc[i] += buc[i - 1];
    for (int i = (1 << n) - 1; ~i; --i) {
      int now = a[i] ^ (1 << (k - 1));
      tmp[--buc[rnk[now]]] = now;
    }
    memcpy(a, tmp, sizeof(int) * (1 << n));
    memcpy(tmp, rnk, sizeof(int) * (1 << n));
    tmp[a[0]] = 0;
    for (int i = 1; i < (1 << n); ++i) tmp[a[i]] = tmp[a[i - 1]] + comp(a[i - 1], a[i]);
    memcpy(rnk, tmp, sizeof(int) * (1 << n));
  }
  for (int i = 0; i < (1 << n); ++i) cout << S[i ^ a[0]];
  return 0;
}
