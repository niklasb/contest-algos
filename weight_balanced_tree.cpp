// a BB[alpha] tree implementation that rebuilds entire subtrees if they get
// unbalanced
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

double alpha = 0.2;

struct Node {
  int val;
  int size;
  Node *l, *r;
  Node(int val_, int size_, Node* l_, Node* r_) : val(val_), size(size_), l(l_), r(r_) {}
};

#define NODESZ(n) ((n) ? (n)->size : 0)

void dfs(Node *n, vector<int>& res) {
  if (!n) return;
  dfs(n->l, res);
  res.push_back(n->val);
  dfs(n->r, res);
  delete n;
}

Node* build(vector<int>& vals, int l, int r) {
  if (r-l < 1) return 0;
  int m = (l+r)/2;
  return new Node(vals[m], r-l, build(vals, l, m), build(vals, m, r));
}

Node* maintain(Node* n) {
  if (min(NODESZ(n->l), NODESZ(n->r)) >= floor(alpha * n->size))
    return n;
  vector<int> vals;
  dfs(n, vals);
  return build(vals, 0, vals.size());
}

Node* insert(int x, Node* n) {
  if (!n) return new Node(x, 1, 0, 0);
  if (x <= n->val) n->l = insert(x, n->l);
  else             n->r = insert(x, n->r);
  return maintain(n);
}

int main() {
  Node* root = 0;
  for (int i = 1; i < 100000; ++i)
    root = insert(i, root);
}
