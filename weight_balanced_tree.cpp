#include <bits/stdc++.h>
#include <cassert>
using namespace std;
typedef long long ll; typedef long double ld;
#define rep(i,s,e) for (int i=(s);i<(e);++i)
#define all(x) begin(x),end(x)
#define clr(x,y) memset(x,y,sizeof x)
#define contains(x,y) ((x).find(y)!=end(x))
#define pb push_back
#define Tu tuple
#define mk make_pair
#define mkt make_tuple
#define fst first
#define snd second
#define _1 get<0>
#define _2 get<1>
#define _3 get<2>
#define _4 get<3>
#define sz(x) ((int)(x).size())

// a BB[alpha] tree implementation that rebuilds entire subtrees if they get
// unbalanced
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
using namespace std;

double alpha = 0.25;

#define treesz(n) ((n) ? (n)->treesize : 0)
#define valsz(n) ((n) ? (n)->valsize : 0)
struct Node {
  int val, count, valsize, treesize;
  Node *l, *r;
  Node(int val, int count, Node* l, Node* r) : val(val), count(count), l(l), r(r) {
    maintain();
  }
  void maintain() {
    valsize = count + valsz(l) + valsz(r);
    treesize = 1 + treesz(l) + treesz(r);
  }
  bool balanced() {
    return min(treesz(l), treesz(r)) >= alpha * (treesz(l) + treesz(r)) - 1;
  }
};
vector<Node*> nodes;
void dfs(Node *n) {
  if (!n) return;
  dfs(n->l);
  nodes.pb(n);
  dfs(n->r);
}
Node* build(int l, int r) {
  if (r < l) return 0;
  int m = (l + r) / 2;
  Node* res = new Node(nodes[m]->val, nodes[m]->count, build(l, m - 1), build(m + 1, r));
  delete nodes[m];
  return res;
}
int tot = 0;
Node* maintain(Node* n) {
  n->maintain();
  if (n->balanced()) return n;
  nodes.clear();
  dfs(n);
  tot += nodes.size();
  n = build(0, nodes.size() - 1);
  assert(n->balanced());
  return n;
}
void insert(Node*& n, int x, bool rem=0) {
  if (!n && rem) return;
  else if (!n) n = new Node(x, 1, 0, 0);
  else if (x == n->val) n->count += rem ? -1 : 1, n->maintain();
  else {
    insert(x < n->val ? n->l : n->r, x, rem);
    n = maintain(n);
  }
}
void remove(Node*& n, int x) { return insert(n, x, 1); }

Node *root = 0;

// test helpers
int height(Node *n) {
  if (!n) return 0;
  return 1 + max(height(n->l), height(n->r));
}
bool is_consistent(Node *n) {
  if (!n) return 1;
  if (treesz(n->l) + treesz(n->r) + 1 != n->treesize) return 0;
  if (valsz(n->l) + valsz(n->r) + n->count != n->valsize) return 0;
  if (!n->balanced()) return 0;
  return is_consistent(n->l) && is_consistent(n->r);
}
void debug(Node* n, int ind = 0) {
  rep(_,0,ind) cout << "  ";
  if(!n) {cout << "NIL:0" << endl;return;}
  cout << n->val << ":" << n->count <<" " << valsz(n) <<" " << is_consistent(n) << endl;
  debug(n->l, ind + 1);
  debug(n->r, ind + 1);
}

int N = 1000000;
int main() {
  for (int i = 1; i <= N; ++i) insert(root, i);
  for (int i = 1; i <= N; ++i) insert(root, i);
  for (int i = 1; i <= N; ++i) remove(root, i);
  //debug(root);
  cout << "consistent: " << is_consistent(root) << endl;
  cout << "heigth: " << height(root) << endl;
  cout << "total rebuilt nodes: " << tot << endl;
  //vals.clear();
  //dfs(root);
  //sort(all(vals));
  //assert(vals.size() == N);
  //for (int i = 0; i < N; ++i) assert(vals[i] == mk(i + 1, 1));
}
