#include <iostream>
using namespace std;

const int n = 100;
static int tree[n+1];

// 1 <= idx <= n
int read(int idx){
  int sum = 0;
  for (; idx > 0; idx -= idx&-idx)
    sum += tree[idx];
  return sum;
}

void update(int idx, int val){
  for (; idx <= n; idx += idx&-idx)
    tree[idx] += val;
}

int main() {
  update(3, 10);
  update(4, 100);
  update(1, 5);
  update(2, 1);
  cout << read(3) << endl;
}
