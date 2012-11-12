#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <queue>
using namespace std;

const int MAXVARS = 10000*3;
const int MAXN = MAXVARS*2 + 10;
bool visit[MAXN];
vector<int> adj[MAXN], radj[MAXN], dag[MAXN];
int n, cnt, id[MAXN], order[MAXN], color[MAXN], ind[MAXN];
queue<int> q1, q2;

int var(int v) { return v*2; }
int neg(int v) { return (v%2==0)?(v-1):(v+1); }

void dfs(int u) {
  visit[u] = true;
  int i, len = adj[u].size();
  for (i = 0; i < len; ++i)
    if (!visit[adj[u][i]]) dfs(adj[u][i]);
  order[cnt++] = u;
}

bool rdfs(int u) {
  visit[u] = true; id[u] = cnt;
  if (id[u] == id[neg(u)]) return false;
  int i, len = radj[u].size();
  for (i = 0; i < len; i++)
    if (!visit[radj[u][i]] && !rdfs(radj[u][i])) return false;
  return true;
}

void init() {
  memset(visit, 0, sizeof visit);
  memset(id, 0, sizeof(id));
  memset(ind, 0, sizeof ind );
  memset(color, 0, sizeof color);
  while (!q1.empty()) q1.pop(); while (!q2.empty()) q2.pop();
  for (int i = 0; i < MAXN; ++i)
    adj[i].clear(), radj[i].clear(), dag[i].clear();
}

bool kosaraju() {
  int i;
  for (cnt = 0, i = 1; i <= 2*n; i++) if (!visit[i]) dfs(i);
  memset(visit, 0, sizeof visit);
  for (cnt = 0, i = 2*n-1; i >= 0; i--)
    if (!visit[order[i]]) {
      cnt++;
      if (!rdfs(order[i])) return false;
    }
  return true;
}

void topsort() {
  for (int i = 1; i <= 2*n; i++){
    int len = adj[i].size();
    for (int j = 0; j < len; j++)
      if (id[i] != id[adj[i][j]]) {
        dag[id[adj[i][j]]].push_back(id[i]);
        ind[id[i]]++;
      }
  }
  for (int i = 1; i <= cnt; i++) if (ind[i] == 0) q1.push(i);

  int i,j,len,now,p,pid;
  while (!q1.empty()) {
    now = q1.front(); q1.pop();
    if (color[now] != 0) continue ;
    color[now] = 'R'; ind[now] = -1;
    for (i=1; i <= 2*n; i++) {
      if (id[i] == now) {
        p = (i%2) ? i+1 : i-1; pid=id[p]; q2.push(pid);
        while (!q2.empty()) {
          pid = q2.front(); q2.pop();
          if (color[pid] == 'B') continue ;
          color[pid] = 'B';
          int len = dag[pid].size();
          for(j = 0; j < len; j++) q2.push(dag[pid][j]);
        }
      }
    }
    len = dag[now].size();
    for (i = 0; i < len; i++) {
      ind[dag[now][i]]--;
      if (ind[dag[now][i]] == 0) q1.push(dag[now][i]);
    }
  }
}

void add_impl(int v1, int v2) { adj[v1].push_back(v2); radj[v2].push_back(v1); }
void add_equiv(int v1, int v2) { add_impl(v1, v2); add_impl(v2, v1); }
void add_or(int v1, int v2) { add_impl(neg(v1), v2); add_impl(neg(v2), v1); }
void add_xor(int v1, int v2) { add_or(v1, v2); add_or(neg(v1), neg(v2)); }
void add_true(int v1) { add_impl(neg(v1), v1); }
void add_and(int v1, int v2) { add_true(v1); add_true(v2); }

// n = number of variables, variables start at 1
// clauses are of the form "[op] 1 -2" (-2 means negation of variable 2)
int main() {
  int m; cin >> n >> m; // m = number of clauses to follow
  init();
  while (m--) {
    string op; int x, y; cin >> op >> x >> y;
    x = (x > 0) ? var(x) : neg(var(-x));
    y = (y > 0) ? var(y) : neg(var(-y));
    if (op == "or") add_or(x, y);
    if (op == "and") add_and(x, y);
    if (op == "xor") add_xor(x, y);
    if (op == "imp") add_impl(x, y);
    if (op == "equiv") add_equiv(x, y);
  }
  if (!kosaraju()) {
    cout << "Impossible" << endl; return 0;
  }
  topsort();
  for (int i = 1; i <= n; i++){
    if (color[id[var(i)]] == 'R') cout << i << endl;
    else cout << -i << endl;
  }
}
