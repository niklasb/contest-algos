// compute (n!)_p = product_{i=1, i%p!=0}^n {i} mod p^q
// implementation based on
// http://www.cse.sc.edu/~maxal/gpscripts/binomod.gp
// (function factorialmodp1)
ll partial_fac_mod_pq(ll n, ll p, ll q) {
  if (n <= 1) return 1;
  ll pq = 1;
  for (int i = 0; i < q; ++i) pq *= p;
  ll r = ((n/pq) % 2) ? -1 : 1;
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
const int binomod_maxdigs = 100;// max number of digits of n in base p
ll binomod(ll n, ll m, ll p, ll q) {
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
  ll n1 = n, m1 = m, r1 = n-m;
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
  ll pq = 1;
  for (int i = 0; i < q; ++i)
    pq *= p;
  q -= e[0];

  n1 = n; m1 = m; r1 = n-m;
  ll pq1 = 1;
  for (int i = 0; i < q; ++i) pq1 *= p;
  d = 0;
  while (n1) {
    N[d] = n1 % pq1; M[d] = m1 % pq1; R[d] = r1 % pq1;
    n1 /= p; m1 /= p; r1 /= p;
    ++d;
  }

  ll res = ((p > 2 || q < 3) && q < d && e[q-1] % 2) ? -1 : 1;
  for (int i = 0; i < e[0]; i++)
    res = (res * p) % pq;
  for (int i = 0; i < d; i++) {
    ll x = partial_fac_mod_pq(N[i], p, q) % pq;
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
    res = (ll) res * upto[n % mod] % mod;
    res = (ll) res * get_part_of_fac_n_mod_pe(n / p, p, mod, upto, cnt)
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
    upto[i] = i % p ? (ll) upto[i - 1] * i % mod : upto[i - 1];
  int cnt1, cnt2, cnt3;
  int a = get_part_of_fac_n_mod_pe(n, p, mod, upto, cnt1);
  int b = get_part_of_fac_n_mod_pe(k, p, mod, upto, cnt2);
  int c = get_part_of_fac_n_mod_pe(n - k, p, mod, upto, cnt3);
  int res = (ll) a * modinv(b, mod) % mod * modinv(c, mod) % mod
                   * powmod(p, cnt1 - cnt2 - cnt3, mod) % mod;
  return res;
}

const int crt_maxlen = 500;
int binomod_gen(int n, int k, int m) {
  static ll partp[crt_maxlen], partq[crt_maxlen], partmod[crt_maxlen];
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

  ll coef[partn], res[partn];
  for (int i = 0; i < partn; ++i) {
    coef[i] = 1;
    // choose whichever is faster ;)
    res[i] = binomod2(n, k, partp[i], partmod[i]);
    //res[i] = binomod(n, k, partp[i], partq[i]);
  }
  ll sol, mod;
  crt(partn, coef, res, partmod, sol, mod);
  return sol;
}
