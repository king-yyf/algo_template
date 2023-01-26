# RangeAddTree

支持区间修改，单点查询的数据结构，时间复杂度均为 `O(log(n))`,


### 模板代码

```c++
template <typename T>
struct RangeAddTree {
    int n;
    vector<T> a;
    RangeAddTree(int N) : n(N), a(N) {}
    RangeAddTree(vector<T> &A) : RangeAddTree((int)A.size()) {
        for (int i = 1; i <= n; i++) {
            a[i - 1] = A[i - 1] - (i > 1 ? A[i - 2] : 0);
            for (int k = (i & -i) >> 1; k > 0; k >>= 1) 
                a[i - 1] += a[i - k - 1];
        }   
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) a[i - 1] += v;
    }
    void add(int l, int r, const T &v) {  // [l, r]
        add(l, v), add(r + 1, -v);
    }
    T get(int x) const {
        T ans = 0;
        for (int i = min(x + 1, n); i > 0; i -= i & -i) ans += a[i - 1];
        return ans;
    }
};
```

### 使用说明

1. 定义一个 RangeAddTree

```c++
RangeAddTree<int> t(n);
```

或者

```c++
vector<int> a(n);
RangeAddTree<int> t(a);
```

2. 区间修改，对[l,r] 区间的数执行 a[x] = a[x] + c

```c++
t.add(l, r, c)
```

3. 单点查询， 查询 a[x] 的值, 下标从0开始

```c++
t.get(x)
```
