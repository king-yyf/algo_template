# RangeSumTree

支持区间修改，区间查询的数据结构，时间复杂度均为 `O(log(n))`,


### 模板代码

依赖 FenwickTree, 注意由于乘积操作，中间结果容易爆int。

```c++
template <typename T>
struct RangeSumTree {
    FenwickTree<T> a, b;
    RangeSumTree(int N) : a(N + 1), b(N + 1) {}

    // add x to [l, r]
    void add(int l, int r, const T &x) { 
        a.add(l, x), a.add(r + 1, -x);
        b.add(l, x * (1 - l)), b.add(r + 1, x * r);
    }

    // sum of [l, r]  0 <= l <= r < n
    T sum(int l, int r) {
        r++;
        return a.ask(r + 1) * r + b.ask(r + 1) - a.ask(l + 1) * l - b.ask(l + 1);
    }
};
```

### 使用说明

1. 定义一个 RangeSumTree

```c++
RangeSumTree<long long> t(n);
```


2. 区间修改，对[l,r] 区间的数执行 a[x] = a[x] + c, 下标从0开始

```c++
t.add(l, r, c)
```

3. 区间查询 查询 [l, r] 的区间和, 下标从0开始

```c++
t.sum(l, r)
```
