// This is O(n*d) where d is the maximum vertex degree. We can also use DP on
// (x, from) instead in dfs1.
void maintain(int x, int exclude) {
  g[x] = 1;
  for (int y: adj[x]) {
    if (y == exclude) continue;
    g[x] += g[y];
  }
}
// build initial data structures with fixed root
void dfs1(int x, int from) {
  for (int y: adj[x]) if (y != from)
    dfs1(y, x);
  maintain(x, from);
}
// inspect data structures with x as root
void dfs2(int x, int from) {
  for (int y: adj[x]) if (y != from) {
    maintain(x, y);
    maintain(y, -1);
    dfs2(y, x);
  }
  maintain(x, from);
}
