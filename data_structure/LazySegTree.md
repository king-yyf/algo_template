# 带懒标记线段树

支持区间修改，区间查询的数据结构，时间复杂度均为 `O(log(n))`

模板取自 atcoder library

[atcoder doc](https://atcoder.github.io/ac-library/master/document_en/lazysegtree.html)

[oi wiki介绍](https://oi-wiki.org/ds/seg/)

### 模板代码

```c++
template <class S,           // 线段树维护的数据信息
          S (*op)(S, S),    // 左右子节点信息合并到当前节点
          S (*e)(),
          class F,          // 懒标记维护的信息
          S (*tag)(F, S),  // 查询时给当去节点打上懒标记
          F (*merge)(F, F),  // 懒标记合并
          F (*id)()>        // 懒标记的默认值, 用于清空父节点的懒标记
struct LazySegTree {
    int n, size, log;
    vector<S> d;
    vector<F> lz;
    void pull(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void push_down(int k, F f) {
        d[k] = tag(f, d[k]);
        if (k < size) lz[k] = merge(f, lz[k]);
    }
    void push(int k) {
        push_down(2 * k, lz[k]), push_down(2 * k + 1, lz[k]);
        lz[k] = id();
    }
    LazySegTree() : LazySegTree(0) {}
    explicit LazySegTree(int N) : LazySegTree(vector<S>(N, e())) {}
    explicit LazySegTree(const vector<S>& v) : n(int(v.size())) {
        log = ceil_lg(n), size = 1 << log;
        d = vector<S>(2 * size, e()), lz = vector<F>(size, id());
        for (int i = 0; i < n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) pull(i);
    }
    int ceil_lg(int n) {   // minimum non-neg x s.t. `n <= 2^x`
        int x = n == 0 ? 0 :  32 - __builtin_clz(n);
        return  __builtin_popcount(n) == 1 ? x - 1: x;
    }
    void set(int p, S x) {   // 0 <= p < n
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) pull(p >> i);
    }
    S get(int p) {     // Assert 0 <= p < n
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }
    S get(int l, int r) {   // op(a[l], ..., a[r - 1])
        if (l == r) return e();
        l += size, r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        S sl = e(), sr = e();
        while (l < r) {
            if (l & 1) sl = op(sl, d[l++]);
            if (r & 1) sr = op(d[--r], sr);
            l >>= 1, r >>= 1;
        }
        return op(sl, sr);
    }
    S get_all() { return d[1]; }
    void apply(int p, F f) {   // 0 <= p < n
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = tag(f, d[p]);
        for (int i = 1; i <= log; i++) pull(p >> i);
    }
    void apply(int l, int r, F f) {  // a[i] = f(a[i]), [l, r)
        if (l == r) return;
        l += size, r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        while (l < r) {
            if (l & 1) push_down(l++, f);
            if (r & 1) push_down(--r, f);
            l >>= 1, r >>= 1;
        }
        l = l2, r = r2;
        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) pull(l >> i);
            if (((r >> i) << i) != r) pull((r - 1) >> i);
        }
    }
    template <bool (*g)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template <class G> int max_right(int l, G g) { // 0 <= l <= n, g(e()) is true
        if (l == n) return n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) sm = op(sm, d[l]), l++;
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template <bool (*g)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template <class G> int min_left(int r, G g) { // 0 <= r <= n, g(e()) is true
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm)))  sm = op(d[r], sm), r--;
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};
```

### 使用说明

1. 构造函数

```c++
LazySegTree<S, op, e, F, tag, merge, id> seg(n);
```
或者

```c++
vector<S> a;
LazySegTree<S, op, e, F, tag, merge, id> seg(a);
```

+ 时间复杂度 O(n)

2. 赋值 a[p] = x;  0 <= p < n

```c++
seg.set(p, x)
```
+ 时间复杂度 O(log(n))

3. 查询下标 p 处的值, 0 <= p < n

```c++
seg.get(p)
```

+ 时间复杂度 O(log(n))

4. 查询区间 [l, r - 1] 的 op值, 0 <= l <= r <= n

返回 op(a[l], a[l + 1], ..., a[r - 1])

```c++
S s = seg.get(l, r);
```

+ 时间复杂度 O(log(n))

5. 查询整个数组的op值，op(a[0], ..., a[n - 1])

```c++
S s = seg.get_all();
```

+ 时间复杂度 O(1)

6. 区间修改，0 <= l <= r <= n

执行 a[i] = f(a[i]) for all i = l..r-1.

```c++
seg.apply(l, r);
```

+ 时间复杂度 O(log(n))

7. max_right

线段树上二分， 返回一个r 满足

+ r = l 或者 g(op(a[l], a[l + 1], ..., a[r - 1])) = true
+ r = n 或者 g(op(a[l], a[l + 1], ..., a[r])) = false

+ 时间复杂度 O(log(n))

8. min_left

线段树上二分， 返回一个l 满足

+ l = r 或者 g(op(a[l], a[l + 1], ..., a[r - 1])) = true
+ l = 0 或者 g(op(a[l - 1], a[l], ..., a[r - 1])) = false

+ 时间复杂度 O(log(n))

示例：

1. 给区间增加一个数，以及区间查询问题

```c++
struct S {
	long long sum;  // 维护区间和
	int size;       // 维护区间大小
};

using F = long long; //懒标记

S op(S x, S y) {  //sum等于左右sum之和，size类似
    S s;
    s = S{x.sum + y.sum, x.size + y.size};
    return s;
}

S e() {    // 初始化 sum = 0， size = 0.
    return S();
};

S tag(F f, S s) { return S{s.sum + f * s.size, s.size}; } // 打标记 sum += 标记*size

F merge(F x, F y) { return x + y; }  // 父节点标记合并到左右子节点

F id() { return 0; }   // 标记的0元，

LazySegTree<S, op, e, F, tag, merge, id> seg(vector<S>(n,{0,1}));
```

2. 将区间中的每个数 执行 a[i] = a[i] * c + d

```c++
struct S {
    mint a;
    int size;
};

struct F {
    mint a, b;
};

S op(S l, S r) { return S{l.a + r.a, l.size + r.size}; }

S e() { return S{0, 0}; }

S tag(F l, S r) { return S{r.a * l.a + r.size * l.b, r.size}; }

F merge(F l, F r) { return F{r.a * l.a, r.b * l.a + l.b}; }

F id() { return F{1, 0}; }
```
