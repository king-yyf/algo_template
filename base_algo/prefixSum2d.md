# 二维前缀和

二维前缀和模板 sumlime 本地代码补全缩写为 `prefix`


### 模板代码

```c++
template <typename T>
struct PrefixSum2D {
    int n, m;
    vector<vector<T>> a;
    PrefixSum2D(int N, int M) : n(N), m(M), a(N + 1, vector<T>(M + 1, 0)){}
    PrefixSum2D(vector<vector<T>> &A) {
        n = A.size(), m = A[0].size();
        a.assign(n + 1, vector<T>(m + 1, 0));
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < m; ++j)
                a[i + 1][j + 1] = a[i][j + 1] + a[i + 1][j] - a[i][j] + A[i][j];
    }
    T sum(int x1, int y1, int x2, int y2) { // sum[x1..x2, y1..y2]
        return a[x2 + 1][y2 + 1] - a[x2 + 1][y1] - a[x1][y2 + 1] + a[x1][y1];
    }
};
```

### 使用说明

元素下标从0开始。

1. 创建一个大小为 `n*m`, 初始值为0的 PrefixSum2D

```c++
PrefixSum2D<int> p(n, m);
```

或者

```c++
vector<vector<int>> a(n,vector<int>(m));
PrefixSum2D<int> p(a);
```

2. 求 x1 <= x <= x2,  y1 <= y <= y2 子矩阵的元素和

其中 `0 <= x1 <= x2 < n`, `0 <= y1 <= y2 < m`


```c++
long long sum = p.sum(x1, y1, x2, y2);
```
