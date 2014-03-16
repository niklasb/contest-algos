// return first i in [l,r-1] with pred(i)
// or r otherwise
template<typename T, typename F>
T first(T l, T r, F pred) {
  T lo = l, hi = r;
  while (lo < hi) {
    T mid = (lo + hi) >> 1;
    if (pred(mid)) hi = mid;
    else lo = mid + 1;
  }
  return lo;
}
