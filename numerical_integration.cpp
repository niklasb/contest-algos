// adaptive Simpson's rule
double f(double x) { return x*x; }
const double eps=1e-12;

double simps(double a, double b) {
  return (f(a) + 4*f((a+b)/2) + f(b))*(b-a)/6;
}
double integrate(double a, double b) {
  double m = (a+b)/2;
  double l = simps(a,m),r = simps(m,b),tot=simps(a,b);
  if (fabs(l+r-tot) < eps) return tot;
  return integrate(a,m) + integrate(m,b);
}

// 4x improvements by avoiding recomputation of function calls:
double simps(double a, double b, double fa, double fm, double fb) {
  return (fa + 4*fm + fb)*(b-a)/6;
}
double integr(double a, double b, double fa, double fm, double fb) {
  double m = (a+b)/2;
  double fam = f((a+m)/2), fmb = f((m+b)/2);
  double l = simps(a,m,fa,fam,fm),r = simps(m,b,fm,fmb,fb),tot=simps(a,b,fa,fm,fb);
  if (fabs(l+r-tot) < eps) return tot;
  return integr(a,m,fa,fam,fm) + integr(m,b,fm,fmb,fb);
}
double integrate(double a, double b) {
  return integr(a,b,f(a),f((a+b)/2),f(b));
}
