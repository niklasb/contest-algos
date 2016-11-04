int N, sum[2*maxn], mul[2*maxn], lo[2*maxn], hi[2*maxn];
void push(int x) {
  if (x < N) {
    mul[2*x] *= mul[x];
    mul[2*x+1] *= mul[x];
  }
  sum[x] *= mul[x];
  mul[x] = 1;
}
void maintain(int x) {
  push(2*x);
  push(2*x+1);
  sum[x] = sum[2*x] + sum[2*x+1];
  mul[x] = id;
}
void init(int n) {
  for (N=1; N<n; N<<=1);
  for (int i = 0; i < n; ++i) {
    sum[N+i] = base.pow(a[i]);
    mul[N+i] = id;
  }
  for (int i = 0; i < N; ++i) lo[N+i] = hi[N+i] = i;
  for (int i = N-1; i >= 1; --i) {
    maintain(i);
    lo[i] = lo[2*i];
    hi[i] = hi[2*i+1];
  }
}
void update(int x, int ql, int qr, matrix val) {
  if (hi[x] < ql || lo[x] > qr) return;
  if (ql <= lo[x] && qr >= hi[x]) {
    mul[x] *= val;
    return;
  }
  push(x);
  update(2*x, ql, qr, val);
  update(2*x+1, ql, qr, val);
  maintain(x);
}
int qry(int x, int ql, int qr) {
  if (hi[x] < ql || lo[x] > qr) return 0;
  push(x);
  if (ql <= lo[x] && qr >= hi[x]) return sum[x];
  return qry(2*x, ql, qr) + qry(2*x+1, ql, qr);
}
