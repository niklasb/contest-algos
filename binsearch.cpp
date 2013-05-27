template <typename T>
// returns the minimum i from [down, up) that satisfies pred(i)
// or up if no such i exists
int findfirst(int down, int up, T pred) {
  int lo=down,hi=up-1;
  while (lo<hi) {
    int mid = (hi+lo)/2;
    if (pred(mid)) hi=mid;
    else lo=mid+1;
  }
  return pred(lo) ? lo : up;
}
