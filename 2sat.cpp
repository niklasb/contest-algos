const int maxn = 10010; // 2-sat: maxn = 2*maxvars
bool vis[maxn];
int col, color[maxn];
vector<int> adj[maxn], radj[maxn], bycol[maxn+1], st;

void init() { rep(i,0,maxn) adj[i].clear(), radj[i].clear(); }
void dfs(int u, vector<int> adj[]) {
  if (vis[u]) return;
  vis[u] = 1;
  foreach(it,adj[u]) dfs(*it, adj);
  if (col) {
    color[u] = col;
    bycol[col].pb(u);
  } else st.pb(u);
}
// this computes SCCs, outputs them in bycol, in topological order
void kosaraju(int n) { // n = number of nodes
  st.clear();
  clr(vis,0);
  col=0;
  rep(i,0,n) dfs(i,adj);
  clr(vis,0);
  clr(color,0);
  while(!st.empty()) {
    bycol[++col].clear();
    int x = st.back(); st.pop_back();
    if(color[x]) continue;
    dfs(x, radj);
  }
}
// 2-SAT
int assign[maxn]; // for 2-sat only
int var(int x) { return x<<1; }
bool solvable(int vars) {
  kosaraju(2*vars);
  rep(i,0,vars) if (color[var(i)] == color[1^var(i)]) return 0;
  return 1;
}
void assign_vars() {
  clr(assign,0);
  rep(c,1,col+1) {
    foreach(it,bycol[c]) {
      int v = *it >> 1;
      bool neg = *it&1;
      if (assign[v]) continue;
      assign[v] = neg?1:-1;
    }
  }
}
void add_impl(int v1, int v2) { adj[v1].push_back(v2); radj[v2].push_back(v1); }
void add_equiv(int v1, int v2) { add_impl(v1, v2); add_impl(v2, v1); }
void add_or(int v1, int v2) { add_impl(1^v1, v2); add_impl(1^v2, v1); }
void add_xor(int v1, int v2) { add_or(v1, v2); add_or(1^v1, 1^v2); }
void add_true(int v1) { add_impl(1^v1, v1); }
void add_and(int v1, int v2) { add_true(v1); add_true(v2); }

int parse(int i) {
  if (i>0) return var(i-1);
  else return 1^var(-i-1);
}
int main() {
  int n, m; cin >> n >> m; // m = number of clauses to follow
  while (m--) {
    string op; int x, y; cin >> op >> x >> y;
    x = parse(x);
    y = parse(y);
    if (op == "or") add_or(x, y);
    if (op == "and") add_and(x, y);
    if (op == "xor") add_xor(x, y);
    if (op == "imp") add_impl(x, y);
    if (op == "equiv") add_equiv(x, y);
  }
  if (!solvable(n)) {
    cout << "Impossible" << endl; return 0;
  }
  assign_vars();
  rep(i,0,n) cout << ((assign[i]>0)?(i+1):-i-1) << endl;
}
