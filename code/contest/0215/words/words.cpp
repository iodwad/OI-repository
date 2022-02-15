// by iodwad
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstring>
#include <functional>

using namespace std;

using i64 = long long;

const int MAXN = 1e5;
const int MAXM = 1e6;

int n, typ, tot, top;
int val[MAXN + 5];
string str[MAXN + 5];

namespace ACAM {
  int ptot;
  int sum[MAXM + 5], fail[MAXM + 5];
  int trans[MAXM + 5][26];
  int cnt, trash[MAXM + 5];
  vector<int> e[MAXM + 5];
  int newnode(int rt) {
    int o = cnt ? trash[cnt--] : ++ptot;
    sum[o] = 0;
    rt = rt ? rt : o;
    fail[o] = rt;
    fill(trans[o], trans[o] + 26, rt);
    e[o].clear();
    return o;
  }
  void retrieve(int o) {
    trash[++cnt] = o;
  }
  struct ACAM {
    int rt;
    vector<int> id, nd;
    ACAM() {}
    void clear() {
      for (int i : nd) retrieve(i);
      id.clear(); nd.clear();
      rt = 0;
    }
    // ACAM() { clear(); }
    void insert(int _id) {
      if (!rt) nd.push_back(rt = newnode(0));
      int len = str[_id].size(), o = rt;
      for (int i = 0; i < len; ++i) {
        int &to = trans[o][str[_id][i] - 'a'];
        if (to == rt) nd.push_back(to = newnode(rt));
        o = to;
      }
      sum[o] += val[_id];
      id.push_back(_id);
    }
    void build() {
      static queue<int> q; 
      for (int i = 0; i < 26; ++i) if (trans[rt][i] != rt) q.push(trans[rt][i]);
      while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < 26; ++i) {
          int &v = trans[u][i];
          if (v != rt) {
            fail[v] = trans[fail[u]][i];
            q.push(v);
          } else v = trans[fail[u]][i];
        }
      }
      for (int i : nd) if (i != rt) e[fail[i]].push_back(i);
      function<void(int)> dfs = [&](int x) {
        for (int to : e[x]) {
          sum[to] += sum[x];
          dfs(to);
        }
      };
      dfs(rt);
    }
    i64 query(string str) {
      i64 res = 0;
      int len = str.size();
      for (int o = rt, i = 0; i < len; ++i) {
        o = trans[o][str[i] - 'a'];
        res += sum[o];
      }
      return res;
    }
  } t[18];
}

using namespace ACAM;

void maintain() {
  // while (top > 0
  while (top > 0 && t[top].id.size() == t[top - 1].id.size()) {
    static vector<int> id; id.clear();
    for (int i : t[top].id) id.push_back(i);
    for (int i : t[top - 1].id) id.push_back(i);
    t[top - 1].clear(); t[top].clear();
    --top;
    for (int i : id) t[top].insert(i);
    t[top].build();
  }
}

i64 Query(string str) {
  i64 res = 0;
  for (int i = 0; i <= top; ++i) res += t[i].query(str);
  return res;
}

void decode(string &str, i64 v) {
  int len = str.size();
  for (int i = 0; i < len; ++i) {
    i64 c = str[i] - 'a';
    c = (c ^ v) % 26;
    str[i] = c + 'a';
  }
}

int main() {
  freopen("words.in", "r", stdin);
  freopen("words.out", "w", stdout);
  ios::sync_with_stdio(false); cin.tie(0);
  cin >> n >> typ;
  i64 lasans = 0;
  while (n--) {
    int op; cin >> op;
    if (op == 0) {
      ++tot;
      cin >> str[tot] >> val[tot];            
      if (typ) decode(str[tot], lasans);
      t[++top].insert(tot); t[top].build();
      maintain();
    } else {
      static string str; cin >> str;
      if (typ) decode(str, lasans);
      cout << (lasans = Query(str)) << "\n";
    }
  }
  return 0;
}

