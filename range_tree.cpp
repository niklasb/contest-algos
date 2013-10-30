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
#define endl "\n"

int dx[]={0,0,1,-1,1,-1,1,-1}, dy[]={-1,1,0,0,1,-1,-1,1};
const int mod = 1e9+7,maxn=100100;

// space O(n * log n), build O(n * log n), query O(log^2 n + k), k = output
typedef int XX;
typedef int YY;
typedef pair<XX,YY> Point;
typedef pair<XX, vector<Point> > Node;
const XX inf = 1<<30;

int N;
Node tree[maxn];

int left(int i) { return (i<<1) + 1; }
int right(int i) { return (i<<1) + 2; }
bool leaf(int i) { return i >= N-1; }
bool cmpY(const Point& a, const Point& b) { return a.snd < b.snd; }

void build_rec(vector<Node>& src, int l, int r, int i) {
  int m = (l+r)/2;
  tree[i] = mk(src[m].fst, vector<Point>());
  vector<Point>& points = tree[i].snd;
  if (l == r) {
    points = src[l].snd;
  } else {
    build_rec(src, l, m, left(i));
    build_rec(src, m+1, r, right(i));
    merge(tree[left(i)].snd.begin(), tree[left(i)].snd.end(),
          tree[right(i)].snd.begin(), tree[right(i)].snd.end(),
          back_inserter(points), cmpY);
  }
}

int cnt;
void report(int n, int y1, int y2) {
  vector<Point>& p = tree[n].snd;
  vector<Point>::iterator
      l = lower_bound(all(p), Point(0, y1), cmpY),
      r = upper_bound(all(p), Point(0, y2), cmpY);
  //for (; l != r; ++l) { cout << p.fst<<" "<<p.snd << endl; }
  cnt += r - l;
}

void walk(int n, int x, int y1, int y2, bool r) {
  while (!leaf(n)) {
    if (x <= tree[n].fst) {
      if (r) report(right(n), y1, y2);
      n = left(n);
    } else {
      if (!r) report(left(n), y1, y2);
      n = right(n);
    }
  }
  if ((r && x <= tree[n].fst) || (!r && x >= tree[n].fst))
    report(n, y1, y2);
}

void query(int x1, int x2, int y1, int y2) {
  int n = 0;
  while (!leaf(n)) { // find split point
    if      (x1 <= tree[n].fst && x2 <= tree[n].fst) n = left(n);
    else if (x1 >  tree[n].fst && x2 >  tree[n].fst) n = right(n);
    else break;
  }
  if (!leaf(n)) {
    walk(left(n), x1, y1, y2, true);
    walk(right(n), x2, y1, y2, false);
  } else if (x1 <= tree[n].fst && tree[n].fst <= x2) {
    report(n, y1, y2);
  }
}

void build(vector<Point>& src) {
  assert(src.size() > 0);
  sort(all(src));
  vector<Node> items;
  items.pb(mk(src[0].fst, vector<Point>()));
  rep(i,0,src.size()) {
    if (i > 0 && src[i-1].fst != src[i].fst)
      items.pb(mk(src[i].fst, vector<Point>()));
    items.back().snd.pb(src[i]);
  }
  N = 1;
  while (N < items.size()) N <<= 1;
  while (items.size() < N) items.pb(mk(inf, vector<Point>()));
  build_rec(items, 0, N-1, 0);
}

void debug(int i=0, int indent=0) {
  cout << string(indent, ' ');
  cout << tree[i].fst << ": ";
  foreach(it,tree[i].snd) cout<< it->fst << "," << it->snd << "  ";
  cout << endl;
  if (!leaf(i))
    debug(left(i), indent + 2), debug(right(i), indent + 2);
}

int n, q;
int main() {
  ios::sync_with_stdio(0);
  cout << fixed << setprecision(16);
  vector<Point> ps;
  cin>>n>>q;
  rep(i,0,n) {
    Point p; cin >> p.fst >> p.snd;
    ps.pb(p);
  }
  build(ps);
  //debug();
  rep(i,0,q) {
    int x1, x2, y1, y2;
    cin >> x1 >> x2 >> y1 >> y2;
    cnt=0;
    query(x1,x2,y1,y2);
    cout << cnt << endl;
  }
}
