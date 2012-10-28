#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>

using namespace std;

typedef long long llong;

void extended_gcd(llong a, llong b, llong& lastx, llong& lasty) {
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
}

llong mod_inverse(llong a, llong m) {
  llong lastx, lasty;
  extended_gcd(a, m, lastx, lasty);
  while (lastx < 0)
    lastx += m;
  return lastx;
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

// chinese remainder theorem for coprime moduli
llong chinrem_coprime(llong as[], llong ps[], int len) {
  llong mod = 1;
  for (int i = 0; i < len; ++i) mod *= ps[i];
  llong x[len];
  llong p = 1, y = 0;;
  llong res = x[0] = as[0];
  for (int i = 1; i < len; ++i) {
    y += x[i-1]*p;
    p *= ps[i-1];
    x[i] = (as[i] - y) * mod_inverse(p, ps[i]);
    res = (res + x[i] * p) % mod;
  }
  while (res < 0) res += mod;
  return res;
}

// the below two functions are a port of
// http://www.cse.sc.edu/~maxal/gpscripts/binomod.gp to C++
llong facmodpq(llong n, llong p, llong q) {
  llong pq = 1;
  for (int i = 0; i < q; ++i) pq *= p;

  if (n <= 1) return 1;

  /* r = (-1)^(n\p^k) + O(p^k);
     for(i=2,n%p^k,if(i%p,r*=i)); */
  llong r = ((n/pq) % 2) ? -1 : 1;
  for (int i = 2; i <= n % pq; ++i)
    if (i % p)
      r = (r * i) % pq;
  while (r < 0) r += pq;
  return r;
}

const int maxdigs = 20; // maximum number of digits in base p
int binomod(llong n, llong m, llong p, llong q) {
  int ndigs[maxdigs], mdigs[maxdigs], rdigs[maxdigs], N[maxdigs], M[maxdigs], R[maxdigs], e[maxdigs];
  memset(ndigs, 0, sizeof ndigs);
  memset(mdigs, 0, sizeof mdigs);
  memset(mdigs, 0, sizeof rdigs);
  memset(N, 0, sizeof N);
  memset(M, 0, sizeof M);
  memset(M, 0, sizeof R);
  memset(e, 0, sizeof e);
  int d = 0;
  llong n1 = n, m1 = m, r1 = n-m;
  /* np = vector(d+1,i,(n\p^(i-1))%p);
     kp = vector(d+1,i,(k\p^(i-1))%p);
     rp = vector(d+1,i,((n-k)\p^(i-1))%p); */
  while (n1) {
    ndigs[d] = n1 % p; mdigs[d] = m1 % p; rdigs[d] = r1 % p;
    n1 /= p; m1 /= p; r1 /= p;
    ++d;
  }
  /* e = vector(d+1);
     for(i=1,d+1, e[i]=np[i]<kp[i]+if(i>1,e[i-1]));
     forstep(i=d,1,-1, e[i]+=e[i+1]); */
  for (int i = 0; i < d; i++)
    e[i] = (ndigs[i] < (mdigs[i] + ((i > 0) ? e[i-1] : 0)));
  for (int i = d - 2; i >= 0; i--)
    e[i] += e[i+1];

  /* if( e[1] >= q,
       F[z] = Mod(0,p^q);
       next;
     );
     q -= e[1]; */
  if (e[0] >= q) return 0;

  // compute the old value p^q (which is our top-level modulo)
  llong pq = 1;
  for (int i = 0; i < q; ++i)
    pq *= p;
  q -= e[0];

  /* N = vector(d+1,i,(n\p^(i-1))%p^q);
     K = vector(d+1,i,(k\p^(i-1))%p^q);
     R = vector(d+1,i,((n-k)\p^(i-1))%p^q); */
  n1 = n; m1 = m; r1 = n-m;
  llong pq1 = 1;
  for (int i = 0; i < q; ++i) pq1 *= p;
  d = 0;
  while (n1) {
    N[d] = n1 % pq1; M[d] = m1 % pq1; R[d] = r1 % pq1;
    n1 /= p; m1 /= p; r1 /= p;
    ++d;
  }

  // if( (p>2 || q<3) && q<=#e, v *= (-1)^e[q] ); /* switched with line above */
  llong res = ((p > 2 || q < 3) && q < d && e[q-1] % 2) ? -1 : 1;
  for (int i = 0; i < e[0]; i++)
    res = (res * p) % pq;

  // v = p^e[1] * prod(j=1,d+1, factorialmodp1(N[j],p,q) / factorialmodp1(K[j],p,q) / factorialmodp1(R[j],p,q) )
  for (int i = 0; i < d; i++) {
    llong x = facmodpq(N[i], p, q) % pq;
    x = (x * mod_inverse(facmodpq(M[i], p, q), pq)) % pq;
    x = (x * mod_inverse(facmodpq(R[i], p, q), pq)) % pq;
    res = (res * x) % pq;
  }
  while (res < 0) res += pq;
  return res;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  llong n, r;
  for (int i = 0; i < T; ++i) {
    cin >> n >> r;
    // compute binomial coefficient modulo 142857 = 3^3 * 11 * 13 * 37
    llong a[] = { binomod(n, r, 3, 3), binomod(n, r, 11, 1),
                  binomod(n, r, 13, 1), binomod(n, r, 37, 1) };
    llong p[] = { 27, 11, 13, 37 };
    cout << chinrem_coprime(a, p, 4) << endl;
  }
}
