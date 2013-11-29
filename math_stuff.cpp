typedef long long LL;

// compute nth fibonacci number modulo m in O(log n)
LL fib(LL n, LL m) {
  if (!n) return 0;
  LL a=1,b=0,c=0,d=1;
  LL a1=1,b1=1,c1=1,d1=0;
  LL a2,b2,c2,d2;
  n -= 1;
  while (n) {
    if (n & 1) {
      a2=(a*a1%m + b*c1%m)%m;
      b2=(a*b1%m + b*d1%m)%m;
      c2=(c*a1%m + d*c1%m)%m;
      d2=(c*b1%m + d*d1%m)%m;
      a=a2;b=b2;c=c2;d=d2;
    }
    n >>= 1;
    a2=(a1*a1%m + b1*c1%m)%m;
    b2=(a1*b1%m + b1*d1%m)%m;
    c2=(c1*a1%m + d1*c1%m)%m;
    d2=(c1*b1%m + d1*d1%m)%m;
    a1=a2;b1=b2;c1=c2;d1=d2;
  }
  return a;
}

ll p;
ll powmod(ll x, ll e) {
  if (e == 0) return 1;
  if (e & 1) return x * powmod(x, e - 1) % p;
  ll y = powmod(x, e / 2);
  return y * y % p;
}

// factors_p1 = prime factors of (p - 1)
bool is_primitive(ll g) {
  for (auto q : factors_p1) {
    if (1 == powmod(g, (p-1)/q)) return 0;
  }
  return 1;
}
ll g; // g = primitive root of p
void find_prim() {
  for (;;) {
    g = (((ll)rand() << 15) | (ll)rand()) % p;
    if (g < 2) continue;
    if (is_primitive(g)) return;
  }
}
bool have_powers = 0;
unordered_map<ll,ll> powers;
ll dlog(ll b) { // find x such that g^x = b (mod p)
  ll m = (ll)(ceil(sqrt(p-1))+0.5); // better use binary search here...
  if (!have_powers) rep(j,0,m) powers[powmod(g, j)] = j;
  have_powers = 1;
  ll gm = powmod(g, -m + 2*(p-1));
  rep(i,0,m) {
    if (contains(powers, b)) return i*m + powers[b];
    b = b * gm % p;
  }
  assert(0);
  return -1;
}
