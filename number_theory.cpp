typedef long long llong;

const int crt_maxlen = 500;
const int maxm = 100010;
// maximum number of digits of n in base p
const int binomod_maxdigs = 100;

llong extended_gcd(llong a, llong b, llong& lastx, llong& lasty) {
  llong x, y, q, tmp;
  x = 0; lastx = 1;
  y = 1; lasty = 0;
  while (b != 0) {
    q = a / b;
    tmp = b;
    b = a % b;
    a = tmp;
    tmp = x; x = lastx - q*x; lastx = tmp;
    tmp = y; y = lasty - q*y; lasty = tmp;
  }
  return a;
}

// solve the linear equation a x == b (mod n)
// returns the number of solutions up to congruence (can be 0)
//    sol: the minimal positive solution
//    dis: the distance between solutions
llong linear_mod(llong a, llong b, llong n, llong &sol, llong &dis) {
  a = (a % n + n) % n, b = (b % n + n) % n;
  llong d, x, y;
  d = extended_gcd(a, n, x, y);
  if (b % d)
    return 0;
  x = (x % n + n) % n;
  x = b / d * x % n;
  dis = n / d;
  sol = x % dis;
  return d;
}

llong powmod(llong base, llong exp, llong m) {
  llong result = 1;
  while (exp) {
    if (exp & 1)
      result = (result * base) % m;
    exp >>= 1;
    base = (base * base) % m;
  }
  return result;
}

// only works correctly for non-negative a!
llong modinv(llong a, llong m) {
  llong lastx, lasty;
  extended_gcd(a, m, lastx, lasty);
  while (lastx < 0)
    lastx += m;
  return lastx;
}

llong modinv_prime(llong a, llong p) {
  return powmod(a, p-2, p);
}

const int pmax = 1000000;
static map<int, int> pfacs[pmax+1];
void precompute_primefacs(llong to) {
  for (llong i = 2; i <= to; i++) {
    if (!pfacs[i].empty()) continue;
    pfacs[i][i]++;
    for (llong j = i+i; j <= to; j += i) {
      llong x = j;
      while (x % i == 0) {
        pfacs[j][i]++;
        x /= i;
      }
    }
  }
}

static bool sieve[pmax+1];
void precompute_primes(llong to) {
  sieve[0] = sieve[1] = true;
  for (llong i = 2; i <= to; i++) {
    if (sieve[i]) continue;
    for (llong j = i*i; j <= to; j += i)
      sieve[j] = true;
  }
}

bool is_prime(llong x) {
  for (llong i = 2; i*i <= x; ++i)
    if (x % i == 0) return false;
  return true;
}

bool compute_pfacs(llong x, map<int, int>& pfacs) {
  for (llong i = 2; i*i <= x; ++i) {
    while (x % i == 0) {
      pfacs[i]++;
      x /= i;
    }
  }
  if (x > 1)
    pfacs[x]++;
  return pfacs.empty();
}

// solve x = b[i] (mod m[i])  0 <= i < n. m[i] must be comprime!
// result will be mod p_0 * ... * p_{n-1}
llong crt_coprime(llong as[], llong ps[], int len, llong& mod) {
  static llong x[crt_maxlen];
  mod = 1;
  for (int i = 0; i < len; ++i) mod *= ps[i];
  llong p = 1, y = 0;
  llong res = x[0] = as[0];
  for (int i = 1; i < len; ++i) {
    y = (y + x[i-1]*p) % mod;
    p *= ps[i-1];
    x[i] = ((as[i] - y) * modinv(p, ps[i])) % mod;
    res = (res + x[i] * p) % mod;
  }
  while (res < 0) res += mod;
  return res;
}

// find x.  a[i] x = b[i] (mod m[i])  0 <= i < n. m[i] need not be coprime
bool crt_gen(int n, llong *a, llong *b, llong *m, llong &sol, llong &mod) {
  llong A = 1, B = 0, ta, tm, tsol, tdis;
  for (int i = 0; i < n; ++i) {
    if (!linear_mod(a[i], b[i], m[i], tsol, tdis))
      return 0;
    ta = tsol, tm = tdis;
    if (!linear_mod(A, ta - B, tm, tsol, tdis))
      return 0;
    B = A * tsol + B;
    A = A * tdis;
  }
  sol = B, mod = A;
  return 1;
}

// compute (n!)_p = product_{i=1, i%p!=0}^n {i} mod p^q
// implementation based on
// http://www.cse.sc.edu/~maxal/gpscripts/binomod.gp
// (function factorialmodp1)
llong partial_fac_mod_pq(llong n, llong p, llong q) {
  if (n <= 1) return 1;
  llong pq = 1;
  for (int i = 0; i < q; ++i) pq *= p;
  llong r = ((n/pq) % 2) ? -1 : 1;
  for (int i = 2; i <= n % pq; ++i)
    if (i % p)
      r = (r * i) % pq;
  while (r < 0) r += pq;
  return r;
}

// compute C(n,m) modulo p^q using the Theorem 1 outlined in
// http://www.dms.umontreal.ca/~andrew/Binomial/genlucas.html
// Implementation based on
// http://www.cse.sc.edu/~maxal/gpscripts/binomod.gp
llong binomod(llong n, llong m, llong p, llong q) {
  static int ndigs[binomod_maxdigs], mdigs[binomod_maxdigs],
             rdigs[binomod_maxdigs], N[binomod_maxdigs],
             M[binomod_maxdigs], R[binomod_maxdigs], e[binomod_maxdigs];
  memset(ndigs, 0, sizeof ndigs);
  memset(mdigs, 0, sizeof mdigs);
  memset(mdigs, 0, sizeof rdigs);
  memset(N, 0, sizeof N);
  memset(M, 0, sizeof M);
  memset(M, 0, sizeof R);
  memset(e, 0, sizeof e);
  int d = 0;
  llong n1 = n, m1 = m, r1 = n-m;
  while (n1) {
    ndigs[d] = n1 % p; mdigs[d] = m1 % p; rdigs[d] = r1 % p;
    n1 /= p; m1 /= p; r1 /= p;
    ++d;
  }
  for (int i = 0; i < d; i++)
    e[i] = (ndigs[i] < (mdigs[i] + ((i > 0) ? e[i-1] : 0)));
  for (int i = d - 2; i >= 0; i--)
    e[i] += e[i+1];

  if (e[0] >= q) return 0;
  llong pq = 1;
  for (int i = 0; i < q; ++i)
    pq *= p;
  q -= e[0];

  n1 = n; m1 = m; r1 = n-m;
  llong pq1 = 1;
  for (int i = 0; i < q; ++i) pq1 *= p;
  d = 0;
  while (n1) {
    N[d] = n1 % pq1; M[d] = m1 % pq1; R[d] = r1 % pq1;
    n1 /= p; m1 /= p; r1 /= p;
    ++d;
  }

  llong res = ((p > 2 || q < 3) && q < d && e[q-1] % 2) ? -1 : 1;
  for (int i = 0; i < e[0]; i++)
    res = (res * p) % pq;
  for (int i = 0; i < d; i++) {
    llong x = partial_fac_mod_pq(N[i], p, q) % pq;
    x = (x * modinv(partial_fac_mod_pq(M[i], p, q), pq)) % pq;
    x = (x * modinv(partial_fac_mod_pq(R[i], p, q), pq)) % pq;
    res = (res * x) % pq;
  }
  while (res < 0) res += pq;
  return res;
}

// calculates (product_{i=1,i%p!=0}^n i) % p^e
// cnt is the exponent of p in n!
// Time: p^e + log(p, n)
int get_part_of_fac_n_mod_pe(int n, int p, int mod, int *upto, int &cnt) {
  if (n < p) {
    cnt = 0;
    return upto[n];
  } else {
    int res = powmod(upto[mod], n / mod, mod);
    res = (llong) res * upto[n % mod] % mod;
    res = (llong) res * get_part_of_fac_n_mod_pe(n / p, p, mod, upto, cnt)
                                                       % mod;
    cnt += n / p;
    return res;
  }
}

// C(n,k) % p^e where mod == p^e. Sometimes Granville is faster, sometimes
// this one
int binomod2(int n, int k, int p, int mod) {
  static int upto[maxm + 1];
  upto[0] = 1 % mod;
  for (int i = 1; i <= mod; ++i)
    upto[i] = i % p ? (llong) upto[i - 1] * i % mod : upto[i - 1];
  int cnt1, cnt2, cnt3;
  int a = get_part_of_fac_n_mod_pe(n, p, mod, upto, cnt1);
  int b = get_part_of_fac_n_mod_pe(k, p, mod, upto, cnt2);
  int c = get_part_of_fac_n_mod_pe(n - k, p, mod, upto, cnt3);
  int res = (llong) a * modinv(b, mod) % mod * modinv(c, mod) % mod
                   * powmod(p, cnt1 - cnt2 - cnt3, mod) % mod;
  return res;
}

int binomod_gen(int n, int k, int m) {
  static llong partp[crt_maxlen], partq[crt_maxlen], partmod[crt_maxlen];
  int partn = 0, tm = m;
  for (int i = 2; i * i <= tm; ++i)
    if (tm % i == 0) {
      partq[partn] = 0;
      partp[partn] = i;
      partmod[partn] = 1;
      while (tm % i == 0) {
        tm /= i;
        partq[partn] += 1;
        partmod[partn] *= i;
      }
      ++partn;
    }
  if (tm > 1) {
    partq[partn] = 1;
    partp[partn] = tm;
    partmod[partn] = tm;
    ++partn;
  }

  llong coef[partn], res[partn];
  for (int i = 0; i < partn; ++i) {
    coef[i] = 1;
    // choose whichever is faster ;)
    res[i] = binomod2(n, k, partp[i], partmod[i]);
    //res[i] = binomod(n, k, partp[i], partq[i]);
  }
  llong sol, mod;
  crt_gen(partn, coef, res, partmod, sol, mod);
  return sol;
}

// get number of permutations {P_1, ..., P_n} of size n,
// where no number is at its original position (that is, P_i != i for all i)
// also called subfactorial !n
int get_derangement_mod_m(int n, int m) {
  static int res[maxm * 2];
  int d = 1 % m, p = 1;
  res[0] = d;
  for (int i = 1; i <= n && i < 2 * m; ++i) {
    p *= -1;
    d = ((llong) i * d + p + m) % m;
    res[i] = d;
    if (i == n)
      return d;
  }
  // it turns out that !n mod m == !(n mod 2m) mod m
  return res[n % (2 * m)];
}

