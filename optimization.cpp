const double eps = 1e-10;

// assume a unimodal or monotonous function
template <typename T>
double ternary_search(T lt, double l, double r) {
  double yl = f(l);
  double yr = f(r);
  double ym = f((l+r)/2);
  if (lt(ym, lt(yl, yr) ? yl : yr))
    return lt(yl, yr) ? yr : yl;
  while (r-l > eps || abs(yl-yr) > eps) {
    double m1 = (2*l + r) / 3;
    double m2 = (l + 2*r) / 3;
    double ym1 = f(m1);
    double ym2 = f(m2);
    if (lt(ym1, ym2)) {
      l = m1;
      yl = ym1;
    } else {
      r = m2;
      yr = ym2;
    }
  }
  return f((l+r)/2);
}

double ternary_search_max(double l, double r) {
  return ternary_search(less, l, r);
}

double ternary_search_min(double l, double r) {
  return ternary_search(greater, l, r);
}
