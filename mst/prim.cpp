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
#define foreach(v,c) for (typeof((c).begin()) v = (c).begin(); v != (c).end(); ++v)

#define NMAX 10000
int n;

static vector<pair<int, llong> > adj[NMAX + 1];
static bool visited[NMAX + 1];
static int d[NMAX + 1];

llong prim(int start) {
  fill(d, d + n + 1, INT_MAX);

  priority_queue<pair<llong, int> > q;
  d[start] = 0;
  q.push(make_pair(0, start));
  llong res = 0;

  while (!q.empty()) {
    int dist = -q.top().first;
    int a = q.top().second;
    q.pop();
    if (dist > d[a]) continue;
    res += dist;
    visited[a] = true;

    foreach(it, adj[a]) {
      int b = it->first;
      llong w = it->second;
      if (!visited[b] && w < d[b]) {
        d[b] = w;
        q.push(make_pair(-w, b));
      }
    }
  }

  return res;
}

int main() {
  ios_base::sync_with_stdio(false);
  int m;
  scanf("%d %d", &n, &m);
  int a, b, w;
  for (int i = 0; i < m; i++) {
    scanf("%d %d %d", &a, &b, &w);
    adj[a].push_back(make_pair(b, w));
    adj[b].push_back(make_pair(a, w));
  }
  cout << prim(1) << endl;
}
