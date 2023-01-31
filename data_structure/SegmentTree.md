# 线段树

模板取自 atcoder library

[atcoder doc](https://atcoder.github.io/ac-library/master/document_en/segtree.html)

[oi wiki介绍](https://oi-wiki.org/ds/seg/)

### 模板代码

```c++
template <class S, S (*op)(S, S), S (*e)()>
struct SegTree {
    int n, size, log;
    vector<S> d;
    SegTree() : SegTree(0) {}
    explicit SegTree(int n) : SegTree(vector<S>(n, e())) {}
    explicit SegTree(const vector<S> &v) : n(int(v.size())) {
        log = ceil_lg(n), size = 1 << log;
        d = vector<S>(2 * size, e());
        for (int i = 0; i < n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) pull(i);
    }
    int ceil_lg(int n) {   // minimum non-neg x s.t. `n <= 2^x`
        int x = n == 0 ? 0 :  32 - __builtin_clz(n);
        return  __builtin_popcount(n) == 1 ? x - 1: x;
    }
    void set(int p, S x) {  // assert(0 <= p < n)
        p += size, d[p] = x;
        for (int i = 1; i <= log; ++i) pull(p >> i);
    }
    S get(int p) const { return d[p + size];}
    S get(int l, int r) {   // [l, r)
        S sl = e(), sr = e();
        l += size, r += size;
        while (l < r) {
            if (l & 1) sl = op(sl, d[l++]);
            if (r & 1) sr = op(d[--r], sr);
            l >>= 1, r >>= 1;
        }
        return op(sl, sr);
    }
    S get_all() const {return d[1];}

    void pull(int k) { d[k] = op(d[2 * k], d[2 * k + 1]);}
    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F>     // 0 <= l <= n, f(e()) is true
    int max_right(int l, F f) const { 
        if (l == n) return n;
        l += size;
        S x = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(x, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(x, d[l]))) {
                        x = op(x, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            x = op(x, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }
    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F>  // 0 <= r <= n, f(e()) is true
    int min_left(int r, F f) const {
        if (r == 0) return 0;
        r += size;
        S x = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], x))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], x))) {
                        x = op(d[r], x);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            x = op(d[r], x);
        } while ((r & -r) != r);
        return 0;
    }
};
```

### 使用说明

1.构造函数

```c++
SegTree<S, op, e> seg(int n)
SegTree<S, op, e> seg(vector<S> v)
```

需要定义
- S 类型
- S op(S a, S, b) 二元操作
- S e() 单位元素

例如，对于整型类型的区间最小值，定义方式如下

```c++
int op(int a, int b){
    return min(a, b);
}

int e() {
    //INF ,满足 op(e(), a) = op(a, e()) = a,  min(a, e()) = a
    return (int)1e9; 
}

SegTree<int, op, e> seg(10); // 初始化长度为10数组，每个元素都是 e()
```

- 约束： 0 <= n <= 1e8
- 时间复杂度 O(n)

2. set()函数

将 x 赋值给 a[p]

```c++
void seg.set(int p, int x)
```
- 约束: 0 <= p < n
- 时间复杂度 O(logn)

3.get函数

返回元素 a[p]

```c++
S seg.get(int p)
```
- 约束: 0 <= p < n
- 时间复杂度 O(1)

3.区间查询函数 get

如果 l=r, 返回 e(), 否则 返回 op(a[l], a[l+1], ... , a[r - 1]) 

```c++
S seg.get(int l, int r)
```
- 约束: 0 <= l <= r <= n
- 时间复杂度 O(log(n))


4.get_all 函数

如果 n=0, 返回 e(), 否则 返回 op(a[0], a[1], ... , a[n - 1]) 

```c++
S seg.get_all()
```

- 时间复杂度 O(1)


5.max_right函数

1. 需要定义 bool f(S x) 函数，max_right函数在线段树上执行二分查找
2. 应该定义以S为参数并返回bool的函数对象。

函数返回一个下标 r， 满足下面两个条件

- r = l or f(op(a[l], a[l+1], ..., a[r-1])) = true
- r = n or f(op(a[l], a[l+1], ..., a[r])) = false

如果 f 是单调的，
返回 满足 `f(op(a[l], a[l + 1], ..., a[r - 1])) = true` 的最大的 r

```c++
1. int seg.max_right<f>(int l)
2. int seg.max_right<F>(int l, F f)
```
-  0 <= l <= n
-  f(e()) = true
-  f函数传入相同的参数时，返回值相同
-  时间复杂度 O(logn)

6. min_left

```c++
(1) int seg.min_left<f>(int r)
(2) int seg.min_left<F>(int r, F f)
```

1. 需要定义 bool f(S x) 函数，max_right函数在线段树上执行二分查找
2. 应该定义以S为参数并返回bool的函数对象。

函数返回一个下标 l， 满足下面两个条件

- l = r or f(op(a[l], a[l + 1], ..., a[r - 1])) = true
- l = 0 or f(op(a[l - 1], a[l], ..., a[r - 1])) = false

如果 f 是单调的，
返回 满足 `f(op(a[l], a[l + 1], ..., a[r - 1])) = true` 的最小的 l

-  0 <= l <= n
-  f(e()) = true
-  f函数传入相同的参数时，返回值相同
-  时间复杂度 O(logn)

**不同信息的函数设置**

1.最大值
```c++
int op(int a, int b){
    return max(a, b);
}

int e() {
    return -1; // 比数组中最小元素要小 即满足 max(e(),a[i]) = a[i]
}
```

2.最小值
```c++
int op(int a, int b){
    return min(a, b);
}

int e() {
    //INF ,满足 op(e(), a) = op(a, e()) = a,  min(a, e()) = a
    return (int)1e9; 
}
```

3.区间和
```c++
int op(int a, int b){
    return a+b;
}

int e() {
    return 0;  // e() + a[i] = a[i]
}

//将第i个数+b，
seg.seg(a-1, seg.get(a-1) + b);
```