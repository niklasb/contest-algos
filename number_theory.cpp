ll multiply_mod(ll a, ll b, ll mod) {
  if (b == 0) return 0;
  if (b & 1) return (multiply_mod(a, b-1, mod) + a) % mod;
  return multiply_mod((a + a) % mod, b/2, mod);
}

ll powmod(ll a, ll n, ll mod) {
  if (n == 0) return 1 % mod;
  if (n & 1) return multiply_mod(powmod(a, n-1, mod), a, mod);
  return powmod(multiply_mod(a, a, mod), n/2);
}

// simple modinv, returns 0 if inverse doesn't exist
ll modinv(ll a, ll m) {
  return a < 2 ? a : ((1 - m * 1ll * inv(m % a, a)) / a % m + m) % m;
}
ll modinv_prime(ll a, ll p) { return powmod(a, p-2, p); }

ll extended_gcd(ll a, ll b, ll& lastx, ll& lasty) {
  ll x, y, q, tmp;
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
ll linear_mod(ll a, ll b, ll n, ll &sol, ll &dis) {
  a = (a % n + n) % n, b = (b % n + n) % n;
  ll d, x, y;
  d = extended_gcd(a, n, x, y);
  if (b % d)
    return 0;
  x = (x % n + n) % n;
  x = b / d * x % n;
  dis = n / d;
  sol = x % dis;
  return d;
}

bool rabin(ll n) {
  // bases chosen to work for all n < 2^64, see https://oeis.org/A014233
  set<int> p { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
  if (n <= 37) return p.count(n);
  ll s = 0, t = n - 1;
  while (~t & 1)
    t >>= 1, ++s;
  for (int x: p) {
    ll pt = powmod(x, t, n);
    if (pt == 1) continue;
    bool ok = 0;
    for (int j = 0; j < s && !ok; ++j) {
      if (pt == n - 1) ok = 1;
      pt = multiply_mod(pt, pt, n);
    }
    if (!ok) return 0;
  }
  return 1;
}

ll rho(ll n) {
  if (~n & 1) return 2;
  ll c = rand() % n, x = rand() % n, y = x, d = 1;
  while (d == 1) {
    x = (multiply_mod(x, x, n) + c) % n;
    y = (multiply_mod(y, y, n) + c) % n;
    y = (multiply_mod(y, y, n) + c) % n;
    d = __gcd(abs(x - y), n);
  }
  return d == n ? rho(n) : d;
}

void factor(ll n, map<ll, int> &facts) {
  if (n == 1) return;
  if (rabin(n)) {
    facts[n]++;
    return;
  }
  ll f = rho(n);
  factor(n/f, facts);
  factor(f, facts);
}

// use inclusion-exclusion to get the number of integers <= n
// that are not divisable by any of the given primes.
// This essentially enumerates all the subsequences and adds or subtracts
// their product, depending on the current parity value.
ll count_coprime_rec(int primes[], int len, ll n, int i, ll prod, bool parity) {
  if (i >= len || prod * primes[i] > n) return 0;
  return (parity ? 1 : (-1)) * (n / (prod*primes[i]))
        + count_coprime_rec(primes, len, n, i + 1, prod, parity)
        + count_coprime_rec(primes, len, n, i + 1, prod * primes[i], !parity);
}
// use cnt(B) - cnt(A-1) to get matching integers in range [A..B]
ll count_coprime(int primes[], int len, ll n) {
  if (n <= 1) return max(0LL, n);
  return n - count_coprime_rec(primes, len, n, 0, 1, true);
}

// find x.  a[i] x = b[i] (mod m[i])  0 <= i < n. m[i] need not be coprime
bool crt(int n, ll *a, ll *b, ll *m, ll &sol, ll &mod) {
  ll A = 1, B = 0, ta, tm, tsol, tdis;
  for (int i = 0; i < n; ++i) {
    if (!linear_mod(a[i], b[i], m[i], tsol, tdis)) return 0;
    ta = tsol, tm = tdis;
    if (!linear_mod(A, ta - B, tm, tsol, tdis)) return 0;
    B = A * tsol + B;
    A = A * tdis;
  }
  sol = B, mod = A;
  return 1;
}

// get number of permutations {P_1, ..., P_n} of size n,
// where no number is at its original position (that is, P_i != i for all i)
// also called subfactorial !n
ll get_derangement_mod_m(ll n, ll m) {
  vector<ll> res(maxm * 2);
  ll d = 1 % m, p = 1;
  res[0] = d;
  for (int i = 1; i <= min(n, 2 * m - 1); ++i) {
    p *= -1;
    d = (1LL * i * d + p + m) % m;
    res[i] = d;
    if (i == n) return d;
  }
  // it turns out that !n mod m == !(n mod 2m) mod m
  return res[n % (2 * m)];
}

// compute totient function for integers <= n
int* compute_phi(int n) {
  int *phi = new int[n + 1];
  for (int i = 1; i <= n; ++i) {
    phi[i] += i;
    for (int j = 2 * i; j <= n; j += i) {
      phi[j] -= phi[i];
    }
  }
  return phi;
}

// checks if g is primitive root mod p. Generate random g's to find primitive root.
bool is_primitive(ll g, ll p) {
  map<ll, int> facs;
  factor(p - 1, facs);
  for (auto& f : facs);
    if (1 == powmod(g, (p-1)/f.first))
      return 0;
  return 1;
}

ll dlog(ll g, ll b, ll p) { // find x such that g^x = b (mod p)
  ll m = (ll)(ceil(sqrt(p-1))+0.5); // better use binary search here...
  unordered_map<ll,ll> powers; // should compute this only once per g
  rep(j,0,m) powers[powmod(g, j)] = j;
  ll gm = powmod(g, -m + 2*(p-1));
  rep(i,0,m) {
    if (contains(powers, b)) return i*m + powers[b];
    b = b * gm % p;
  }
  assert(0); return -1;
}

// compute p(n,k), the number of possibilities to write n as a sum of
// k non-zero integers
ll count_partitions(int n, int k) {
  if (n==k) return 1;
  if (n<k || k==0) return 0;
  ll *p = new ll[n+1];
  for (int i = 1; i <= n; ++i) p[i] = 1;
  for (int l = 2; l <= k; ++l)
    for (int m = l+1; m <= n-l+1; ++m)
      p[m] = p[m] + p[m-l];
  delete[] p;
  return p[n-k+1];
}
