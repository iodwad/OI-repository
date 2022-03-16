// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>

#define int long long

using namespace std;

const int MAXL = 1 << 18;
const int MAXN = 2e5;
const int INF = 0x3f3f3f3f3f3f3f3f;

struct ZKW {
  int N;
  int minv[MAXL << 1 | 1];
  void init(int n) { 
    ++n;
    N = 1 << __lg(n);
    if ((n & -n) != n) N <<= 1;
    memset(minv, 0x3f, sizeof(minv));
  }
  void modify(int pos, int v) {
    pos += N; minv[pos] = v;
    for (int i = pos >> 1; i; i >>= 1) 
      minv[i] = min(minv[i << 1], minv[i << 1 | 1]);
  }
  int query(int l, int r) {
    int res = INF;
    for (int L = l + N - 1, R = r + N + 1; L ^ R ^ 1; L >>= 1, R >>= 1) {
      if (~L & 1) res = min(res, minv[L ^ 1]);
      if (R & 1) res = min(res, minv[R ^ 1]);
    }
    return res;
  }
} t0, t1;

int n, m, W[MAXN + 5], sum[MAXN + 5];
int K, A[MAXN + 5], L[MAXN + 5];

int Solve() {
  int res = 0;
  for (int i = 1; i <= K; ++i) t0.modify(A[i], L[i] - sum[A[i]]), t1.modify(A[i], L[i] + sum[A[i]]);
  for (int i = 1; i <= K; ++i) {
    int l = 1, r = A[i] - 1, tmp = A[i];
    while (l <= r) {
      int mid = (l + r) >> 1;
      if (t1.query(mid, A[i] - 1) <= sum[A[i]] + L[i]) { l = mid + 1; continue; } 
      if (t0.query(max(1ll, 2 * mid - A[i] + 1), mid) <= sum[A[i]] + L[i] - 2 * sum[mid]) { l = mid + 1; continue; }
      if (2 * mid - A[i] >= 1 && t0.query(2 * mid - A[i], 2 * mid - A[i]) < sum[A[i]] + L[i] - 2 * sum[mid]) { l = mid + 1; continue; }
      tmp = mid;
      r = mid - 1;
    }
    res += A[i] - tmp + 1;
    l = A[i] + 1; r = n; tmp = A[i];
    while (l <= r) {
      int mid = (l + r) >> 1;
      if (t0.query(A[i] + 1, mid) <= L[i] - sum[A[i]]) { r = mid - 1; continue; }
      if (t1.query(mid, min(n, 2 * mid - A[i])) <= L[i] + 2 * sum[mid] - sum[A[i]]) { r = mid - 1; continue; }
      tmp = mid;
      l = mid + 1;
    }
    res += tmp - A[i];
  }
  for (int i = 1; i <= K; ++i) t0.modify(A[i], INF - sum[A[i]]), t1.modify(A[i], INF + sum[A[i]]);
  return res;
}

signed main() {
  ios::sync_with_stdio(false); cin.tie(0);
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  cin >> n >> m;
  for (int i = 1; i < n; ++i) cin >> W[i];
  for (int i = 2; i <= n; ++i) sum[i] = sum[i - 1] + W[i - 1];
  t0.init(n); t1.init(n);
  for (int i = 1; i <= n; ++i) t0.modify(i, INF - sum[i]), t1.modify(i, INF + sum[i]);
  while (m--) {
    cin >> K;
    for (int i = 1; i <= K; ++i) cin >> A[i] >> L[i];
    int res = Solve();
    cout << res << "\n";
  }
  return 0;
}
