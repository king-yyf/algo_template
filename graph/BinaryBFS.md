# 01 BFS

01bfs是 Dijkstra 算法是一种特殊情况，适用于边权只有0和1两种情况，可以在O(n + m) 时间内求出某点到其它节点的最短路径。



### 模板代码


```c++
template<typename T>
struct BFS01 {
    using E = pair<T, int>;
    int n;
    vector<vector<E>> g;    // cost, to
    BFS01(int N) : n(N), g(N){}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
    }

    void add_bidir_edge(int u, int v, T cost) {
        add_edge(u, v, cost);
        add_edge(v, u, cost);
    }

    vector<T> bfs(int s) {  // unreachable : -1
        vector<T> d(n, T(-1));
        vector<int> vis(n, 0);
        deque<int> q;
        q.push_back(s);
        d[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            if (vis[u]) continue;
            vis[u] = 1;
            for (auto &[c, v] : g[u]) {
                if (d[v] != T(-1) and d[v] <= d[u] + c) continue;
                d[v] = d[u] + c;
                if (c == 0) q.push_front(v);
                else q.push_back(v);
            }
        }
        return d;
    }
};
```

### 使用说明

1. 创建一个 n 个节点的 01 bfs

```c++
BFS01<int> b(n);
```

2. 添加边

```c++
b.add_edge(u, v, 0); // b.add_edge(u, v, 1);
```

3. 求最短路

```c++
b.bfs(s);
```

