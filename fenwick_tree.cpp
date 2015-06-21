const int n = 10000; // ALL INDICES START AT 1 WITH THIS CODE!!

// mode 1: update indices, read prefixes
void update_idx(int tree[], int i, int val) { // v[i] += val
  for (; i <= n; i += i & -i) tree[i] += val;
}
int read_prefix(int tree[], int i) { // get sum v[1..i]
  int sum = 0;
  for (; i > 0; i -= i & -i) sum += tree[i];
  return sum;
}
int kth(int k) { // find kth element in tree (1-based index)
  int ans = 0;
  for (int i = maxl; i >= 0; --i) // maxl = largest i s.t. (1<<i) <= n
    if (ans + (1<<i) <= N && tree[ans + (1<<i)] < k) {
      ans += 1<<i;
      k -= tree[ans];
    }
  return ans+1;
}

// mode 2: update prefixes, read indices
void update_prefix(int tree[], int i, int val) { // v[1..i] += val
  for (; i > 0; i -= i & -i) tree[i] += val;
}
int read_idx(int tree[], int i) { // get v[i]
  int sum = 0;
  for (; i <= n; i += i & -i) sum += tree[i];
  return sum;
}

// mode 3: range-update range-query (using point-wise of linear functions)
const int maxn = 100100;
int n;
ll mul[maxn], add[maxn];

void update_idx(ll tree[], int x, ll val) {
  for (int i = x; i <= n; i += i & -i) tree[i] += val;
}
void update_prefix(int x, ll val) { // v[x] += val
  update_idx(mul, 1, val);
  update_idx(mul, x + 1, -val);
  update_idx(add, x + 1, x * val);
}
ll read_prefix(int x) { // get sum v[1..x]
  ll a = 0, b = 0;
  for (int i = x; i > 0; i -= i & -i) a += mul[i], b += add[i];
  return a * x + b;
}
void update_range(int l, int r, ll val) { // v[l..r] += val
  update_prefix(l - 1, -val);
  update_prefix(r, val);
}
ll read_range(int l, int r) { // get sum v[l..r]
  return read_prefix(r) - read_prefix(l - 1);
}
