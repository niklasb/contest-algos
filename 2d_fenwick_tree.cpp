#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int maxn = 111;
int max_x = 100, max_y = 100;
int tree[maxn][maxn];

// 2D point-update prefix-query
void update_idx(int x, int y, int val) {
  for (int  i = x; i <= max_x; i += i & -i)
    for (int j = y; j <= max_y; j += j & -j)
      tree[i][j] += val;
}
int read_prefix(int x, int y) {
  int res = 0;
  for (int  i = x; i > 0; i -= i & -i)
    for (int j = y; j > 0; j -= j & -j)
      res += tree[i][j];
  return res;
}

// 2D prefix-update prefix-query
typedef ll Tree[maxn][maxn];
Tree A, B, C, D;
void update_idx(Tree t, int x, int y, ll val) {
  for (int  i = x; i <= max_x; i += i & -i)
    for (int j = y; j <= max_y; j += j & -j)
      t[i][j] += val;
}
void update_prefix(int x, int y, ll val) {
  update_idx(A, 1, 1, val);

  update_idx(A, 1, y + 1, -val);
  update_idx(B, 1, y + 1, val * y);

  update_idx(A, x + 1, 1, -val);
  update_idx(C, x + 1, 1, val * x);

  update_idx(A, x + 1, y + 1, val);
  update_idx(B, x + 1, y + 1, -val * y);
  update_idx(C, x + 1, y + 1, -val * x);
  update_idx(D, x + 1, y + 1, val * x * y);
}
ll read_prefix(int x, int y) {
  ll a, b, c, d;
  a = b = c = d = 0;
  for (int  i = x; i > 0; i -= i & -i)
    for (int j = y; j > 0; j -= j & -j) {
      a += A[i][j]; b += B[i][j]; c += C[i][j]; d += D[i][j];
    }
  return a * x * y + b * x + c * y + d;
}
// helpers
void update_range(int x1, int y1, int x2, int y2, ll val) {
  update_prefix(x2, y2, val);
  update_prefix(x1 - 1, y2, -val);
  update_prefix(x2, y1 - 1, -val);
  update_prefix(x1 - 1, y1 - 1, val);
}
ll read_range(int x1, int y1, int x2, int y2) {
  return read_prefix(x2, y2)
            - read_prefix(x1 - 1, y2) - read_prefix(x2, y1 - 1)
            + read_prefix(x1 - 1, y1 - 1);
}
ll read_idx(int x, int y) {
  return read_range(x,y,x,y);
}

// reference
ll naive[maxn][maxn];
void update_idx_ref(int x, int y, ll val) {
  naive[x][y] += val;
}
void update_prefix_ref(int x, int y, ll val) {
  for (int i = 1; i <= x; ++i)
    for (int j = 1; j <= y; ++j)
      naive[i][j] += val;
}
ll read_prefix_ref(int x, int y) {
  ll res = 0;
  for (int i = 1; i <= x; ++i)
    for (int j = 1; j <= y; ++j)
      res += naive[i][j];
  return res;
}

int main() {
  update_range(3, 2, 4, 4, 2);
  update_range(2, 4, 5, 5, 3);
  for (int i = 1; i <= 6; ++i) {
    for (int j = 1; j <= 6; ++j)
      cout << read_idx(i, j) << " ";
    cout << endl;
  }
  return 0;
  srand(time(NULL));
  for (int i = 1; i <= 100000; ++i) {
    int x = 1 + (rand() % max_x);
    int y = 1 + (rand() % max_y);
    int ty = rand() % 2;
    if (ty == 0) {
      int val = rand() % 10000;
      update_prefix(x, y, val);
      update_prefix_ref(x, y, val);
    } else if (ty == 1) {
      assert(read_prefix_ref(x,y) == read_prefix(x,y));
    }
  }
}
