int n, m, piv; // rows, columns
long double M[222][222], eps=1e-3;
bool used[222];
//...
int rank = 0;
for(int col = 0; col < m; ++col) {
  for (piv = 0; piv < n; ++piv) if (!used[piv] && abs(M[piv][col]) > eps) break;
  if (piv == n) continue;
  rank++;
  used[piv] = 1;
  for (int i = 0; i < n; ++i) if (i != piv) {
    long double t = M[i][col] / M[piv][col];
    for (int j = 0; j < m; ++j) M[i][j] -= t * M[piv][j];
  }
}
