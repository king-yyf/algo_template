# FenwickTree

支持单点修改，区间查询的数据结构，时间复杂度均为 `O(log(n))`,
本地 sublime 代码补全缩写 'bit'


### 模板代码

```c++
template <typename T>
struct FenwickTree {
    int n;
    vector<T> a;
    FenwickTree(int n) : n(n), a(n) {}
    FenwickTree(vector<T> &A) : FenwickTree((int)A.size()) {
        for (int i = 0; i < n; ++i) add(i, A[i]);
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) a[i - 1] += v;
    }
    T ask(int x) {
        T ans = 0;
        for (int i = min(x + 1, n); i > 0; i -= i & -i) ans += a[i - 1];
        return ans;
    }
    T ask(int l, int r) { //sum[l..r]
        if (l > r) return 0;
        return ask(r) - ask(l - 1);
    }
};
```

### 使用说明

1. 定义一个 FenwickTree

```c++
FenwickTree<int> fen(n);
```

或者

```c++
vector<int> a(n);
FenwickTree<int> fen(a);
```

2. 单点修改， 赋值 `a[x] = a[x] + c`  下标从 `0-(n-1)`

```c++
fen.add(x, c)
```

3. 区间查询，查询区间 [l, r] 的元素和, `0 <= l, r <= n - 1`

```c++
fen.ask(l, r)
```
