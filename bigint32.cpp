#include <bits/stdc++.h>
using namespace std;

const int basesz=32;
const uint64_t base=1ull<<basesz;

string binary(uint64_t x, int min_width = 1) {
  string res;
  while ((min_width--) > 0 || x) {
    res += '0' + (x&1);
    x>>=1;
  }
  reverse(res.begin(), res.end());
  return res;
}

struct BigInt {
  vector<uint32_t> digs;
  BigInt(uint64_t x=0) { while (x) digs.push_back(x%base), x /= base; }
  BigInt(const BigInt& other) : digs(other.digs) {}
  void operator=(const BigInt& other) { digs = other.digs; }

  void normalize() {
    while (digs.size() && !digs.back()) digs.pop_back();
  }

  bool zero() { return digs.empty(); }

  uint64_t value() {
    uint64_t res = 0;
    for (auto it = digs.rbegin(); it != digs.rend(); ++it)
      res = res * base + *it;
    return res;
  }

  void add(int i, uint64_t x) {
    if (!x) return;
    if (i >= (int)digs.size())
      digs.resize(i + 1);
    uint64_t y = uint64_t{digs[i]} + x;
    if (y < base) { digs[i] = y; return; }
    digs[i] = y % base;
    assert(digs[i] < base);
    add(i + 1, y / base);
  }

  BigInt operator++() {
    BigInt old(*this);
    add(0,1);
    return old;
  }

  BigInt operator+(const BigInt& other)  const {
    BigInt res(other);
    for (size_t i = 0; i < digs.size(); ++i)
      res.add(i, digs[i]);
    res.normalize();
    return res;
  }

  BigInt operator*(const BigInt& other) const {
    BigInt res;
    for (size_t i = 0; i < digs.size(); ++i)
      for (size_t j = 0; j < other.digs.size(); ++j) {
        assert(digs[i] < base);
        assert(other.digs[j] < base);
        res.add(i + j, 1ll * digs[i] * other.digs[j]);
      }
    res.normalize();
    return res;
  }

  BigInt operator>>(int shift) const {
    assert(shift < basesz);
    BigInt res;
    res.digs.resize(digs.size());
    for (size_t i = 0; i < digs.size(); ++i) {
      uint32_t& cur = res.digs[i];
      cur = digs[i]>>shift;
      if (i + 1 < digs.size())
        cur |= (uint64_t{digs[i+1]} << (basesz - shift)) & (base - 1);
    }
    res.normalize();
    return res;
  }

  bool operator<(const BigInt& other) const {
    if (digs.size() != other.digs.size())
      return digs.size() < other.digs.size();
    return lexicographical_compare(digs.rbegin(), digs.rend(),
                                   other.digs.rbegin(), other.digs.rend());
  }

  bool operator==(const BigInt& other) const {
    return !(*this < other) && !(other < *this);
  }

  pair<BigInt, BigInt> divmod(const BigInt& other) const {
    BigInt lo(0), hi(*this + 1); // here, hi is exclusive
    while (lo + 1 < hi) {
      BigInt mid = (lo + hi)>>1;
      //cout << " lo=" << lo << " hi=" << hi << " mid=" << mid << endl;
      //cout << "    " << (*this) << " < " << (mid*other) << " = " << (*this<mid*other) << endl;
      if (*this < mid * other) hi = mid;
      else lo = mid;
    }
    BigInt div = lo;
    BigInt tmp = div * other;
    lo = 0; hi = other;
    while (lo < hi) {
      BigInt mid = (lo + hi)>>1;
      if (tmp + mid < *this) lo = mid + 1;
      else hi = mid;
    }
    return {div, lo};
  }

  BigInt operator/(const BigInt& other) const { return divmod(other).first; }
  BigInt operator%(const BigInt& other) const { return divmod(other).second; }

  BigInt powmod(BigInt exp, BigInt mod) const {
    BigInt res(1), base(*this);
    res = res % mod;
    for (auto chunk : exp.digs)
      for (int i = 0; i < basesz; ++i) {
        if ((chunk >> i) & 1)
          res = (res * base) % mod;
        base = (base * base) % mod;
      }
    return res;
  }

  string str(int base=10) const {
    const char* alph = "0123456789abcdef";
    assert(0 <= base && base <= 16);

    BigInt bigbase(base), tmp(*this);
    string res;
    while (!tmp.zero()) {
      res += alph[(tmp % bigbase).value()];
      tmp = tmp / base;
    }
    if (res.empty()) res += '0';
    reverse(res.begin(), res.end());
    return res;
  }

  string bin() const { return str(2); }
  string hex() const { return str(16); }

  static BigInt parse(const string& s, int base=10) {
    const char* alph = "0123456789abcdef";
    BigInt res(0), basebig(base);
    for (char c: s) {
      c = tolower(c);
      const char* p = strchr(alph, c);
      assert(p);
      res = res * basebig + BigInt(p - alph);
    }
    return res;
  }
};

ostream& operator<<(ostream& out, const BigInt& x) {
  return out << x.str();
}

void test(int ax, int bx, int cx) {
  BigInt a(ax);
  BigInt b(bx);
  BigInt c(cx);

  cout << "assert ";
  cout << a << " * ";
  cout << b << " == ";
  cout << a*b << endl;

  cout << "assert ";
  cout << a << " + ";
  cout << b << " == ";
  cout << a+b << endl;

  cout << "assert ";
  cout << a << " >> 1 == ";
  cout << (a>>1) << endl;

  cout << "assert ";
  cout << a << " / ";
  cout << b << " == ";
  cout << a/b << endl;

  cout << "assert ";
  cout << a << " % ";
  cout << b << " == ";
  cout << a%b << endl;

  cout << "assert ";
  cout << "pow(" << a << ", ";
  cout << c << ", ";
  cout << b << ") == ";
  cout << a.powmod(c, b) << endl;
}

using timer = chrono::high_resolution_clock;

int main() {
  BigInt p = BigInt::parse(
      "d65d758b1c9d035fe54550ff437143d633f0284a4551318d79710c5a9230b55f", 16);
  BigInt q = BigInt::parse(
      "1630b16242196ebaf50ebcd4a7da00eb6c49b6f69c6d11e5af4e13c48d0b9891", 16);
  BigInt n = p*q;

  assert(n.hex() ==
    "1294ce28e3f06deb70b2c67b6475a4524822b58aba45c62eb97b8bcc17e370af"
    "3f74266f653251c1f42d9a931bce67270db72780b0711ee61179cbd2d75c22cf");

  BigInt d = BigInt::parse(
    "b59e4678e77fe6e6a368c922cd8c13d92dbc153b25f95a64da16fd18a76630bd"
    "b90adfa5cc3a7e9891d21a25a9c8192bcc46032baaf00e8bdcbebc0aaa6a781", 16);
  BigInt e(0x010001);

  BigInt m = BigInt::parse("666f6f626172", 16);
  BigInt c = m.powmod(e, n);

  auto start = timer::now();

  assert(m == c.powmod(d, n));

  uint64_t msecs = chrono::duration_cast<chrono::microseconds>(
      timer::now() - start).count();
  cout << "powmod time = " << setprecision(2) << fixed << 1.*msecs/1e6 << endl;

  /*
  for (int ax = 0; ax <= 100; ++ax) {
    for (int bx = 1; bx <= 100; ++bx) {
      for (int cx = 0; bx <= 100; ++bx) {
        test(ax, bx, cx);
      }
    }
  }
  */
}
