# 数论


===

Index
---
<!-- TOC -->
- 
- [素数](#素数)
  - [素数判定](#素数判定)
  - [埃氏筛素数](#埃氏筛素数)
  - [线性素数筛](#线性素数筛)
  - [素数原根](#素数原根)
  - [统计n以内的素数个数](#统计n以内的素数个数)
  - [5e8量级素数筛](#5e8量级素数筛)
- [分解质因数](#分解质因数)
- [1e18量级质因数分解](#1e18量级质因数分解)
- [64位整数相乘](#64位整数相乘)
- [欧拉函数](#欧拉函数)
  - [求x的欧拉函数](#求x的欧拉函数)
  - [筛法求欧拉函数](#筛法求欧拉函数)
- [约数个数和约数之和](#约数个数和约数之和)
- [筛法求约数个数](#筛法求约数个数)
- [筛法求约数和](#筛法求约数和)
- [筛法求莫比乌斯函数](#筛法求莫比乌斯函数)
- [裴蜀定理与扩展欧几里德](#裴蜀定理与扩展欧几里德)
- [中国剩余定理](#中国剩余定理)
- [组合数](#组合数)


<!-- /TOC -->


## 素数


### 素数判定


```c++
bool is_prime(int x) {
    if (x <= 2) return x == 2;
    if (x % 2 == 0) return false;
    for (int i = 3, t = sqrt(x); i <= t; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}
```

### 埃氏筛素数

- 时间复杂度：O(nloglogn)

```c++
const int MXN = 1e5 + 5;
int primes[MXN], cnt; // primes[]存储所有素数
bool st[MXN];   //st[x]存储x是否被筛掉
void sieve(int n) {
    for (int i = 2; i <= n; ++i) {
        if (st[i]) continue;
        primes[cnt ++ ] = i;
        for (int j = 2 * i; j <= n; j += i)
            st[j] = true;
    }
}
```

### 线性素数筛

```c++
const int MXN = 1e5 + 5;
int primes[MXN], prime_cnt; 
bool st[MXN];
void sieve(int n) {  // 筛小于等于n的所有素数 MXN 要大于n
    for (int i = 2; i <= n; ++i) {
        if (!st[i]) primes[prime_cnt++] = i;
        for (int j = 0; primes[j] * 1ll * i <= n; ++j) {
            st[primes[j] * i] = true;
            if (i % primes[j] == 0) break;
        }
    }
}
```

### 素数原根

给定质数p, 如果数x满足 `(0<x<p)`, 当 i取1到p-1时，x的i次方模p构成的集合恰好为1到p-1, 则称x为质数p的一个原根。

结论： **一个质数p的原根个数等于p-1的欧拉函数**

```c++
long long safe_mod(long long x, long long m) { x %= m; if (x < 0) x += m; return x;}
long long pow_mod(long long x, long long n, int m) {  // 0 <= n; 1 <= m, return (x ^ n) % m
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1,y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}
int prime_root(int m) {
    if (m == 2) return 1;
    if (m == 167772161 || m == 469762049 || m == 998244353) return 3;
    if (m == 754974721) return 11;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1, x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) x /= i;
        }
    }
    if (x > 1) divs[cnt++] = x;
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
```

### 统计n以内的素数个数

[counting primes](https://judge.yosupo.jp/problem/counting_primes)

输入整数n，统计1-n之间的素数个数。

+ 1 <= n <= 1e11

```c++
__attribute__((target("avx2"), optimize("O3", "unroll-loops")))
long long prime_counting(long long n) {
    constexpr int p[10]{2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    for (int i = 0; i < 10; ++i) if (n < p[i]) 
        return i;
    const int v =  sqrtl(n);
    int s = (v + 1) / 2;
    vector<int> smalls(s), roughs(s);
    vector<long long> larges(s);
    for (int i = 0; i < s; ++i) 
        smalls[i] = i, roughs[i] = 2 * i + 1, larges[i] = (n / (2 * i + 1) - 1) / 2;
    vector<bool> skip(v + 1);
    const auto divide = [](ll n, ll d) -> int { return (double)n / d;};
    const auto half = [](int n) -> int { return (n - 1) >> 1;};
    int pc = 0;
    for (int p = 3; p <= v; p += 2) if (!skip[p]) {
        int q = p * p;
        if ((ll)q * q > n) break;
        skip[p] = true;
        for (int i = q; i <= v; i += 2 * p) skip[i] = true;
        int ns = 0;
        for (int k = 0; k < s; k++) {
            int i = roughs[k];
            if (skip[i]) continue;
            ll d = (ll)i * p;
            larges[ns] = larges[k] - (d <= v ? larges[smalls[d >> 1] - pc] : smalls[half(divide(n, d))]) + pc;
            roughs[ns++] = i;
        }
        s = ns;
        for (int i = half(v), j = ((v / p) - 1) | 1; j >= p; j -= 2) {
            int c = smalls[j >> 1] - pc;
            for (int e = (j * p) >> 1; i >= e; i--) smalls[i] -= c;
        }
        pc++;
    }
    larges[0] += (ll)(s + 2 * (pc - 1)) * (s - 1) / 2;
    for (int k = 1; k < s; k++) larges[0] -= larges[k];
    for (int l = 1; l < s; l++) {
        ll q = roughs[l], m = n / q;
        int e = smalls[half(m / q)] - pc;
        if (e < l + 1) break;
        ll t = 0;
        for (int k = l + 1; k <= e; k++)
            t += smalls[half(divide(m, roughs[k]))];
        larges[0] += t - (ll)(e - l) * (pc + l - 1);
    }
    return larges[0] + 1;
}
```

### 5e8量级素数筛

[Enum primes](https://judge.yosupo.jp/problem/enumerate_primes)

输入N, A, B, 求1-N内质数个数，同时输出 p[B], p[B + A], ..., p[B + kA], p[B + kA] <= N.

+ 1 <= N <= 5e8
+ 0 <= B < A <= N
+ 保证输出质数的数量不超过1e6

```c++
vector<int> prime_sieve(const int N, const int Q = 17, const int L = 1<<15) {
    using u8 = unsigned char;
    constexpr int rs[] = {1, 7, 11, 13, 17, 19, 23, 29};
    struct P { 
        P(int p) : p(p) {}
        int p; int pos[8];
    };
    auto approx_prime_count = [] (const int N) -> int {
        return N > 60184 ? N / (log(N) - 1.1) : max(1., N / (log(N) - 1.11)) + 1;
    };
    const int v = sqrt(N), vv = sqrt(v);
    vector<bool> isp(v + 1, true);
    for (int i = 2; i <= vv; ++i) if (isp[i]) {
        for (int j = i * i; j <= v; j += i) isp[j] = false;
    }

    const int rsize = approx_prime_count(N + 30), M = (N + 29) / 30;;
    vector<int> primes = {2, 3, 5}; 
    primes.reserve(rsize);

    vector<P> sprimes; size_t pbeg = 0;
    int prod = 1; 
    for (int p = 7; p <= v; ++p) {
        if (!isp[p]) continue;
        if (p <= Q) prod *= p, ++pbeg, primes.push_back(p);
        auto pp = P(p); 
        for (int t = 0; t < 8; ++t) {
            int j = (p <= Q) ? p : p * p;
            while (j % 30 != rs[t]) j += p << 1;
            pp.pos[t] = j / 30;
        }
        sprimes.push_back(pp);
    }

    vector<u8> pre(prod, 0xFF);
    for (size_t pi = 0; pi < pbeg; ++pi) {
        auto pp = sprimes[pi]; const int p = pp.p;
        for (int t = 0; t < 8; ++t) {
            const u8 m = ~(1 << t);
            for (int i = pp.pos[t]; i < prod; i += p) pre[i] &= m;
        }
    }

    const int block_size = (L + prod - 1) / prod * prod;
    vector<u8> block(block_size); u8* pblock = block.data();

    for (int beg = 0; beg < M; beg += block_size, pblock -= block_size) {
        int end = min(M, beg + block_size);
        for (int i = beg; i < end; i += prod) 
            copy(pre.begin(), pre.end(), pblock + i);
        if (beg == 0) pblock[0] &= 0xFE;
        for (size_t pi = pbeg; pi < sprimes.size(); ++pi) {
            auto& pp = sprimes[pi];
            const int p = pp.p;
            for (int t = 0; t < 8; ++t) {
                int i = pp.pos[t]; 
                const u8 m = ~(1 << t);
                for (; i < end; i += p) pblock[i] &= m;
                pp.pos[t] = i;
            }
        }
        for (int i = beg; i < end; ++i) {
            for (int m = pblock[i]; m > 0; m &= m - 1) 
                primes.push_back(i * 30 + rs[__builtin_ctz(m)]);
        }
    }
    while (primes.size() && primes.back() > N) primes.pop_back();
    return primes;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, a, b;
    cin >> n >> a >> b;
    auto primes = prime_sieve(n);
    int c = primes.size(), m = (c - b + a - 1) / a;
    cout << c << ' ' << m << '\n';
    for (int i = b; i < c; i += a) {
        cout << primes[i] << " \n"[i + a >= c];
    } 
    
    return 0;
}
```


## 分解质因数

```c++
vector<pair<int,int>> getDivisors(int x) {
    vector<pair<int,int>> res;
    for (int i = 2, t = sqrt(x); i <= t; ++i) {
        if (x % i == 0) {
            int s = 0;
            while (x % i == 0) 
                x /= i, s++;
            res.emplace_back(i,s);
        }
    }
    if (x > 1) res.emplace_back(x,1);
    return res;
}
```

### 1e18量级质因数分解

[factorize](https://judge.yosupo.jp/problem/factorize)

q个查询，每个查询，输入a，输出a的质因数个数及a的所有质因数。
例如：输入 8， 输出 `3 2 2 2`, 3个质因数，分别为 `2 2 2`.

+ 1 <= a <= 1e18

```c++
namespace fast_factorize {
  struct m64 {
    using i64 = int64_t;
    using u64 = uint64_t;
    using u128 = __uint128_t;
    inline static u64 m, r, n2; // r * m = -1 (mod 1<<64), n2 = 1<<128 (mod m)
    static void set_mod(u64 m) {
        assert(m < (1ull << 62));
        assert((m & 1) == 1);
        m64::m = m; n2 = -u128(m) % m; r = m;
        for (int _ = 0; _ < 5; ++ _) r *= 2 - m*r;
        r = -r;
        assert(r * m == -1ull);
    }
    static u64 reduce(u128 b) { return (b + u128(u64(b) * r) * m) >> 64; }
    u64 x;
    m64() : x(0) {}
    m64(u64 x) : x(reduce(u128(x) * n2)){};
    u64 val() const { u64 y = reduce(x); return y >= m ? y-m : y; }
    m64 &operator+=(m64 y) { x += y.x - (m << 1); x = (i64(x) < 0 ? x + (m << 1) : x);return *this; }
    m64 &operator-=(m64 y) {x -= y.x;x = (i64(x) < 0 ? x + (m << 1) : x);return *this;}
    m64 &operator*=(m64 y) { x = reduce(u128(x) * y.x); return *this; }
    m64 operator+(m64 y) const { return m64(*this) += y; }
    m64 operator-(m64 y) const { return m64(*this) -= y; }
    m64 operator*(m64 y) const { return m64(*this) *= y; }
    bool operator==(m64 y) const { return (x >= m ? x-m : x) == (y.x >= m ? y.x-m : y.x); }
    bool operator!=(m64 y) const { return not operator==(y); }
    m64 pow(u64 n) const {
        m64 y = 1, z = *this;
        for ( ; n; n >>= 1, z *= z) if (n & 1) y *= z;
        return y;
    }
  };
  constexpr int lg(int x) {return x == 0 ? -1 : 31 - __builtin_clz(x);}  
  mt19937_64 rng_mt{random_device{}()};
  ll rnd(ll n) { return uniform_int_distribution<ll>(0, n - 1)(rng_mt); }
  bool primetest(const uint64_t x) {
      using u64 = uint64_t;
      if (x == 2 or x == 3 or x == 5 or x == 7) return true;
      if (x % 2 == 0 or x % 3 == 0 or x % 5 == 0 or x % 7 == 0) return false;
      if (x < 121) return x > 1;
      const u64 d = (x-1) >> __builtin_ctzll(x-1);
      m64::set_mod(x);
      const m64 one(1), minus_one(x-1);
      auto ok = [&](u64 a) {
          auto y = m64(a).pow(d);
          u64 t = d;
          while (y != one and y != minus_one and t != x-1) y *= y, t <<= 1;
          if (y != minus_one and t % 2 == 0) return false;
          return true;
      };
      if (x < (1ull << 32)) {
          for (u64 a : { 2, 7, 61 }) if (not ok(a)) return false;
      } else {
          for (u64 a : { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 }) {
              if (x <= a) return true;
              if (not ok(a)) return false;
          }
      }
      return true;
  }
  ll rho(ll n, ll c) {
    m64::set_mod(n);
    assert(n > 1);
    const m64 cc(c);
    auto f = [&](m64 x) { return x * x + cc; };
    m64 x = 1, y = 2, z = 1, q = 1;
    ll g = 1;
    const ll m = 1LL << (lg(n) / 5); // ?
    for (ll r = 1; g == 1; r <<= 1) {
      x = y;
      for (int _ = 0; _ < r; ++ _) y = f(y);
      for (ll k = 0; k < r and g == 1; k += m) {
        z = y;
        for (int _ = 0, t = min(m, r - k); _ < t; ++ _)
          y = f(y), q *= x - y;
        g = gcd(q.val(), n);
      }
    }
    if (g == n)
      do {
        z = f(z);
        g = gcd((x - z).val(), n);
      } while (g == 1);
    return g;
  }
  ll find_prime_factor(ll n) {
    assert(n > 1);
    if (primetest(n)) return n;
    for (int _ = 0; _ < 100; ++ _) {
      ll m = rho(n, rnd(n));
      if (primetest(m)) return m;
      n = m;
    }
    cerr << "failed" << endl;
    assert(false);
    return -1;
  }
  vector<pair<ll, int>> factor(ll n) {
    assert(n >= 1);
    vector<pair<ll, int>> pf;
    for (int p = 2; p < 100; ++ p) {
      if (p * p > n) break;
      if (n % p == 0) {
        int e = 0;
        do {
          n /= p, e += 1;
        } while (n % p == 0);
        pf.emplace_back(p, e);
      }
    }
    while (n > 1) {
      ll p = find_prime_factor(n);
      int e = 0;
      do {
        n /= p, e += 1;
      } while (n % p == 0);
      pf.emplace_back(p, e);
    }
    sort(pf.begin(), pf.end());
    return pf;
  }
} // namespace fast_factorize

// auto p =  fast_factorize::factor(x);
```

## 64位整数相乘

```c++
ll mul64(ll a, ll b, ll p) { // 64位整数相乘
    ll ans = 0;
    for (; b; b >>= 1) {
        if (b & 1) ans = (ans + a) % p;
        a = (ll) a * 2 % p;
    }
    return ans;
}
```


## 欧拉函数


### 求x的欧拉函数

```
定义：1 ~ N中 与 N 互质的数的个数被称为欧拉函数，记为ϕ(N)
若 N = p1^a1 * p2^a2 * ... * pm^am 则：
ϕ(N) = N * (p1-1)/p1 * (p2-1)/p2 * ... * (pm-1)/pm
```

```c++
int phi(int x) {  //欧拉函数(1-n中与n互质的数)
    int res = x;
    for (int i = 2, t = sqrt(x); i <= t; i ++ )
        if (x % i == 0) {
            res = res / i * (i - 1);
            while (x % i == 0) x /= i;
        }
    if (x > 1) res = res / x * (x - 1);
    return res;
}
```

### 筛法求欧拉函数

```c++
const int MXN = 1e5 + 5;
int euler[MXN], prime_cnt; 
bool st[MXN];
void sieve_eulers(int n) { // 筛法求欧拉函数
    euler[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!st[i]) {
            primes[prime_cnt++] = i;
            euler[i] = i - 1;
        }
        for (int j = 0; primes[j] * 1ll * i <= n; ++j) {
            int t = primes[j] * i;
            st[t] = true;
            if (i % primes[j] == 0) {
                euler[t] = euler[i] * primes[j];
                break;
            }
            euler[t] = euler[i] * (primes[j] - 1);
        }
    }
}
```

## 约数个数和约数之和

```
如果 N = p1^c1 * p2^c2 * ... *pk^ck
约数个数： (c1 + 1) * (c2 + 1) * ... * (ck + 1)
约数之和： (p1^0 + p1^1 + ... + p1^c1) * ... * (pk^0 + pk^1 + ... + pk^ck)
```

## 筛法求约数个数

给定一个n， 输出1-n每个数的约数个数。

+ 1 <= n <= 1e6

```c++
const int MXN = 1e6 + 5;
int primes[MXN], prime_cnt; 
bool st[MXN];
int a[MXN], d[MXN]; //a[i]记录i的最小质因子的次数，d[i]记录i的约数个数
void count_divs(int n) {
    d[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!st[i]) {
            primes[prime_cnt++] = i;
            a[i] = 1, d[i] = 2;
        }
        for (int j = 0; primes[j] * 1ll * i <= n; j++) {
            int t = primes[j] * i;
            st[t] = true;
            if (i % primes[j] == 0) {
                a[t] = a[i] + 1;
                d[t] = d[i] / a[t] * (a[t] + 1);
                break;
            } else a[t] = 1, d[t] = d[i] * 2;
        }
    }
}
```

## 筛法求约数和

给定一个n， 输出1-n每个数的约数和。

1 <= n <= 1e6


```c++
const int MXN = 1e6 + 5;
int primes[MXN], prime_cnt; 
bool st[MXN];
int g[MXN], f[MXN]; //f[i]记录i的约数和
void count_divs(int n) {
    g[1] = f[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!st[i]) {
            primes[prime_cnt++] = i;
            g[i] = g[i] = i + 1;
        }
        for (int j = 0; primes[j] * 1ll * i <= n; j++) {
            int t = primes[j] * i;
            st[t] = true;
            if (i % primes[j] == 0) {
                g[t] = g[i] * primes[j] + 1;
                f[t] = f[i] / g[i] * g[t];
                break;
            } else g[t] = primes[j] + 1, f[t] = f[i] * g[t];
        }
    }
}
```

## 筛法求莫比乌斯函数

莫比乌斯函数定义

+ n = 1 时, mu(n) = 1
+ 当n含有相同质因子时， mu(n) = 0
+ n 不含相同质因子时，mu(n)=(-1)^s， 其中s为n的不同质因子的个数

```c++
const int MXN = 1e6 + 5;
int primes[MXN], prime_cnt; 
bool st[MXN];
int mu[MXN]; 
void count_divs(int n) {
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!st[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; primes[j] * 1ll * i <= n; j++) {
            int t = primes[j] * i;
            st[t] = true;
            if (i % primes[j] == 0) {
                mu[t] = 0
                break;
            } else mu[t] = -mu[j];
        }
    }
}
```

## 裴蜀定理与扩展欧几里德

裴蜀定理： 一定存在整数x,y,满足 `ax+by=gcd(a,b)`

推广： 一定存在整数x1,..xn,满足 `a1x1+a2x2+...anxn=gcd(a1,a2,...,an)`


**扩展欧几里德**

给定 a,b, 求 x,y 使得 ax + by = gcd(a, b)

```c++
int exgcd(int a, int b, int &x, int &y) { //求x, y，st.ax + by = gcd(a, b)
    if (!b) {
        x = 1; y = 0;
        return a;
    }
    int d = exgcd(b, a % b, y, x);
    y -= (a/b) * x;
    return d;
}
```

## 中国剩余定理

给定长度为n的模数数组 mods, 和余数数组 rem, 其中模数 mods[1],..mods[n]为两两互质的整数，求x的最小非负整数解，其中x满足 x % mods[i] = rem[i]

```c++
long long safe_mod(long long x, long long m) { x %= m; if (x < 0) x += m; return x;}
pair<ll, ll> inv_gcd(ll a, ll b) { 
    a %= b; if (a == 0) return {b, 0}; if (a < 0) a += b;
    ll s = b, u = 0, v = 1;
    while (a) { ll t = s / a; s -= a * t, u -= v * t; swap(s, a); swap(u, v);}
    if (u < 0) u += b / s;
    return {s, u};
}
pair<ll, ll> crt(vector<ll>& r, vector<ll>& m) { // pair.first 即为答案
    assert(r.size() == m.size());
    int n = int(r.size());
    long long r0 = 0, m0 = 1;
    for (int i = 0; i < n; i++) {
        assert(1 <= m[i]);
        long long r1 = safe_mod(r[i], m[i]), m1 = m[i];
        if (m0 < m1) { swap(r0, r1);swap(m0, m1);}
        if (m0 % m1 == 0) { if (r0 % m1 != r1) return {0, 0}; continue;}
        long long g, im;
        tie(g, im) = inv_gcd(m0, m1);
        long long u1 = (m1 / g);
        if ((r1 - r0) % g) return {0, 0};
        long long x = (r1 - r0) / g % u1 * im % u1;
        r0 += x * m0;
        m0 *= u1;  // -> lcm(m0, m1)
        if (r0 < 0) r0 += m0;
    }
    return {r0, m0};
}
```

### 组合数

```c++
struct Comb {
    vector<mint> fac, inv;
    vector<vector<mint>> s1, s2;
    Comb() : fac(1, 1), inv(1, 1){}
    void reserve(int a){
        if(fac.size() >= a) return;
        if(a < fac.size() * 2) a = fac.size() * 2;
        while(fac.size() < a) fac.push_back(fac.back() * mint(fac.size()));
        inv.resize(fac.size());
        inv.back() = fac.back().inv();
        for(int i = inv.size() - 1; inv[i - 1] == 0; i--) inv[i - 1] = inv[i] * i;
    }
    mint fact(int n){ if(n < 0) return 0; reserve(n + 1); return fac[n]; }
    mint invfac(int n) {if(n < 0) return 0; reserve(n + 1); return inv[n]; }
    mint P(int n, int r){ // 排列数
        if(r < 0 || n < r) return 0;
        if(n >> 24){ mint ans = 1; for(int i = 0; i < r; i++) ans *= n--; return ans; }
        reserve(n + 1); return fac[n] * inv[n - r];
    }
    mint Q(int n) {reserve(n); return fac[n - 1];} // n个元素的圆排列数
    mint C(int n, int m) { // 组合数
        if (n < m || m < 0) return 0;
        reserve(n + 1);
        return fac[n] * inv[m] * inv[n - m];
    }
    mint MC(int n, int m) { // r个相同物品放到n个篮子方案数
        return C(n + m - 1, n - 1);
    }
    mint catalan(int n) {
        reserve(n * 2 + 1); return fac[n * 2] * inv[n] * inv[n + 1];
    }
    mint CQ(int n, int m) {  // n个元素中选m个元素围成一圈的圆排列数。
        return C(n, m) * Q(m);
    }
    mint D(int n) { // 1-n的排列p的错位排列数  D[n]=(n-1)*(D[n-1]+D[n-2])
        if (n <= 3) return mint(n - 1);
        mint x = 1, y = 2, ans;
        for (int i = 4; i <= n; ++i) {
            ans = (i - 1) * (x + y);
            x = y, y = ans;
        }
        return ans;
    }
    mint stl1(int n, int m) { //n个不同元素，划分为m个非空圆排列的方案数(第一类斯特林数)
        if (s1.size() == 0) {
            int N = 10, M = 5;  // 根据题目数据范围调整
            s1.assign(N, vector<mint>(M));
            s1[0][0] = 1;
            for (int i = 1; i < N; ++i) for (int j = 1; j < M; ++j) {
                s1[i][j] = s1[i - 1][j - 1] + (i - 1) * s1[i - 1][j];
            }
        }
        return s1[n][m];
    }
    mint stl2(int n, int m) {// n个不同元素，划分为m个非空子集的方案数(第二类斯特林数)
        if (s2.size() == 0) {
            int N = 10, M = 5;  // 根据题目数据范围调整
            s2.assign(N, vector<mint>(M));
            s2[0][0] = 1;
            for (int i = 1; i < N; ++i) for (int j = 1; j < M; ++j) {
                s2[i][j] = s2[i - 1][j - 1] + j * s2[i - 1][j];
            }
        }
        return s2[n][m];
    }
} comb;
```