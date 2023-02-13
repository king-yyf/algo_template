# 割点与点双连通分量


### 模板代码



```c++
struct Vdcc {
    int n, m = 0, init = 0;
    vector<vector<pair<int,int>>> g;
    vector<array<int, 2>> elist;
    vector<int> dfn, low, stack;
    vector<bool>  is_cut, is_bridge;
    vector<vector<int>> components;

    Vdcc(int n) : n(n), g(n), dfn(n), low(n){}

    void add_edge(int a, int b) {
        g[a].emplace_back(b, m);
        g[b].emplace_back(a, m);
        elist.push_back({a, b});
        m++;
    }

    void build(int root = -1) {
        init = 1;
        vector<bool> visited(n, false);
        is_cut.assign(n, false);
        is_bridge.assign(m, false);
        int now_dfn = 0;

        function<void(int, int)> dfs = [&](int u, int fa) {
            visited[u] = true;
            low[u] =  dfn[u] = now_dfn++;
            is_cut[u] = false;
            int pa_cnt = 0, child = 0;
            for (auto &[v, id] : g[u]) {
                if (v == fa && pa_cnt++ == 0) continue;
                if (visited[v]) {
                    low[u] = min(low[u], dfn[v]);
                    if (dfn[v] < dfn[u]) stack.push_back(u);
                } else {
                    int size = int(stack.size());
                    dfs(v, u);
                    child++;
                    low[u] = min(low[u], low[v]);
                    if (low[v] > dfn[u]) {
                        is_bridge[id] = true;
                        vector<int> comp = {u, v};
                        if (comp[0] > comp[1]) swap(comp[0], comp[1]);
                        components.push_back(comp);
                    } else if (low[v] == dfn[u]) {
                        stack.push_back(u);
                        vector<int> comp(stack.begin() + size, stack.end());
                        sort(comp.begin(), comp.end());
                        comp.erase(unique(comp.begin(), comp.end()), comp.end());
                        components.push_back(comp);
                        stack.resize(size);
                    } else {
                        stack.push_back(u);
                    }
                    if (low[v] >= dfn[u]) is_cut[u] = true;
                }
            }
            if (fa < 0) is_cut[u] = child > 1;
        };

        if (0 <= root && root < n)
            dfs(root, -1);
        for (int i = 0; i < n; i++)
            if (!visited[i])
                dfs(i, -1);
    }
    
};

// Note: instead of a block-cut tree this is technically a block-vertex tree, which ends up being much easier to use.
struct block_cut_tree {
    Vdcc &bi_comps;

    int n, BC, T;
    vector<vector<int>> g;
    vector<int> fa, dep;

    block_cut_tree(Vdcc &_bi_comps) : bi_comps(_bi_comps) {}

    void build() {
        n = bi_comps.n, BC = int(bi_comps.components.size());
        T = n + BC;
        g.assign(T, {});

        auto add_edge = [&](int a, int b) {
            assert((a < n) ^ (b < n));
            g[a].push_back(b);
            g[b].push_back(a);
        };

        function<void(int, int)> dfs = [&](int u, int pa) {
            fa[u] = pa, dep[u] = pa < 0 ? 0 : dep[pa] + 1;
            for (int v : g[u]) if (v != pa)
                dfs(v, u);
        };

        for (int bc = 0; bc < BC; bc++)
            for (int x : bi_comps.components[bc])
                add_edge(x, n + bc);

        fa.assign(T, -1);
        dep.resize(T);

        for (int root = 0; root < T; root++)
            if (fa[root] < 0)
                dfs(root, -1);
    }

    bool same_component(int a, int b) const {
        if (dep[a] > dep[b])
            swap(a, b);
        return a == b || (dep[b] == dep[a] + 2 && fa[fa[b]] == a) || (fa[a] >= 0 && fa[a] == fa[b]);
    }
};
```


### 使用说明


1. 初始化 时间复杂度 O(n)

```c++
Vdcc g(n);
```

2. 添加一条边，时间复杂度 O(1)

```c++
g.add_edge(u, v);
```

3. 计算点双连通分量，割点割边

+ is_cut: 存储某个点是否是割点
+ is_bridge: 存储某条边是否是桥
+ components: 点双双连通分量，割点可能存在于多个点双双连通分量中。

4. block_cut_tree

设原始图中点的数目为 n, 点双双连通分量数目为 m，建一棵 (n+m) 个点的树，由每个原图中的点向其所在连通分量的点点编号建一条边，最终结果会形成一棵树(如果原图是连通的)或森林。
