# 数论


===

Index
---
<!-- TOC -->

- 素数
  - 素数判定
  - 线性素数筛
  - 素数原根
- 分解质因数
- 64位整数相乘
- 欧拉函数
  - 求x的欧拉函数
  - 筛法求欧拉函数
- 约数个数和约数之和
- 筛法求约数个数
- 筛法求约数和
- 筛法求莫比乌斯函数
- 裴蜀定理与扩展欧几里德
- 中国剩余定理
- 组合数


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