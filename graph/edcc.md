# 桥与边双连通分量


### 模板代码



```c++
struct Edcc {
    int n, m = 0, dcc_cnt = 0, init = 0;
    vector<vector<pair<int,int>>> g;
    vector<array<int, 2>> elist;
    vector<int> dfn, low, ids, deg;
    vector<bool> is_bridge;

    Edcc(int n = 0): n(n), dfn(n, -1), low(n), g(n), ids(n){}

    void add_edge(int a, int b) {
        g[a].emplace_back(b, m);
        g[b].emplace_back(a, m);
        elist.push_back({a, b});
        m++;
    }

    void get_ids() {
        init = 1;
        vector<bool> visited(n, false);
        is_bridge.assign(m, false);
        vector<int> stk;
        int now_dfn = 0;

        function<void(int, int)> dfs = [&](int u, int fa) {
            visited[u] = true;
            stk.push_back(u);
            low[u] = dfn[u] = now_dfn++;
            int pa_cnt = 0;
            for (auto &[v, id]: g[u]) {
                if (v == fa && pa_cnt++ == 0) continue;
                if (visited[v]) {
                    low[u] = min(low[u], dfn[v]);
                } else {
                    dfs(v, u);
                    is_bridge[id] = low[v] > dfn[u];
                    low[u] = min(low[u], low[v]);
                }
            }
            if (dfn[u] == low[u]) {
                while (true) {
                    int v = stk.back();
                    stk.pop_back();
                    ids[v] = dcc_cnt;
                    if (u == v) break;
                }
                dcc_cnt ++;
            }
        };
        
        for (int i = 0; i < n; i++)
            if (!~dfn[i])
                dfs(i, -1);
    }

    vector<vector<int>> edcc() {
        if (!init) get_ids();
        vector<vector<int>> groups(dcc_cnt);
        for (int i = 0; i < n; ++i) {
            groups[ids[i]].push_back(i);
        }
        deg.assign(dcc_cnt, 0);
        for (int i = 0; i < m; ++i) {
            if (is_bridge[i]) {
                deg[ids[elist[i][0]]]++;
                deg[ids[elist[i][1]]]++;
            }
        }
        return groups;
    }

    vector<vector<int>> bridge_tree() {
        if (!init) get_ids();
        vector<vector<int>> tr(dcc_cnt);
        for (int i = 0; i < m; ++i) {
            if (is_bridge[i]) {
                int x = ids[elist[i][0]], y = ids[elist[i][1]];
                tr[x].push_back(y);
                tr[y].push_back(x);
            }
        }
        return tr;
    }
};
```


### 使用说明


1. 初始化 时间复杂度 O(n)

```c++
Edcc g(n);
```

2. 添加一条边，时间复杂度 O(1)

```c++
g.add_edge(u, v);
```

3. 求边的双连通分量

+ 时间复杂度 O(n + m)

返回一个二维vector, dcc[i]里存的是连通分量id为i的顶点编号。

```c++
auto dcc = g.edcc();
```

4. 每个顶点所在连通分量编号

```c++
for (int i = 0; i < n; ++i) {
    cout << g.ids[i] << " \n"[i == n - 1];
}
```

5. 判断某条边是否是桥

```c++
for (int i = 0; i < m; ++i) {
    cout << g.is_bridge[i] << " \n"[i == n - 1];
}
```


7. 边的双连通分量缩点后形成的树

+ 时间复杂度 O(n + m)

```c++
auto t = g.bridge_tree();
```