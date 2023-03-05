# modint

素数取模模板, 模板修改自atcoder, 本地 sublime 代码补全缩写为 `mint`


### 模板代码

```c++
ll inverse(ll a, ll m) { 
    a %= m; if (a == 0) return 0; if (a < 0) a += m;
    ll u = 0, v = 1;
    while (a) { ll t = m / a;  m -= t * a; swap(a, m); u -= t * v; swap(u, v); }
    return u;
}
template <int m, bool is_prime = true>
struct static_mod {
    using mint = static_mod;
    static constexpr int mod() { return m; }
    static_mod() : _v(0) {}
    template <class T> static_mod(T v) {ll x = (ll)(v % (ll)(umod())); if (x < 0) x += umod(); _v = (unsigned int)(x);}
    static_mod(unsigned int v) { _v = (unsigned int)(v % umod());}
    unsigned int val() const { return _v; }
    mint& operator++() { _v++; if (_v == umod()) _v = 0; return *this;}
    mint& operator--() { if (_v == 0) _v = umod(); _v--; return *this;}
    mint operator++(int) { mint result = *this; ++*this; return result;}
    mint operator--(int) { mint result = *this; --*this; return result;}
    mint& operator+=(const mint& rhs) { _v += rhs._v; if (_v >= umod()) _v -= umod();return *this;}
    mint& operator-=(const mint& rhs) { _v -= rhs._v; if (_v >= umod()) _v += umod();return *this;}
    mint& operator*=(const mint& rhs) { unsigned long long z = _v; z *= rhs._v; _v = (unsigned int)(z % umod()); return *this;}
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }
    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }
    mint pow(ll n) const {mint x = *this, r = 1; while (n) { if (n & 1) r *= x; x *= x;n >>= 1;} return r;}
    mint inv() const { if(is_prime) {assert(_v);return pow(umod() - 2);} return inverse(_v, m);}
    friend mint operator+(const mint& lhs, const mint& rhs) { return mint(lhs) += rhs;}
    friend mint operator-(const mint& lhs, const mint& rhs) { return mint(lhs) -= rhs;}
    friend mint operator*(const mint& lhs, const mint& rhs) { return mint(lhs) *= rhs;}
    friend mint operator/(const mint& lhs, const mint& rhs) { return mint(lhs) /= rhs;}
    friend bool operator==(const mint& lhs, const mint& rhs) { return lhs._v == rhs._v;}
    friend bool operator!=(const mint& lhs, const mint& rhs) { return lhs._v != rhs._v;}
    friend ostream& operator << (ostream& out, const mint& n) { return out << n.val(); }
    friend istream& operator >> (istream& in, mint& n) { ll x; in >> x; n = mint(x); return in; }
private:
    unsigned int _v;
    static constexpr unsigned int umod() { return m; }
};
using mint = static_mod<998244353>; // 1000000007
```

### 使用说明

需要定义mod值。

1. 定义mint类型

```c++
const int mod = 998244353;
using mint = static_mod<mod>; 
mint x = 10; // mint x(10);
```

如果 mod 不是素数时，需要设置 `is_prime = false`.

```c++
const int mod = 205;
using mint =  static_mod<mod, is_prime = false>;
```


2. 输入输出

```c++
mint x;
cin >> x;
cout << x << "\n";
```

4. 四则运算

```c++
mint x, y;
x += y;
x -= y;
x *= y;
x = x + 4;
x++;
x /= y;
```

5. 快速幂

```c++
mint x;
mint ans = x.pow(5);
```


### 动态取模模板

```c++
using ull = unsigned long long;
ll inverse(ll a, ll m) { 
    a %= m; if (a == 0) return 0; if (a < 0) a += m;
    ll u = 0, v = 1;
    while (a) { ll t = m / a;  m -= t * a; swap(a, m); u -= t * v; swap(u, v); }
    return u;
}
struct barrett {
    unsigned int _m;
    ull im;
    explicit barrett(unsigned int m) : _m(m), im((ull)(-1) / m + 1) {} // 1 <= m < 2^31
    unsigned int umod() const { return _m; }
    unsigned int mul(unsigned int a, unsigned int b) const { // 0 <= a, b < m, return a * b % m
        ull z = a; z *= b;
        ull x = (ull)(((unsigned __int128)(z)*im) >> 64);
        unsigned int v = (unsigned int)(z - x * _m);
        if (_m <= v) v += _m;
        return v;
    }
};
template <int id> struct dynamic_mod {
    using mint = dynamic_mod;
    static int mod() { return (int)(bt.umod()); }
    static void set_mod(int m) { assert(1 <= m); bt = barrett(m);}
    static mint raw(int v) { mint x; x._v = v; return x;}
    dynamic_mod() : _v(0) {}
    template <class T>
    dynamic_mod(T v) { ll x = (ll)(v % (ll)(mod())); if (x < 0) x += mod(); _v = (unsigned int)(x);}
    dynamic_mod(unsigned int v) { _v = (unsigned int)(v % mod());}
    unsigned int val() const { return _v; }
    mint& operator++() { _v++; if (_v == umod()) _v = 0; return *this;}
    mint& operator--() { if (_v == 0) _v = umod(); _v--; return *this;}
    mint operator++(int) { mint result = *this; ++*this; return result;}
    mint operator--(int) { mint result = *this; --*this; return result;}
    mint& operator+=(const mint& rhs) { _v += rhs._v; if (_v >= umod()) _v -= umod(); return *this;}
    mint& operator-=(const mint& rhs) { _v += mod() - rhs._v; if (_v >= umod()) _v -= umod(); return *this;}
    mint& operator*=(const mint& rhs) { _v = bt.mul(_v, rhs._v); return *this;}
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }
    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }
    mint pow(ll n) const { assert(0 <= n); mint x = *this, r = 1; while (n) { if (n & 1) r *= x; x *= x; n >>= 1;}return r;}
    mint inv() const { return inverse(_v, mod()); }
    friend mint operator+(const mint& lhs, const mint& rhs) { return mint(lhs) += rhs;}
    friend mint operator-(const mint& lhs, const mint& rhs) { return mint(lhs) -= rhs;}
    friend mint operator*(const mint& lhs, const mint& rhs) { return mint(lhs) *= rhs;}
    friend mint operator/(const mint& lhs, const mint& rhs) { return mint(lhs) /= rhs;}
    friend bool operator==(const mint& lhs, const mint& rhs) { return lhs._v == rhs._v;}
    friend bool operator!=(const mint& lhs, const mint& rhs) { return lhs._v != rhs._v;}
private:
    unsigned int _v;
    static barrett bt;
    static unsigned int umod() { return bt.umod(); }
};
template <int id> barrett dynamic_mod<id>::bt(998244353);
using modint = dynamic_mod<998244353>;
```

### 使用说明

方法和 static_mod 基本一致，支持动态 set_mod

1. 设置模数

```c++
const int mod = 1e9 + 7;
modint x = 10;
modint::set_mod(mod);
```

