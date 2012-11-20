#include <iostream>
#include <vector>
using namespace std;

#define REP(i,n) for (int i = 0; i < (n); ++i)
#define SZ(x) ((int)(x).size())

// below is a quad tree optimized for range sum queries. using it,
// we can update the value of single cells of a grid and query the sum of
// a 2D range

struct Point {
  int x, y;
  int value;
  Point(int x_, int y_, int value_) : x(x_), y(y_), value(value_) {}
  Point() : x(0), y(0), value(0) {}
};

const size_t CAPACITY = 20;
const int CHILDSIZE = 4;

struct Quadtree {
  Quadtree *children[CHILDSIZE];
  vector<Point> points;
  bool leaf;
  int x, y, width, height, w1, h1;
  int sum;
  Quadtree(int x_, int y_, int width_, int height_)
    : points(), leaf(true), x(x_), y(y_), width(width_), height(height_), sum(0)
  {}
  ~Quadtree() {
    if (!leaf) REP(i, CHILDSIZE) delete children[i];
  }

  int range_query(int x1, int x2, int y1, int y2) {
    if (x1 <= x && x2 >= x + width && y1 <= y && y2 >= y + height) return sum;
    if (x2 <= x || x1 >= x + width || y2 <= y || y1 >= y + height) return 0;
    int res = 0;
    if (leaf) {
      REP(i, SZ(points)) {
        Point& p = points[i];
        if (x1 <= p.x && p.x < x2 && y1 <= p.y && p.y < y2)
          res += p.value;
      }
    } else {
      REP(i, CHILDSIZE)
        res += children[i]->range_query(x1, x2, y1, y2);
    }
    return res;
  }

  void update(int x, int y, int diff) {
    sum += diff;
    if (leaf) {
      bool found = false;
      REP(i, SZ(points))
        if (points[i].x == x && points[i].y == y) {
          points[i].value += diff;
          found = true;
          break;
        }
      if (!found) {
        sum -= diff;
        insert(Point(x,y,diff));
      }
    } else get_child(x, y).update(x, y, diff);
  }

  void split() {
    if (width == 1 && height == 1) return;
    leaf = false;
    w1 = width/2, h1 = height/2;
    children[0] = new Quadtree(x, y, w1, h1);
    children[1] = new Quadtree(x+w1, y, width-w1, h1);
    children[2] = new Quadtree(x+w1, y+h1, width-w1, height-h1);
    children[3] = new Quadtree(x, y+h1, w1, height-h1);
    REP(i, SZ(points))
      get_child(points[i].x, points[i].y).insert(points[i]);
    points.clear();
  }

  void insert(const Point& p) {
    sum += p.value;
    if (leaf && points.size() == CAPACITY) split();
    if (leaf) points.push_back(p);
    else get_child(p.x, p.y).insert(p);
  }

  Quadtree& get_child(int x, int y) {
    for (int i = 0; i < CHILDSIZE-1; ++i) {
      Quadtree& c = *children[i];
      if (x >= c.x && y >= c.y && x < c.x + c.width && y < c.y + c.height)
        return c;
    }
    return *children[CHILDSIZE-1];
  }
};
