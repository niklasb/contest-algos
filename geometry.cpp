#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <valarray>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <iomanip>
#include <complex>
#include <cassert>

using namespace std;
using namespace std::tr1;

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
#define foreach(v,c) for(typeof((c).begin()) v=(c).begin(); v!=(c).end();++v)
#define rep(i,s,e) for (int i=(s);i<(e);++i)
#define pb push_back
#define mk make_pair
#define X first
#define Y second
#define all(x) (x).begin(),(x).end()
#define clr(x,y) memset(x,y,sizeof x)
#define contains(x,y) (x).find(y)!=(x).end()
#define endl "\n"

typedef long double D;
const D eps=1e-12, inf=1e15, pi=acos(-1), e=exp(1.);

D sq(D x) { return x*x; }
D rem(D x, D y) { return fmod(fmod(x,y)+y,y); }
D rtod(D rad) { return rad*180/pi; }
D dtor(D deg) { return deg*pi/180; }
int sgn(D x) { return (x > eps) - (x < -eps); }
D fixzero(D x, int d) { return (x>0 || x<=-5/pow(10,d+1)) ? x:0; }

typedef complex<D> P;
namespace std {
  bool operator<(const P& a, const P& b) {
    return mk(real(a), imag(a)) < mk(real(b), imag(b));
  }
}

D cross(P a, P b)    { return imag(conj(a) * b); }
D cross(P a, P b, P c) { return cross(b-a, c-a); }
D dot(P a, P b)      { return real(conj(a) * b); }
P scale(P a, D len)  { return a * (len/abs(a)); }
P rotate(P p, D ang) { return p * polar(D(1), ang); }
D angle(P a, P b)    { return arg(b) - arg(a); }

int ccw(P a, P b, P c) {
  b -= a; c -= a;
  if (cross(b, c) > eps)  return +1;  // counter clockwise
  if (cross(b, c) < -eps) return -1;  // clockwise
  if (dot(b, c) < 0)      return +2;  // c--a--b on line
  if (norm(b) < norm(c))  return -2;  // a--b--c on line
  return 0;
}

typedef vector<P> L;
typedef vector<P> G;
G dummy;
L line(P a, P b) {
  L res; res.pb(a); res.pb(b); return res;
}
P dir(const L& l) { return l[1]-l[0]; }

D project(P e, P x) { return dot(e,x) / norm(e); }
P pedal(const L& l, P p) { return l[1] + dir(l) * project(dir(l), p-l[1]); }
int intersectLL(const L &l, const L &m) {
  if (abs(cross(l[1]-l[0], m[1]-m[0])) > eps) return 1;  // non-parallel
  if (abs(cross(l[1]-l[0], m[0]-l[0])) < eps) return -1; // same line
  return 0;
}
bool intersectLS(const L& l, const L& s) {
  return cross(dir(l), s[0]-l[0])*       // s[0] is left of l
         cross(dir(l), s[1]-l[0]) < eps; // s[1] is right of l
}
bool intersectLP(const L& l, const P& p) {
  return abs(cross(l[1]-p, l[0]-p)) < eps;
}
bool intersectSS(const L& s, const L& t) {
  return sgn(ccw(s[0],s[1],t[0]) * ccw(s[0],s[1],t[1])) <= 0 &&
         sgn(ccw(t[0],t[1],s[0]) * ccw(t[0],t[1],s[1])) <= 0;
}
bool intersectSP(const L& s, const P& p) {
  return abs(s[0]-p)+abs(s[1]-p)-abs(s[1]-s[0]) < eps; // triangle inequality
}
P reflection(const L& l, P p) {
  return p + P(2,0) * (pedal(l, p) - p);
}
D distanceLP(const L& l, P p) {
  return abs(p - pedal(l, p));
}
D distanceLL(const L& l, const L& m) {
  return intersectLL(l, m) ? 0 : distanceLP(l, m[0]);
}
D distanceLS(const L& l, const L& s) {
  if (intersectLS(l, s)) return 0;
  return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
}
D distanceSP(const L& s, P p) {
  P r = pedal(s, p);
  if (intersectSP(s, r)) return abs(r - p);
  return min(abs(s[0] - p), abs(s[1] - p));
}
D distanceSS(const L& s, const L& t) {
  if (intersectSS(s, t)) return 0;
  return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])),
             min(distanceSP(t, s[0]), distanceSP(t, s[1])));
}
P crosspoint(const L& l, const L& m) { // return intersection point
  D A = cross(dir(l), dir(m));
  D B = cross(dir(l), l[1] - m[0]);
  return m[0] + B / A * dir(m);
}
L bisector(P a, P b) {
  P A = (a+b)*P(0.5,0);
  return line(A, A+(b-a)*P(0,1));
}

#define next(g,i) g[(i+1)%g.size()]
#define prev(g,i) g[(i+g.size()-1)%g.size()]
L edge(const G& g, int i) { return line(g[i], next(g,i)); }
D area(const G& g) {
  D A = 0;
  rep(i,0,g.size())
    A += cross(g[i], next(g,i));
  return abs(A/2);
}

// intersect with half-plane left of l[0] -> l[1]
G convex_cut(const G& g, const L& l) {
  G Q;
  rep(i,0,g.size()) {
    P A = g[i], B = next(g,i);
    if (ccw(l[0], l[1], A) != -1) Q.pb(A);
    if (ccw(l[0], l[1], A)*ccw(l[0], l[1], B) < 0)
      Q.pb(crosspoint(line(A, B), l));
  }
  return Q;
}
bool convex_contain(const G& g, P p) { // check if point is inside convex polygon
  rep(i,0,g.size())
    if (ccw(g[i], next(g, i), p) == -1) return 0;
  return 1;
}
G convex_intersect(G a, G b) { // intersect two convex polygons
  rep(i,0,b.size())
    a = convex_cut(a, edge(b, i));
  return a;
}
void triangulate(G g, vector<G>& res) { // triangulate a simple polygon
  while (g.size() > 3) {
    bool found = 0;
    rep(i,0,g.size()) {
      if (ccw(prev(g,i), g[i], next(g,i)) != +1) continue;
      G tri;
      tri.pb(prev(g,i));
      tri.pb(g[i]);
      tri.pb(next(g,i));
      bool valid = 1;
      rep(j,0,g.size()) {
        if ((j+1)%g.size() == i || j == i || j == (i+1)%g.size()) continue;
        if (convex_contain(tri, g[j])) {
          valid = 0;
          break;
        }
      }
      if (!valid) continue;
      res.pb(tri);
      g.erase(g.begin() + i);
      found = 1; break;
    }
    assert(found);
  }
  res.pb(g);
}
void graham_step(G& a, G& st, int i, int bot) {
  while (st.size()>bot && sgn(cross(*(st.end()-2), st.back(), a[i]))<=0)
    st.pop_back();
  st.pb(a[i]);
}
bool cmpY(P a, P b) { return mk(imag(a),real(a)) < mk(imag(b),real(b)); }
G graham_scan(const G& points) { // will return points in ccw order
  // special case: all points coincide, algo might return point twice
  G a = points; sort(all(a),cmpY);
  int n = a.size();
  if (n<=1) return a;
  G st; st.pb(a[0]); st.pb(a[1]);
  for (int i = 2; i < n; i++) graham_step(a,st,i,1);
  int mid = st.size();
  for (int i = n - 2; i >= 0; i--) graham_step(a,st,i,mid);
  while (st.size() > 1 && !sgn(abs(st.back() - st.front()))) st.pop_back();
  return st;
}
G voronoi_cell(G g, const vector<P> &v, int s) {
  rep(i,0,v.size())
    if (i!=s)
      g = convex_cut(g, bisector(v[s], v[i]));
  return g;
}
const int ray_iters = 20;
bool simple_contain(const G& g, P p) { // check if point is inside simple polygon
  int yes = 0;
  rep(_,0,ray_iters) {
    D angle = 2*pi * (D)rand() / RAND_MAX;
    P dir = rotate(P(inf,inf), angle);
    L s = line(p, p + dir);
    int cnt = 0;
    rep(i,0,g.size()) {
      if (intersectSS(edge(g, i), s)) cnt++;
    }
    yes += cnt%2;
  }
  return yes > ray_iters/2;
}
bool intersectGG(const G& g1, const G& g2) {
  if (convex_contain(g1, g2[0])) return 1;
  if (convex_contain(g2, g1[0])) return 1;
  rep(i,0,g1.size()) rep(j,0,g2.size()) {
    if (intersectSS(edge(g1, i), edge(g2, j))) return 1;
  }
  return 0;
}
D distanceGP(const G& g, P p) {
  if (convex_contain(g, p)) return 0;
  D res = inf;
  rep(i,0,g.size())
    res = min(res, distanceSP(edge(g, i), p));
  return res;
}
P centroid(const G& v) {
  D S = 0;
  P res;
  rep(i,0,v.size()) {
    D tmp = cross(v[i], next(v,i));
    S += tmp;
    res += (v[i] + next(v,i)) * tmp;
  }
  S /= 2;
  res /= 6*S;
  return res;
}

struct C {
  P p; D r;
  C(P p, D r) : p(p),r(r) {}
  C(){}
};
// intersect circle with line through (c.p + v * dst/abs(v)) "orthogonal" to the circle
// dst can be negative
G intersectCL2(const C& c, D dst, P v) {
  G res;
  P mid = c.p + v * (dst/abs(v));
  if (sgn(abs(dst)-c.r) == 0) { res.pb(mid); return res; }
  D h = sqrt(sq(c.r) - sq(dst));
  P hi = scale(v * P(0,1), h);
  res.pb(mid + hi); res.pb(mid - hi);
  return res;
}
G intersectCL(const C& c, const L& l) {
  if (intersectLP(l, c.p)) {
    P h = scale(dir(l), c.r);
    G res; res.pb(c.p + h); res.pb(c.p - h); return res;
  }
  P v = pedal(l, c.p) - c.p;
  return intersectCL2(c, abs(v), v);
}
G intersectCS(const C& c, const L& s) {
  G res1 = intersectCL(c,s), res2;
  for(auto it: res1) if (intersectSP(s, it)) res2.pb(it);
  return res2;
}
int intersectCC(const C& a, const C& b, G& res=dummy) {
  D sum = a.r + b.r, diff = abs(a.r - b.r), dst = abs(a.p - b.p);
  if (dst > sum + eps || dst < diff - eps) return 0;
  if (max(dst, diff) < eps) { // same circle
    if (a.r < eps) { res.pb(a.p); return 1; } // degenerate
    return -1; // infinitely many
  }
  D p = (sq(a.r) - sq(b.r) + sq(dst))/(2*dst);
  P ab = b.p - a.p;
  res = intersectCL2(a, p, ab);
  return res.size();
}
typedef valarray<D> P3;
P3 p3(D x=0, D y=0, D z=0) {
  P3 res(3);
  res[0]=x;res[1]=y;res[2]=z;
  return res;
}
ostream& operator<<(ostream& out, const P3& x) {
  return out << "(" << x[0]<<","<<x[1]<<","<<x[2]<<")";
}
P3 cross(const P3& a, const P3& b) {
  P3 res;
  rep(i,0,3) res[i]=a[(i+1)%3]*b[(i+2)%3]-a[(i+2)%3]*b[(i+1)%3];
  return res;
}
D dot(const P3& a, const P3& b) {
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
D norm(const P3& x) { return dot(x,x); }
D abs(const P3& x) { return sqrt(norm(x)); }
D project(const P3& e, const P3& x) { return dot(e,x) / norm(e); }
P project_plane(const P3& v, P3 w, const P3& p) {
  w -= project(v,w)*v;
  return P(dot(p,v)/abs(v), dot(p,w)/abs(w));
}

template <typename T, int N> struct Matrix {
  T data[N][N];
  Matrix<T,N>() { clr(data,0); }
  Matrix<T,N> operator+(const Matrix<T,N>& other) const {
    Matrix res; rep(i,0,N) rep(j,0,N) res[i][j] = data[i][j] + other[i][j]; return res;
  }
  Matrix<T,N> operator*(const Matrix<T,N>& other) const {
    Matrix res; rep(i,0,N) rep(k,0,N) rep(j,0,N) res[i][j] += data[i][k] * other[k][j]; return res;
  }
  Matrix<T,N> transpose() const {
    Matrix res; rep(i,0,N) rep(j,0,N) res[i][j] = data[j][i]; return res;
  }
  void multiply_vector(const T v[N], T result[N]) {
    rep(i,0,N) result[i] = 0;
    rep(i,0,N) rep(j,0,N) result[i] += data[i][j] * v[j];
  }
  const T* operator[](int i) const { return data[i]; }
  T* operator[](int i) { return data[i]; }
};
template <typename T, int N>
static Matrix<T,N> id() { Matrix<T,N> m; rep(i,0,N) m[i][i] = 1; return m; }
template <typename T>
T pow(const T& x, ll e) {
  if (!e) return id<T>();
  if (e & 1) return x*pow(x,e-1);
  T res = pow(x,e/2);
  return res*res;
}
template <typename T, int N> ostream& operator<<(ostream& out, Matrix<T,N> mat) {
  rep(i,0,N) { rep(j,0,N) out << mat[i][j] << " "; cout << endl; } return out;
} // creates a rotation matrix around axis x (must be normalized). Rotation is
// counter-clockwise if you look in the inverse direction of x onto the origin
template<typename M> void create_rot_matrix(M& m, double x[3], double a) {
  rep(i,0,3) rep(j,0,3) {
    m[i][j] = x[i]*x[j]*(1-cos(a));
    if (i == j) m[i][j] += cos(a);
    else m[i][j] += x[(6-i-j)%3] * ((i == (2+j) % 3) ? -1 : 1) * sin(a);
  }
}

int main() {
  ios::sync_with_stdio(0);
  cout << fixed << setprecision(3);
  D x1,y1,x2,y2,r1,r2;
  vector<P> ip;
  while (cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2) {
    ip.clear();
    int res = intersectCC(C(P(x1,y1),r1), C(P(x2,y2),r2), ip);
    if (res==0) cout << "NO INTERSECTION";
    else if (res==-1) cout << "THE CIRCLES ARE THE SAME";
    sort(all(ip));
    rep(i,0,ip.size()) {
      cout << "(" << fixzero(real(ip[i]),3) << "," << fixzero(imag(ip[i]),3) << ")";
      //cout << "(" << real(ip[i]) << "," << imag(ip[i]) << ")";
    }
    cout << endl;
  }
}
