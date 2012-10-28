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

#define NMAX 10000
static vector<pair<llong, int2> > edges;
static int path[NMAX+1];
static int root_rank[NMAX+1];

void init_set(int x) {
  path[x] = x;
  root_rank[x] = 0;
}

int find_set(int x) {
  if (path[x] == x) return x;
  return (path[x] = find_set(path[x]));
}

void union_sets(int x, int y) {
  path[find_set(x)] = find_set(y);
  /*
  // rank heuristic
  int r1 = find_set(x);
  int r2 = find_set(y);
  if (root_rank[r1] < root_rank[r2]) {
    path[r1] = r2;
  } else {
    path[r2] = r1;
    if (root_rank[r1] == root_rank[r2])
      root_rank[r1]++;
  }
  */
}

llong kruskal() {
  for (int i = 1; i <= n; i++)
    init_set(i);
  sort(edges.begin(), edges.end());
  llong res = 0;
  foreach(it, edges) {
    llong w = it->first;
    int a = it->second.first;
    int b = it->second.second;
    if (find_set(a) == find_set(b)) continue;
    res += w;
    union_sets(a, b);
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
    edges.push_back(make_pair(w, make_pair(a, b)));
  }
  cout << kruskal() << endl;
}
