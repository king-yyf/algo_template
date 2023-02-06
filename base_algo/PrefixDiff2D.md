# 二维差分

二维差分模板 sumlime 本地代码补全缩写为 `diff`


### 模板代码

```c++
template <typename T>
struct PrefixDiff2D {
    int n, m;
    vector<vector<T>> a;
    PrefixDiff2D(int N, int M) : n(N), m(M), a(N, vector<T>(M, 0)){}
    PrefixDiff2D(vector<vector<T>> &A) {
        n = A.size(), m = A[0].size();
        a.assign(n, vector<T>(m, 0));
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < m; ++j)
                add(i, j, i, j, a[i][j]);
    }
    void add(int x1, int y1, int x2, int y2, T c) { // sum[x1..x2, y1..y2]
        a[x1][y1] += c;
        if (y2 + 1 < m) a[x1][y2 + 1] -= c;
        if (x2 + 1 < n) a[x2 + 1][y1] -= c;
        if (x2 + 1 < n && y2 + 1 < m) a[x2 + 1][y2 + 1] += c;
    }
    void get() { // 求操作完成后的矩阵
        for (int i = 1; i < n; ++i) a[i][0] += a[i - 1][0];
        for (int j = 1; j < m; ++j) a[0][j] += a[0][j - 1];
        for (int i = 1; i < n; ++i) for (int j = 1; j < m; ++j)
            a[i][j] += a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1];
    }
};
```

### 使用说明

元素下标从0开始。

1. 创建一个大小为 `n*m`, 初始值为0的 PrefixDiff2D

```c++
PrefixDiff2D<int> d(n, m);
```

或者

```c++
vector<vector<int>> a(n,vector<int>(m));
PrefixDiff2D<int> d(a);
```

2. 将 x1 <= x <= x2,  y1 <= y <= y2 子矩阵的元素 都加上c

其中 `0 <= x1 <= x2 < n`, `0 <= y1 <= y2 < m`


```c++
d.add(x1, y1, x2, y2, c);
```

3. 求操作完成后的最终数组，原数组为差分数组的前缀和

```c++
d.get();
auto ans = d.a;
```
