# FenwickTree2D

支持单点修改，区间查询的数据结构，时间复杂度均为 `O(log(n*m))`,
本地 sublime 代码补全缩写 'fen'



### 模板代码

```c++
template<typename T>
struct FenwickTree2D{
    vector<vector<T>> tr;
    int n, m;
    FenwickTree2D(int N, int M) : n(N), m(M), tr(N, vector<T>(M, 0)){}

    void add(int x, int y, int val){ // 0 <= x < n, 0 <= y < m
        for(int i = x + 1; i <= n; i += i & -i)
            for(int j = y + 1; j <= m; j += j & -j)
                tr[i - 1][j - 1] += val;
    }
 
    T ask(int x, int y) { // 0 <= x < n, 0 <= y < m
        T ret = 0;
        for(int i = x + 1; i > 0; i -= i & -i)
            for(int j = y + 1; j > 0; j -= j & -j)
                ret += tr[i - 1][j - 1];
        return ret;
    }
    T ask(int x1, int y1, int x2, int y2) {  // sum[x1..x2, y1..y2]
        return ask(x2, y2) - ask(x2, y1 - 1) - ask(x1 - 1, y2) + ask(x1 - 1, y1 - 1);
    }
};
```

### 使用说明

1. 定义一个二维 FenwickTree

```c++
FenwickTree2D<int> fen(n, m);
```
或者

```c++
vector<vector<int>> a;
FenwickTree2D<int> fen(a);
```

2. 单点修改， 赋值 `a[x][y] = a[x][y] + c`  其中 `0 <= x < n, 0 <= y < m`

```c++
fen.add(x, y, c)
```

3. 区间查询，查询 `x1 <= x <= x2, y1 <= y <= y` 组成矩形的元素和

```c++
fen.ask(x1, y1, x2, y2)
```
