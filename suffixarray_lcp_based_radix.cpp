const int maxn = 200010, maxlg = 18; // maxlg = ceil(log_2(maxn))
struct SA {
  pair<pii, int> L[maxn]; // O(n * log n) space
  int P[maxlg+1][maxn], n, stp, cnt, sa[maxn];
  SA(const string& s) : n(s.size()) { // O(n * log n)
    rep(i,0,n) P[0][i] = s[i];
    sa[0] = 0; // in case n == 1
    for (stp = 1, cnt = 1; cnt < n; stp++, cnt <<= 1) {
      rep(i,0,n) L[i] = mk(mk(P[stp-1][i], i + cnt < n ? P[stp-1][i+cnt] : -1), i);
      std::sort(L, L + n);
      rep(i,0,n)
        P[stp][L[i].snd] = i>0 && L[i].fst == L[i-1].fst ?  P[stp][L[i-1].snd] : i;
    }
    rep(i,0,n) sa[i] = L[i].snd;
  }
  int lcp(int x, int y) { // time log(n); x, y = indices into string, not SA
    int k, ret = 0;
    if (x == y) return n - x;
    for (k = stp - 1; k >= 0 && x < n && y < n; k --)
      if (P[k][x] == P[k][y])
        x += 1<<k, y += 1<<k, ret += 1<<k;
    return ret;
  }
};
