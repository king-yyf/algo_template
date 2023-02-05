# 位运算模版




### 模板代码

```c++
struct Bits {
    int clz(int x) {return x == 0 ? 64 : __builtin_clz(x);} // 二进制中前导0的个数
    int clz(long long x) {return x == 0 ? 64 : __builtin_clzll(x);} // 前导0
    int ctz(int x) {return x == 0 ? 0 : __builtin_ctz(x);} // 二进制中后缀0的个数
    int ctz(long long x) {return x == 0 ? 0 : __builtin_ctzll(x);} // 后缀0 (long long)
    int pct(int x) {return __builtin_popcount(x);} // 二进制中1的个数
    int pct(long long x) {return __builtin_popcountll(x);} // 二进制中1的个数
    int lg(int x) {return x == 0 ? -1 : 31 - __builtin_clz(x);}  // floor(log(2)),x的最高位1
    int clg(int x) {return x <= 1 ? 0 : 32 - __builtin_clz(x - 1);} // ceil(log(2)), minimum x s.t. n <= 2^x
    int ffs(int x) {return __builtin_ffs(x);} // x的最后一位1的是从后向前第几位,eg: 100100, return 3.
    int prt(int x) {return __builtin_parity(x);} //x二进制中1的个数模2的结果
    int prt(long long x) {return __builtin_parityll(x);} //prt long long
    bool get(int x, int i) {return (x & (1 << i)) != 0;} // 获取第i位
    void set(int &x, int i) {x |= (1 << i);}            // 将x第i位设为1
    void reset(int &x, int i) {x &= (~(1 << i));}      //  将x第i位清0
    void flip(int &x) {x = ~x;};        // 翻转x的每一位
    void flip(int &x, int i) {x ^= (1 << i);};        // 翻转第i位
    void flip(int &x, int l, int r) {x ^= makemask(l, r);} // 翻转第l位到第r位
    void resetleft(int &x, int i) {x &= ((1 << i) - 1);} // 清零最高位至第i位(包括第i位和最高位)
    void resetright(int &x, int i) {x &= (-1 << (i + 1));} // 清零第i位至第0位(包括第i位和第0位)
    void set(int &x, int i, int v) { v ? set(x, i) : reset(x, i);} // 将x的第i位设置为v
    bool ispow(int x) {return (x & (x - 1)) == 0;}   // 判断x是否为2的整数次方
    vector<int> gray(int n) {vector<int> a(1<<n); for(int i=0; i<(1<<n); ++i) a[i]=i^(i>>1); return a;}
    int hamming(int x, int y){return pct(x ^ y);} // x,y 二进制的汉明距离
    int setback1(int k) {return (1 << k) - 1;}  // 返回低k位全为1的数(k从1开始)
    int countback1(int x) {return ctz(~x);} //  二进制中后缀1的个数
    int makemask(int l, int r) {return setback1(r + 1) ^ setback1(l);} // 第l位到第r位为1，其余为0的数
} b;

template<typename F> // 按递增顺序枚举二进制包含k个1且小于2^n的数字
void kbits(int n, int k, F &&f) {
    if (k == 0) { f(0); return;}
    for (int s = (1 << k) - 1, x, y; s < 1 << n;) {
        f(s);
        x = __builtin_ctz(s), y = __builtin_ctz(~s >> x);
        s += (1 << x) + (1 << (y - 1)) - 1;
    }
}
template<typename F> // 按递减顺序枚举二进制包含k个1且小于2^n的数字
void kbits_rev(int n, int k, F &&f) {
    if (k == 0) { f(0); return;}
    for (int s = b.makemask(n - k, n - 1), x = 0, y; x != k;) {
        f(s);
        x = __builtin_ctz(~s), s = s ^ ((1 << x) - 1), y = __builtin_ctz(s);
        b.flip(s, y - x - 1, y);
    }
}
template<typename F>  // 递增枚举二进制状态x的所有子集
void submasks(int x, F &&f) {
    for (int s = 0;;s = s ^ (1 << __builtin_ctz(s ^ x) + 1) - 1 & x) { f(s); if (s == x) break;}
}
template<typename F>  // 递减枚举二进制状态x的所有子集
void submasks_rev(int x, F &&f) {
    for (int s = x;;s = (s - 1) & x) { f(s); if (s == 0) break;}
}
template<typename F> // 枚举二进制状态x的所有小于2^n的超集，
void supermasks(int n, int x, F &&f) {
    for (int s = x; s < 1 << n; s = (s + 1) | x) { f(s);}
}
template<typename F>  // 递增枚举某个元素的所有含有 k 个 1 的子状态
void subkbits(int x, int k, F &&f) {
    if (k > __builtin_popcount(x)) return;
    vector<int> res(33);
    for (int i = 0, j = 0; i < 32; ++i)
        if (x & (1 << i)) res[++j] = x & ((1 << (i + 1)) - 1);
    for (int s = res[k], back, a;;) {
        f(s);
        back = b.setback1(b.ctz(s)) & x;
        if (back < (s ^ x)) {
            a = b.ctz(x - s - back);
            int md = b.setback1(a) & s;
            s += (1 << a) + res[b.pct(md) - 1] - md;
        } else break;
    }
}
template<typename F>  // 递减枚举某个元素的所有含有 k 个 1 的子状态
void subkbits_rev(int x, int k, F &&f) {
    if (k > __builtin_popcount(x)) return;
    // TODO
}
```

### 使用说明

见注释
