#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>

using namespace std;
using namespace std::tr1;

typedef long long llong;
typedef pair<int, int> int2;
typedef pair<llong, llong> llong2;
#define foreach(v,c) for (typeof((c).begin()) v = (c).begin(); v != (c).end(); ++v)
#define rep(i,n) for (int i = 0; i < (n); ++i)

struct MinCostFlow {
  static const int maxn = 300;
  static const int maxm = 4000;
  typedef int Captype;
  typedef int Valtype;
  static const Captype flowlimit = 100000;
  struct edge {
    int node, next; Captype flow; Valtype value;
  } edges[maxm<<1];
  int graph[maxn], queue[maxn], pre[maxn], con[maxn], n, m, source, target, top;
  bool inq[maxn];
  Captype maxflow;
  Valtype mincost, dis[maxn];
  MinCostFlow() { memset(graph, -1, sizeof(graph)); top=0;}
  inline int inverse(int x) { return 1 + ((x>>1)<<2)-x; }
  inline void addedge(int u, int v, Captype c, Valtype w) {
    if (c == 0) return;
    //cout << u << " -> " << v << ": " << c << "/" << w << endl;
    edges[top].value = w; edges[top].flow = c; edges[top].node = v;
    edges[top].next = graph[u]; graph[u] = top++;
    edges[top].value = -w; edges[top].flow = 0; edges[top].node = u;
    edges[top].next = graph[v]; graph[v] = top++;
  }
  bool SPFA() {
    int point, nod, now, head = 0, tail = 1;
    memset(pre, -1, sizeof pre);
    memset(inq, 0, sizeof inq);
    memset(dis, 0x7f, sizeof dis);
    dis[source] = 0; queue[0] = source; pre[source] = source; inq[source] = true;
    while (head != tail) {
      now = queue[head++]; point = graph[now]; inq[now] = false; head %= maxn;
      while (point != -1) {
        nod = edges[point].node;
        if (edges[point].flow > 0 && dis[nod]>dis[now]+edges[point].value) {
          dis[nod] = dis[now] + edges[point].value;
          pre[nod] = now;
          con[nod] = point;
          if (!inq[nod]) {
            inq[nod] = true;
            queue[tail++] = nod;
            tail %= maxn;
          }
        }
        point = edges[point].next;
      }
    }
    return pre[target]!=-1;
  }
  void extend() {
    Captype w = flowlimit;
    for (int u = target; pre[u] != u; u = pre[u])
      w = min(w, edges[con[u]].flow);
    maxflow += w;
    mincost += dis[target]*w;
    for (int u = target; pre[u] != u; u = pre[u]) {
      edges[con[u]].flow -= w;
      edges[inverse(con[u])].flow += w;
    }
  }
  void mincostflow() {
    maxflow = 0; mincost = 0;
    while (SPFA()) extend();
  }
};

void count_chars(string& x, int tc[26]) {
  memset(tc, 0, 26 * (sizeof *tc));
  foreach(c, x) tc[(int)(*c - 'a')]++;
}

int tc[26];
int main() {
  ios_base::sync_with_stdio(false);
  MinCostFlow* f = new MinCostFlow();
  string t; cin >> t;
  count_chars(t, tc);

  f->source = 50;
  f->target = 51;
  for (int i = 0; i < 26; ++i)
    f->addedge(i, f->target, tc[i], 0);

  int n; cin >> n;
  for (int i = 0; i < n; ++i) {
    int a;
    string s; cin >> s >> a;
    count_chars(s, tc);
    f->addedge(f->source, 100 + i, a, i+1);
    for (int c = 0; c < 26; ++c)
      f->addedge(100 + i, c, tc[c], 0);
  }
  f->mincostflow();
  if (f->maxflow == t.size())
    cout << f->mincost << endl;
  else
    cout << -1 << endl;
}
