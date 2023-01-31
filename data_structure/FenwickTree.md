# FenwickTree

支持单点修改，区间查询的数据结构，时间复杂度均为 `O(log(n))`,
本地 sublime 代码补全缩写 'bit'


### 模板代码

```c++
template <typename T>
struct FenwickTree {
    int n;
    vector<T> a;
    static int lg(unsigned x) {return x == 0 ? -1 : 31 - __builtin_clz(x);}
    FenwickTree(int n) : n(n), a(n) {}
    FenwickTree(vector<T> &A) : FenwickTree((int)A.size()) {
        for (int i = 0; i < n; ++i) add(i, A[i]);
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) a[i - 1] += v;
    }
    T sum(int x) {
        T ans = 0;
        for (int i = min(x + 1, n); i > 0; i -= i & -i) ans += a[i - 1];
        return ans;
    }
    T sum(int l, int r) { //sum[l..r]
        return l <= r ? sum(r) - sum(l - 1) : 0; 
    }
    T get(int x) const {   // 0 <= x < n, get a[x] in O(1), Equivalent to sum(x, x).
        T s = a[x];
        int y = x + 1;
        y -= y & -y;
        while (x != y) s -= a[x - 1], x -= x & -x;
        return s;
    }
    void set(int x, T v) {  // 0 <= x < n, set a[x] = v
        add(x, v - get(x));
    }
    int kth(T k) const {
        if (k < T()) return -1;
        int x = 0;
        for (int i = 1 << lg(n); i; i /= 2) {
            if (x + i <= n && k >= a[x + i - 1]) {
                x += i, k -= a[x - 1];
            }
        }
        return x;
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
fen.sum(l, r)
```

4. 单点赋值

```c++
fen.set(x, c)
```

5. 获取下标为x处的值, 0 <= x < n

```c++
fen.get(x)
```

6. kth 函数

传入参数x，返回一个 [0,n] 之间最小的p，满足 sum(p) > x.

kth 函数可以用来模拟 值在[0, n) 之间的 ordered set/multiset。

+ set(x, 1)  // 在set中插入x
+ set(x, 0)  // 从set中删除x
+ sum(x)    // set中小于等于x的元素数量
+ get(x)    // x 是否存在集合中
+ kth(x)    // 集合中的第 k 小元素， k从0开始，**如果x>=n , 则返回n**