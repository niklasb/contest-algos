// represents fennwick tree over interval 1..n (n incl.)
int t[maxn];
int n;

void update(int idx, int x) {
  while (idx <= n) {
    t[idx] += x;
    idx += idx & -idx;
  }
}

int read(int idx) {
  int sum = 0;
  while (idx > 0) {
    sum += t[idx];
    idx -= idx & -idx;
  }
  return sum;
}
