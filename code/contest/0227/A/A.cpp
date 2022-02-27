// by iodwad
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>

#define int long long

using namespace std;

const int MAXN = 1e5;

int n, L, R;
int a[MAXN + 5], d[MAXN + 5], s[MAXN + 5];

struct BIT {
  int bit[MAXN + 5];
  BIT() {}
  void clear() { memset(bit, 0, sizeof(bit)); }
  void update(int pos, int val) {
    for (int i = pos; i <= n; i += i & (-i)) bit[i] += val;
  }
  int query(int pos) {
    int res = 0;
    for (int i = pos; i; i -= i & -i) res += bit[i];
    return res;
  }
} t;

void work() {
  t.clear();
  for (int i = 1; i <= n; ++i) {
    int tot = i - t.query(a[i]);
    t.update(a[i], 1);
    ++s[2]; --s[tot + 1];
  }
}

signed main() {
  freopen("A.in", "r", stdin);
  freopen("A.out", "w", stdout);
  cin >> n >> L >> R;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 2; i <= n; ++i) d[i + 1] -= n - i + 1;
  work(); 
  reverse(a + 1, a + n + 1);
  work();
  for (int i = 1; i <= n; ++i) s[i] += s[i - 1];
  for (int i = 1; i <= n; ++i) d[i] += d[i - 1] + s[i];
  d[1] += n;
  int ans = 0;
  for (int i = L; i <= R; ++i) ans ^= d[i];
  cout << ans << "\n";
  return 0;
}
