#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#define INF 2000000000L

typedef int X; typedef int Y;
struct Point {
  X x; Y y;
  Point() :x(0),y(0){};
  Point(X _x, Y _y) :x(_x),y(_y){};
};

bool cmpY(const Point& a, const Point& b) { return a.y < b.y; }
bool cmpX(const Point& a, const Point& b) { return a.x < b.x; }

struct Subtree {
  vector<Point> points;
  // augmentation hooks
  void update(Subtree& left, Subtree& right) { }
  void update_leaf(Point& p) { }
};

const int maxn = 100000;
const int size = maxn*4 + 10;

X xs[size];
bool leaf[size];
Subtree subtrees[size];
Point input[maxn];

int left(int i) { return i * 2 + 1; }
int right(int i) { return i * 2 + 2; }
int parent(int i) { return (i-1)/2; }

void callback(Point& p) {
  cout << p.x << " " << p.y << endl;
}

// input must be sorted by the X coordinate
// X values must be distinct!
void build(Point input[], int start, int end, int pos) {
  int m = (end + start - 1)/2;
  xs[pos] = input[m].x;
  subtrees[pos].points.clear();
  if ((leaf[pos] = end - start == 1)) {
    subtrees[pos].points.push_back(input[m]);
    subtrees[pos].update_leaf(input[m]); // only necessary if Subtree class was augmented
  } else {
    build(input, start, m+1, left(pos));
    build(input, m+1, end, right(pos));
    Subtree& l = subtrees[left(pos)], r = subtrees[right(pos)];
    merge(l.points.begin(), l.points.end(), r.points.begin(), r.points.end(),
          back_inserter(subtrees[pos].points), cmpY);
    subtrees[pos].update(l, r); // only for augmentation
  }
}

void report(int n, int y1, int y2) {
  //callback(subtrees[n]); return; // uncomment to report whole subtree
  vector<Point>& p = subtrees[n].points;
  typename vector<Point>::iterator l = lower_bound(p.begin(), p.end(), Point(0, y1), cmpY),
                                   r = upper_bound(p.begin(), p.end(), Point(0, y2), cmpY);
  for (; l != r; ++l) callback(*l);
}

void walk(int n, int x, int y1, int y2, bool r) {
  while (!leaf[n]) {
    if (x <= xs[n]) {
      if (r) report(right(n), y1, y2);
      n = left(n);
    } else {
      if (!r) report(left(n), y1, y2);
      n = right(n);
    }
  }
  if ((r && x <= xs[n]) || (!r && x >= xs[n]))
    report(n, y1, y2);
}

// reports points (x, y) with x1 <= x <= x2 && y1 <= y <= y2
void query(int x1, int x2, int y1, int y2) {
  int n = 0;
  while (!leaf[n]) { // find split point
    if      (x1 <= xs[n] && x2 <= xs[n]) n = left(n);
    else if (x1 >  xs[n] && x2 >  xs[n]) n = right(n);
    else break;
  }
  if (!leaf[n]) {
    walk(left(n), x1, y1, y2, true);
    walk(right(n), x2, y1, y2, false);
  } else if (x1 <= xs[n] && xs[n] <= x2) {
    report(n, y1, y2);
  }
}

int main() {
  Point test[] = {{7,2},{2,7},{4,5},{8,1},{5,4},{1,8},{3,6},{6,3}};
  sort(test, test + 8, cmpX); // don't forget to sort!
  build(test, 0, 8, 0);
  query(2, 6, 5, 100);
}
