// works in-place, O(n*d), swap needs to be adapted
// could contain a bug! test against sort first!
//
// len = number of digits
// getdig(x, 0) should get the most significant digit, get(x, len - 1) the lsd etc.
const int max_val=256;
template<typename T, typename F>
void radix_sort_msb(T& seq, F getdig, int len, int l, int r, int base = 0) {
  if (r-l <= 20) {
    sort(&seq[l],&seq[r]); // even better: use insertion sort
    return;
  }
  if (base == len || r-l <= 1) return;
  int start[max_val+1] = {0}, pos[max_val+1] = {0};
  start[0] = l;
  rep(i,l,r) start[getdig(seq[i], base)+1]++;
  rep(i,1,max_val+1) start[i] += start[i-1], pos[i] = start[i];
  rep(c,0,max_val) {
    rep(i,start[c], start[c+1]) {
      for(;;) {
        int d = getdig(seq[i], base);
        if (d==c) break;
        int& j = pos[d];
        swap(seq[i], seq[j]);
        j++;
      }
    }
    radix_sort_msb(seq,getdig,len,start[c],start[c+1],base+1);
  }
}
