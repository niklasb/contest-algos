const int MAXN = 500, MAXC = 1<<29;
struct edge { int dest, cap, cost, rev; };
vector<edge> adj[MAXN];
int dis[MAXN], source, target, iter, cap, cost;
edge* pre[MAXN];

void addedge(int x, int y, int cap, int cost) {
  adj[x].push_back(edge {y, cap, cost, (int)adj[y].size()});
  adj[y].push_back(edge {x, 0, -cost, (int)adj[x].size() - 1});
}

bool spfa() { // optimization: use dijkstra here and do Johnson reweighting before
  memset(dis, 0x3f, sizeof dis);
  queue<int> q;
  pre[source] = pre[target] = 0;
  dis[source] = 0;
  q.emplace(source);
  while (!q.empty()) {
    int x = q.front(), d = dis[x];
    q.pop();
    for (auto& e : adj[x]) {
      int y = e.dest, w = d + e.cost;
      if (e.cap < cap || dis[y] <= w) continue;
      dis[y] = w;
      pre[y] = &e;
      q.push(y);  // optimization: only push if not in queue yet
    }
  }
  edge* e = pre[target];
  if (!e) return 0;  // min-cost only: return also if dis[target] >= 0
  while (e) {
    edge& rev = adj[e->dest][e->rev];
    e->cap -= cap;
    rev.cap += cap;
    cost += cap * e->cost;
    e = pre[rev.dest];
  }
  return 1;
}

pair<int,int> mincostflow(int S, int T) {
  cap = MAXC, source = S, target = T, cost = 0;
  int flow = 0;
  while(cap) {
    while(spfa()) flow += cap;
    cap /= 2;
  }
  return {flow, cost};
}
