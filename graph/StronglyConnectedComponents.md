# 强连通分量


### 模板代码

模板代码基于 AC Library

[SCC doc](https://atcoder.github.io/ac-library/master/document_en/scc.html)

```c++
struct SCC {
    int n, cnt = 0;
    vector<pair<int, int>> edges;
    vector<int> low, dfn, ids, in, out;
    explicit SCC(int n) : n(n), low(n), dfn(n, -1), ids(n){}
    void add_edge(int from, int to) { edges.push_back({from, to}); }

    void scc_ids() {
        vector<int> start(n + 1), elist(edges.size()), visited;
        for (auto &e : edges) 
            start[e.first + 1]++;
        for (int i = 1; i <= n; ++i) 
            start[i] += start[i - 1];
        auto counter = start;
        for (auto &e : edges)
            elist[counter[e.first]++] = e.second;
        int now_dfn = 0;
        visited.reserve(n);
        auto dfs = [&](auto self, int v) -> void {
            low[v] = dfn[v] = now_dfn++;
            visited.push_back(v);
            for (int i = start[v]; i < start[v + 1]; i++) {
                auto to = elist[i];
                if (dfn[to] == -1) {
                    self(self, to);
                    low[v] = min(low[v], low[to]);
                } else {
                    low[v] = min(low[v], dfn[to]);
                }
            }
            if (low[v] == dfn[v]) {
                while (true) {
                    int u = visited.back();
                    visited.pop_back();
                    dfn[u] = n, ids[u] = cnt;
                    if (u == v) break;
                }
                cnt++;
            }
        };
        for (int i = 0; i < n; i++) if (dfn[i] == -1) dfs(dfs, i);
        for (auto& x : ids) x = cnt - 1 - x;
    }

    vector<vector<int>> scc() {
        scc_ids();
        vector<vector<int>> groups(cnt);
        for (int i = 0; i < n; i++) {
            groups[ids[i]].push_back(i);
        }
        in.assign(cnt, 0), out.assign(cnt, 0);
        for (auto &[from, to]: edges) {
            int x = ids[from], y = ids[to];
            if (x != y) in[y]++, out[x]++;
        }
        return groups;
    }
};
```


### 使用说明

1. 初始化, 时间复杂度 O(n)

```c++
SCC g(n);
```

2. 添加一条有向边, 均摊复杂度 O(1)

```c++
g.add_edge(from, to);
```

3. 求强连通分量 时间复杂度 O(n + m)

```c++
vector<vector<int>> ans = g.scc();
```

返回一个二维 vector, 满足

+ 每个顶点恰好出现在一个 顶点数组中
+ 每个数组中的顶点对应于图中的一个强连通分量
+ 顶点数组按照拓扑序排序，例如，节点 (u, v) 是两个不同强连通分量中的节点，如果 有一条 从u到v的直接有向边，则包含u的节点数组排在包含v的节点数组前面。

4. 有向图强连通分量个数

```c++
int m = g.cnt;
```

5. 缩点后的拓扑图每个强连通分量表示的新点的入度和出度

其中 in[i]表示缩点后的拓扑图第i个强连通分量的入度，out[i]表示出度。

```c++
vector<int> in(cnt), out(cnt);
```