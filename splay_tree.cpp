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
#include <iomanip>
#include <complex>
#include <valarray>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>

using namespace std;
using namespace std::tr1;

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
#define foreach(v,c) for(typeof((c).begin()) v=(c).begin(); v!=(c).end();++v)
#define rep(i,s,e) for (int i=(s);i<(e);++i)
#define pb push_back
#define mk make_pair
#define fst first
#define snd second
#define all(x) (x).begin(),(x).end()
#define clr(x,y) memset(x,y,sizeof x)
#define contains(x,y) (x).find(y)!=(x).end()

int dx[]={0,0,1,-1,1,-1,1,-1}, dy[]={-1,1,0,0,1,-1,-1,1};
const int mod = 1e9+7;

typedef int T;
T sentinel = -(1<<30); // must compare < to every value
struct Tree {
  int sz,cnt;
  T val; Tree *child[2];
  Tree(T val=sentinel, int cnt=0, Tree* l=0, Tree* r=0) : cnt(cnt), val(val) {
    child[0]=l; child[1]=r;maintain();
  }
  void maintain() { sz = cnt + (child[0]?child[0]->sz:0) + (child[1]?child[1]->sz:0); }
  void setchild(bool c, Tree* t) { child[c]=t; maintain(); }
};
void rotate(Tree*& t, bool c) {
  Tree *x = t->child[c];
  t->setchild(c, x->child[!c]);
  x->setchild(!c, t);
  t = x;
}
void splay(Tree*& t, T x) {
  Tree N(0,0,0), *lr[2]={&N,&N};
  for(;;) {
    bool c = x > t->val;
    if (t->val == x || !t->child[c]) break;
    if (!c && x < t->child[c]->val) rotate(t, c);
    if (c  && x > t->child[c]->val) rotate(t, c);
    if (!t->child[c]) break;
    lr[!c]->setchild(c, t); lr[!c] = t; t = t->child[c];
  }
  rep(i,0,2) {
    lr[i]->setchild(!i, t->child[i]);
    t->setchild(i, N.child[!i]);
  }
}
T walk(Tree* t, bool c) { return t->child[c] ? walk(t->child[c], c) : t->val; }
void splaypred(Tree*& t, T x) { // splay max y in the tree with y <= x
  splay(t,x);
  if (t->val > x) splay(t, walk(t->child[0], 1));
}
pair<Tree*,Tree*> split(Tree *t, T x) { // split into subtrees (<= i, >i)
  splaypred(t,x);
  Tree *r = t->child[1];
  t->setchild(1,0);
  return mk(t, r);
}
Tree* merge(Tree *a, Tree *b) { // precondition: max a < min b
  if (!a || !b) return a?a:b;
  splay(a, walk(a,1));
  splay(b, walk(b,0));
  a->setchild(1,b);
  return a;
}
void insert(Tree*& t, T x) {
  splay(t,x);
  if (t->val == x) t->cnt++, t->sz++;
  else {
    pair<Tree*,Tree*> lr = split(t, x);
    t = new Tree(x, 1, lr.first, lr.second);
  }
}
void remove(Tree*& t, T x) {
  splay(t, x);
  if (t->val != x) return;
  t->cnt--; t->sz--;
  if (!t->cnt) {
    Tree *old = t;
    t = merge(t->child[0], t->child[1]);
    delete old;
  }
}
T select(Tree* t, int k) { // ranks start at 0
  int l = t->child[0] ? t->child[0]->sz : 0;
  if (k < l) return select(t->child[0], k);
  if (k >= l + t->cnt) return select(t->child[1], k - l - t->cnt);
  return t->val;
}
T getrank(Tree*& t, T x) { // find min i with select(t,i) > x }
  splaypred(t,x);
  return (t->child[0] ? t->child[0]->sz : 0) + t->cnt*(t->val != x);
}

void print(Tree *t, int indent=0) {
  rep(i,0,indent) cout << " ";
  if (!t) {cout << "NIL:0:0" << endl;return;}
  //if (t->val==sentinel) {cout<<"-oo:0:0"<<endl;return;}
  cout << t->val << "/" << t->cnt << " "<< t->sz << endl;
  rep(i,0,2) print(t->child[i], indent+2);
  if(!indent) cout << "=========="<<endl;
}

Tree* fromAry(T ary[], int len) {
  if (len == 0) return 0;
  return new Tree(ary[0],0, fromAry(ary+1,len-1));
}

int main() {
#ifndef LOCAL
  //freopen("x.in", "r", stdin);
  //freopen("x.out", "w", stdout);
#endif
  ios::sync_with_stdio(0);
  cout << fixed << setprecision(16);
  Tree *t = new Tree();
  vector<int> objs;
  rep(i,0,100000) objs.pb(i);
  random_shuffle(all(objs));
  foreach(it,objs) insert(t,*it);
  random_shuffle(all(objs));
  foreach(it,objs) remove(t,*it);
  //vector<int> objs;
  //init();
  //rep(i,0,100000) objs.pb(i);
  //random_shuffle(all(objs));
  //foreach(it,objs) insert(root,*it);
  //random_shuffle(all(objs));
  //foreach(it,objs) remove(root,*it);
  //cout << select(t,100) << endl;
  return 0;
}
