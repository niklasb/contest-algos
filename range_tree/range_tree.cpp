#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

typedef int X; typedef int Y;
struct Point { X x; Y y; Point() :x(0),y(0){}; Point(X _x, Y _y) :x(_x),y(_y){}; };

struct Subtree {
  Subtree() : points() {}
  Subtree(vector<Point>& p) : points(p) { }
  vector<Point> points;
  // augmentation hooks!
  void update(Subtree& left, Subtree& right) { }
  void update_leaf() { }
};

class RangeTree2D {
public:
#define _SIZE (4*input.size()+10)
#define _CB template <typename Callback>
  // input array must be sorted by the X coordinate
  RangeTree2D(vector<Point>& input) : xs(_SIZE), leaf(_SIZE), subtrees(_SIZE) {
    build(input, 0, input.size(), 0);
  }

  // reports points (x, y) with x1 <= x <= x2 && y1 <= y <= y2
  _CB void query(int x1, int x2, int y1, int y2, Callback cb) {
    int n = 0;
    while (!leaf[n]) { // find split point
      if      (x1 <= xs[n] && x2 <= xs[n]) n = left(n);
      else if (x1 >  xs[n] && x2 >  xs[n]) n = right(n);
      else break;
    }
    if (!leaf[n]) {
      walk(left(n), x1, y1, y2, true, cb);
      walk(right(n), x2, y1, y2, false, cb);
    } else if (x1 <= xs[n] && xs[n] <= x2) {
      report_points(n, y1, y2, cb);
    }
  }

  _CB void walk(int n, int x, int y1, int y2, bool report_right, Callback cb) {
    while (!leaf[n]) {
      if (x <= xs[n]) {
        if (report_right) report_points(right(n), y1, y2, cb);
        n = left(n);
      } else {
        if (!report_right) report_points(left(n), y1, y2, cb);
        n = right(n);
      }
    }
    if ((report_right && x <= xs[n]) || (!report_right && x >= xs[n]))
      report_points(n, y1, y2, cb);
  }

private:
  _CB void report_points(int n, int y1, int y2, Callback cb) {
    // cb(subtrees[n]); return // uncomment to report whole subtree
    vector<Point>& p = subtrees[n].points;
    typename vector<Point>::iterator l = lower_bound(p.begin(), p.end(), Point(0, y1), cmpY),
                                     r = upper_bound(p.begin(), p.end(), Point(0, y2), cmpY);
    for (; l != r; ++l) cb(*l);
  }

  void build(vector<Point>& input, int start, int end, int pos) {
    int m = (end + start - 1)/2;
    xs[pos] = input[m].x;
    if ((leaf[pos] = end - start == 1)) {
      subtrees[pos].points.push_back(input[m]);
      subtrees[pos].update_leaf(); // only necessary if Subtree class was augmented
    } else {
      build(input, start, m+1, left(pos));
      build(input, m+1, end, right(pos));
      merge(subtrees[left(pos)].points.begin(), subtrees[left(pos)].points.end(),
            subtrees[right(pos)].points.begin(), subtrees[right(pos)].points.end(),
            back_inserter(subtrees[pos].points), cmpY);
      subtrees[pos].update(subtrees[left(pos)], subtrees[right(pos)]); // only for augmentation
    }
  }

  static bool cmpY(const Point& a, const Point& b) { return a.y < b.y; }
  int left(int i) { return i * 2 + 1; }
  int right(int i) { return i * 2 + 2; }
  int parent(int i) { return (i-1)/2; }

  int size;
  vector<X> xs;
  vector<bool> leaf;
  vector<Subtree> subtrees;
};

struct callback {
  callback(RangeTree2D& t) : tree(t) { }
  void operator()(const Point& p) {
    cout << p.x << " " << p.y << endl;
  }
  RangeTree2D& tree;
};

int main() {
  vector<Point> test;
  test.push_back(Point(1, 8));
  test.push_back(Point(2, 7));
  test.push_back(Point(3, 6));
  test.push_back(Point(4, 5));
  test.push_back(Point(5, 4));
  test.push_back(Point(6, 3));
  test.push_back(Point(7, 2));
  test.push_back(Point(8, 1));
  RangeTree2D t(test);
  t.query(1, 6, 5, 100, callback(t));
}
