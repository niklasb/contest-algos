#include <bits/stdc++.h>
using namespace std;

using ll=long long;
using ull=unsigned long long;
#define rep(i,s,e) for(int i=(s);i<(e);++i)
#include "../number_theory.cpp"

int main() {
  for (int a = 1; a <= 10000; ++a) {
    map<ll,int> facs;
    factor(a, facs);
    ll x = 1;
    for (auto it: facs) {
      assert(rabin(it.first));
      x *= powmod(it.first, it.second, a+1);
    }
    assert(x == a);
  }
}
