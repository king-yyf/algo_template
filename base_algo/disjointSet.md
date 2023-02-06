# 并查集

基础并查集模板 sumlime 本地代码补全缩写为 `dsu`


### 模板代码

```c++
struct DSU {
    vector<int> p, siz;
    DSU(int n) : p(n), siz(n, 1) { iota(p.begin(), p.end(), 0); }
    inline int get(int x) { return (x == p[x] ? x : (p[x] = get(p[x])));}
    bool same(int x, int y) { return get(x) == get(y); }
    bool merge(int x, int y) {
        x = get(x), y = get(y);
        if (x == y) return false;
        siz[x] += siz[y];
        p[y] = x;
        return true;
    }
    int size(int x) { return siz[get(x)]; }
    vector<vector<int>> groups() {
        vector<vector<int>> res(p.size());
        for (int i = 0; i < p.size(); i++) res[get(i)].push_back(i);
        res.erase(
            remove_if(res.begin(), res.end(),
                           [&](const vector<int>& v) { return v.empty(); }),
            res.end());
        return res;
    }
};
```

### 使用说明

1. 创建一个大小为n的并查集

```c++
DSU d(n);
```

2. 判断x和y是否在同一个连通分量中

其中 0 <= x, y < n

```c++
d.same(x, y);
```

3. 求x的祖先节点

```c++
int p = d.get(x);
```

4. 合并x和y所在的连通分量

```c++
d.merge(x, y);
```

5. 求x所在连通分量包含的节点数量

```c++
int n = d.size(x);
```

6. 获取当前所有的连通分量及其包含的节点

```c++
vector<vector<int>> ans = d.groups();
```