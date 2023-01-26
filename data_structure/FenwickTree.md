# FenwickTree

支持单点修改，区间查询的数据结构，时间复杂度均为 `O(nlog(n))`

1. 定义一个 FenwickTree

```c++
FenwickTree<int> fen(n);
```

或者

```c++
vector<int> a(n);
FenwickTree<int> fen(n);
```

2. 单点修改， 赋值 `a[x] = a[x] + c`  下标从 `0-(n-1)`

```c++
fen.add(x, c)
```

3. 区间查询，查询区间 [l, r]的元素和

```c++
fen.ask(l, r)
```

