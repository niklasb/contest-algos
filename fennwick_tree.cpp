const int n = 10;

void update_idx(int tree[], int idx, int val) { // v[idx] += val
  for (; idx <= n; idx += idx & -idx) tree[idx] += val;
}
int read_prefix(int tree[], int idx) { // get sum v[1..idx]
  int sum = 0;
  for (; idx > 0; idx -= idx & -idx) sum += tree[idx];
  return sum;
}
void update_prefix(int tree[], int idx, int val) { // v[1..idx] += val
  for (; idx > 0; idx -= idx & -idx) tree[idx] += val;
}
int read_idx(int tree[], int idx) { // get v[idx]
  int sum = 0;
  for (; idx <= n; idx += idx & -idx) sum += tree[idx];
  return sum;
}

#include <iostream>
using namespace std;
int tree[n+1];
int main() {
  update_prefix(tree, 3, 1);
  update_prefix(tree, 5, 3);
  for (int i = 1; i <= n; ++ i) cout << read_idx(tree, i) << " ";cout<<endl;
}
