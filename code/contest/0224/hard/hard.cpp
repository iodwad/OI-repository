// by iodwad
#include <cstdio>
#include <iostream>
#include <cmath>

#define int long long

using namespace std;

const int MAXN = 1e5 + 10;
const int INF = 2e16;
const double eps = 1e-6;
const double PI = acos(-1);

struct Z { 
  int a, b;
  Z(int _a = 0, int _b = 0) : a(_a), b(_b) {}
  Z operator + (const Z &x) const { return Z(a + x.a, b + x.b); }
  Z operator - (const Z &x) const { return Z(a - x.a, b - x.b); }
  Z operator * (const Z &x) const { return Z(a * x.a - b * x.b, a * x.b + b * x.a); }
  Z conj() { return Z(a, -b); }
  bool operator == (const Z &x) const { return a == x.a && b == x.b; }
  int norm() { return a * a + b * b; }
  Z operator % (Z &b) const {
    Z tau = (*this) * b.conj(), n; double d = b.norm(); 
    n = Z(round(1. * tau.a / d), round(1. * tau.b / d));
    return (*this) - n * b;
  }
} p[MAXN], g;

int n, m, Ans;
int Xmax = -INF, Xmin = INF, Ymax = -INF, Ymin = INF;

Z Gcd(Z a, Z b) { return (b == Z(0, 0)) ? a : Gcd(b, a % b); }

signed main () {
  cin >> n;
  for (int i = 1; i <= n; ++i) cin >> p[i].a >> p[i].b;
  for(int i = n; i >= 1; --i) p[i] = p[i] - p[1];
  for(int i = 1; i <= n; ++i) g = Gcd(g, p[i]);
  for(int i = 1, l = g.norm(); i <= n; ++i) {
    Z cur = p[i] * g.conj(); 
    cur.a /= l, cur.b /= l;
    Xmax = max(Xmax, cur.a), Xmin = min(Xmin, cur.a);
    Ymax = max(Ymax, cur.b), Ymin = min(Ymin, cur.b);
  }
  Ans = max(Xmax - Xmin + 1, Ymax - Ymin + 1);
  Ans = Ans * Ans - n;
  printf("%lld\n", Ans);
  return 0;}
