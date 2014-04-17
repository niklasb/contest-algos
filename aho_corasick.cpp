const int K = 10;
struct vertex {
  int next[K];
  bool leaf;
  int p;
  char pch;
  int link;
  int go[K];
};

vertex t[1200];
int root=1;
int top=2;

void init() {
  root = 1;
  t[root] = {0};
  top = 2;
}

void add_string (const string & s) {
  int v = 0;
  for (char c: s) {
    int a = c-'0';
    if (!t[v].next[a]) {
      t[top] = {0};
      t[top].p = v;
      t[top].pch = a;
      t[v].next[a] = top++;
    }
    v = t[v].next[a];
  }
  t[v].leaf = 1;
}

int go(int v, int c);

int get_link(int v) {
  if (!t[v].link) {
    if (v == 0 || t[v].p == 0)
      t[v].link = 0;
    else
      t[v].link = go(get_link (t[v].p), t[v].pch);
  }
  return t[v].link;
}

int go(int v, int c) {
  if (!t[v].go[c]) {
    if (t[v].next[c])
      t[v].go[c] = t[v].next[c];
    else
      t[v].go[c] = v==0 ? 0 : go(get_link(v), c);
  }
  return t[v].go[c];
}

// TODO this seems wrong. Maybe we shouldn't forget the intermediate links
bool is_accepting(int v) {
  return t[v].leaf || t[get_link(v)].leaf;
}
