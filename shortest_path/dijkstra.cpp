#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>

using namespace std;

typedef long long llong;
typedef pair<int, int> int2;
#define foreach(v,c) for (typeof((c).begin()) v = (c).begin(); v != (c).end(); ++v)

#define NM 10000
int n;
// first = other node, second = edge weight
vector<int2> adj[NM+1];
int d[NM+1];

llong dijkstra(int a, int b) {
  fill(d, d + n + 1, INT_MAX);

  priority_queue<pair<llong, int> > q;
  q.push(make_pair(0, a));
  d[a] = 0;

  while (!q.empty()) {
    int x = q.top().second;
    int dist = -q.top().first;
    q.pop();
    if (dist > d[x]) continue;
    if (x == b) break;
    foreach(it, adj[x]) {
      int v = it->first;
      llong w = it->second + d[x];
      if (d[v] > w) {
        d[v] = w;
        q.push(make_pair(-d[v], v));
      }
    }
  }
  return d[b];
}
