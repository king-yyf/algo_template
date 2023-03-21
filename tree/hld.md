# 树链剖分

**基础概念**

+ 重儿子：父节点所有儿子中，子树节点数目最多的节点
+ 重边：父节点和重儿子练成的边
+ 重链：由多条重边连接而成的路径

**结论**

1. 整棵树会被剖分成若干条重链
2. 轻儿子一定是某条重链的顶点
3. 任意一条路径被切分成不超过log(n)条重链


**数组**

第一次 dfs 完成下述数组赋值

+ fa[u]: 存u的父节点
+ dep[u]: 存u的深度
+ siz[u]: 存以u为根子树节点数
+ g[u][0]: 存u的重儿子，dfs过程中会将重儿子交换到g[u][0]位置

第二次 dfs 完成下述数组构建

+ top[u]: 存u所在重链的顶点
+ in[u]: 存u剖分后的新编号，即第一次遍历到u时的编号
+ seq[u]: seq[u] = v, 表示下标为v的节点剖分后的新编号为u, 如果 in[u] = v, 则 seq[v] = u


### 模板代码

本地sublime 代码补全提示为 `hld`


```c++
struct HLD {
    int n, cur = 0;
    vector<int> siz, top, dep, fa, in, out, seq;
    vector<vector<int>> g;
    HLD(int n) : n(n), siz(n), top(n), dep(n), fa(n, -1), in(n), out(n), seq(n), g(n) {}
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void build(int root = 0) {
        top[root] = root;
        dfs1(root);
        dfs2(root);
    }
    void dfs1(int u) {
        if (fa[u] != -1)
            g[u].erase(find(g[u].begin(), g[u].end(), fa[u]));

        siz[u] = 1;
        for (auto &v : g[u]) {
            fa[v] = u, dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (siz[v] > siz[g[u][0]]) {
                swap(v, g[u][0]);  // g[u][0] 存储u节点的重儿子
            }
        }
    }
    void dfs2(int u) {
        in[u] = cur++;
        seq[in[u]] = u;
        for (auto v : g[u]) {
            top[v] = v == g[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = cur;
    }
    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) u = fa[top[u]];
            else v = fa[top[v]];
        }
        return dep[u] < dep[v] ? u : v;
    }
    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }
    bool is_anc(int u, int v) {
        return in[u] <= in[v] && in[v] < out[u];
    }
    template<typename F> 
    void path(int u, int v, F &&f) { // 处理从u到v的路径
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            f(in[top[u]], in[u] + 1);
            u = fa[top[u]];
        }
        if (dep[u] < dep[v]) swap(u, v);
        f(in[v], in[u] + 1);
    }
    template<typename F> 
    void tree(int u, F &&f) { // 处理以u为根的子树
        f(in[u], out[u]);
    }
};
```

### 使用说明

1. 初始化一个HLD

+ 时间复杂度 O(n), 主要用来初始化各个数组

```c++
HLD h(n);
```

2. 添加边

```c++
h.add_edge(u, v); // 0 <= u, v < n
```

3. 两次dfs构建各种数组

+ 时间复杂度 O(n)

```c++
h.build(root);
```

4. 查询节点u和v的最近公共祖先

```c++
int p = h.lca(u, v);
```

5. 处理树上一条路径的修改与查询问题

根据剖分编号将树上两点间路径转化为不超过log(n)条重链，每条重链的新编号对应一个连续的区间，可以通过线段树维护区间的某些结果。

+ 时间复杂度： `O(log(n)*O(T)), O(T)` 是一次查询或修改所需要的时间，例如在线段树中为 log(n).

```c++
h.path(u, v,[&](int x, int y){
    f(x, y); // seg.apply(x, y, z), seg.get(x, y).sum
});
```

6. 处理树上一个子树的修改与查询问题

+ 时间复杂度： `O(T)` 是一次查询或修改所需要的时间，例如在线段树中为 log(n).

```c++
h.tree(u, [&](int x, int y){
    f(x, y); // seg.apply(x, y, z);
}); 
```
