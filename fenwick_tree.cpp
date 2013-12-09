const int n = 10;

// mode 1: update indices, read prefixes
void update_i(int tree[], int i, int val) { // v[i] += val
  for (; i <= n; i += i & -i) tree[i] += val;
}
int read_prefix(int tree[], int i) { // get sum v[1..i]
  int sum = 0;
  for (; i > 0; i -= i & -i) sum += tree[i];
  return sum;
}
// mode 2: update prefixes, read indices
void update_prefix(int tree[], int i, int val) { // v[1..i] += val
  for (; i > 0; i -= i & -i) tree[i] += val;
}
int read_i(int tree[], int i) { // get v[i]
  int sum = 0;
  for (; i <= n; i += i & -i) sum += tree[i];
  return sum;
}

#include <iostream>
using namespace std;
int tree[n+1];
int main() {
  update_prefix(tree, 3, 1);
  update_prefix(tree, 5, 3);
  for (int i = 1; i <= n; ++ i) cout << read_prefix(tree, i) << " ";cout<<endl;
}
