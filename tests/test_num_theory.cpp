#include <bits/stdc++.h>
using namespace std;

using ll=long long;
using ull=unsigned long long;
#define rep(i,s,e) for(int i=(s);i<(e);++i)
#include "../number_theory.cpp"

int main() {
  for (int a = 0; a <= 100; ++a)
    for (int m = 1; m <= 100; ++m) {
      auto c = pow_cycle(a, m);
      int i = c.first;
      ll C = c.second;
      for (int j = i; j <= i+100; ++j) {
        assert(powmod(a, j, m) == powmod(a, j+C, m));
        assert(powmod(a, j, m) == powmod(a, (j-i) % C + i, m));
      }
    }
}
