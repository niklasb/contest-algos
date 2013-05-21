#include <iostream>
using namespace std;

const int n = 100;
static int tree[n+1];

// 1 <= idx <= n
int read(int idx){
  int sum = 0;
  while (idx > 0){
    sum += tree[idx];
    idx -= (idx & -idx);
  }
  return sum;
}

void update(int idx, int val){
  while (idx <= n) {
    tree[idx] += val;
    idx += (idx & -idx);
  }
}

int main() {
  update(3, 10);
  update(4, 100);
  update(1, 5);
  update(2, 1);
  cout << read(3) << endl;
}
