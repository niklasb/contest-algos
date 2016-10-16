const int K = 20;
struct vertex {
  vertex *next[K], *go[K], *link, *p;
  int pch;
  bool leaf;
  int is_accepting = -1;
};

vertex *create() {
  vertex *root = new vertex();
  root->link = root;
  return root;
}

void add_string (vertex *v, const vector<int>& s) {
  for (int a: s) {
    if (!v->next[a]) {
      vertex *w = new vertex();
      w->p = v;
      w->pch = a;
      v->next[a] = w;
    }
    v = v->next[a];
  }
  v->leaf = 1;
}

vertex* go(vertex* v, int c);

vertex* get_link(vertex *v) {
  if (!v->link)
    v->link = v->p->p ? go(get_link(v->p), v->pch) : v->p;
  return v->link;
}

vertex* go(vertex* v, int c) {
  if (!v->go[c]) {
    if (v->next[c])
      v->go[c] = v->next[c];
    else
      v->go[c] = v->p ? go(get_link(v), c) : v;
  }
  return v->go[c];
}

bool is_accepting(vertex *v) {
  if (v->is_acceping == -1)
    v->is_accepting = get_link(v) == v ? false : (v->leaf || is_accepting(get_link(v)));
  return v->is_accepting;
}
