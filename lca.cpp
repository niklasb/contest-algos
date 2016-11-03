const int N = 100100;
const int H = 17;  // height <= 2**H
int par[N][H+1], lvl[N];

void dfs(int x, int from) {  // from == x for root
  lvl[x] = from==x ? 0 : lvl[from] + 1;
  par[x][0] = from;
  for (int i = 1; i <= H; ++i)
    par[x][i] = par[par[x][i-1]][i-1];
}

// n log n space with "sparse table"
int lca(int x, int y) {
  if (lvl[x] < lvl[y])
    swap(x, y);
  for (int i = H; i >= 0; i--)
    if (lvl[x] - (1<<i) >= lvl[y])
      x = par[x][i];
  assert(lvl[x] == lvl[y]);
  if (x == y) return x;
  for (int i = H; i >= 0; i--)
    if (par[x][i] != par[y][i])
      x = par[x][i], y = par[y][i];
  assert(par[x][0] == par[y][0]);
  return par[x][0];
}
