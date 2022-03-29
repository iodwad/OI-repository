// by iodwad
#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;

using i64 = long long;

const int MOD = 1e9 + 7;
const int MAXN = 1e6;

int add(int x, int y) { return x + y >= MOD ? x + y - MOD : x + y; }

int n, f[MAXN + 5], g[MAXN + 5];
char instr[MAXN + 5], str[MAXN + 5];

namespace PAM {
  int Size, Last;
  int trans[MAXN + 5][26], fail[MAXN + 5], len[MAXN + 5], slink[MAXN + 5], diff[MAXN + 5];
  void init() {
    // Size = Last = 
    fail[1] = fail[2] = 1; 
    len[1] = -1; len[2] = 0;
    for (int i = 1; i < MAXN + 5; ++i) 
      for (int j = 0; j < 26; ++j)
        trans[i][j] = 2;
    slink[2] = 1;
    Size = Last = 2;
  }
  int get_fail(int p, int x) {
    while (str[p - len[x] - 1] != str[p]) x = fail[x];
    return x;
  }
  void extend(int x) {
    int c = str[x] - 'a';
    int p = get_fail(x, Last);
    if (trans[p][c] == 2) {
      int now = ++Size;
      fail[now] = trans[get_fail(x, fail[p])][c];
      len[now] = len[p] + 2;
      diff[now] = len[now] - len[fail[now]];
      slink[now] = diff[now] == diff[fail[now]] ? slink[fail[now]] : fail[now];
      trans[p][c] = now;
    }
    Last = trans[p][c];
  }
}

using namespace PAM;

int main() {
  freopen("code.in", "r", stdin);
  freopen("code.out", "w", stdout);
  scanf("%s", instr + 1); n = strlen(instr + 1);
  for (int l = 1, r = n, i = 1; i <= n; ++i) {
    if (i & 1) str[i] = instr[l++];
    else str[i] = instr[r--];
  }
  f[0] = 1;
  init();
  for (int i = 1; i <= n; ++i) {
    extend(i);
    int now = Last;
    for (; now != 1; now = slink[now]) {
      g[now] = f[i - len[slink[now]] - diff[now]];
      if (slink[now] != fail[now]) g[now] = add(g[now], g[fail[now]]);
      if (~i & 1) f[i] = add(f[i], g[now]);
    }
  }
  printf("%d\n", f[n]);
  return 0;
}
