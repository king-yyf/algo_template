# Dijkstra最短路

Dijkstra 算法是一种求解 **非负权图** 上单源最短路径的算法。

**时间复杂度**

本模板使用优先队列实现， O(mlog(n))

### 模板代码


```c++
template<typename T>
struct Dijkstra {
    using E = pair<T, int>;
    int n;
    vector<vector<E>> g;    // cost, to
    Dijkstra(int N) : n(N), g(N) {}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
    }

    void add_bidir_edge(int u, int v, T cost) {
        add_edge(u, v, cost);
        add_edge(v, u, cost);
    }

    vector<T> dijkstra(int s) {  // unreachable : -1
        vector<T> d(n, T(-1)); 
        priority_queue<E, vector<E>, greater<E>> q;
        d[s] = 0;
        q.emplace(0, s);
        while (!q.empty()) {
            auto [cost, u] = q.top();
            q.pop();
            if (d[u] < cost) continue;
            for (auto &[c, v] : g[u]) {
                if (d[v] == T(-1) || d[u] + c < d[v]) {
                    d[v] = d[u] + c;
                    q.emplace(d[v], v);
                }
            }
        } 
        return d;
    }
};
```

### 使用说明

1. 定义一个 Dijkstra, 点的编号从0开始

```c++
Dijkstra<int> d(n);
```


2. 添加 从 from 到 to， 边权为 cost 的有向边。

```c++
d.add_edge(from, to, cost);
```

3. 添加 从 from 到 to， 边权为 cost 的无向边。

```c++
d.add_bidir_edge(from, to, cost);
```

4. 求源点s到所有节点的最短路，不可达返回-1.

```c++
auto ans = d.dijkstra(s);
```

### 双向 dijkstra

1. 有向图

```c++
template<typename T>
struct BiDirDijkstra {
    using E = pair<T, int>;
    int n;
    vector<vector<E>> g, gr;
    BiDirDijkstra(int N) : n(N), g(N), gr(n) {}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
        gr[v].emplace_back(cost, u);
    }

    T get(int s, int t) {
        priority_queue<E, vector<E>, greater<E>> q, qr;
        vector<T> d(n, T(-1)), dr(n, T(-1)); 
        vector<bool> vis(n), visr(n);
        d[s] = 0, dr[t] = 0;
        q.emplace(0, s); qr.emplace(0, t);
        bool ok = false;
        while (!q.empty() && !qr.empty()) {
            {
                auto [cost, u] = q.top();
                q.pop();
                if (visr[u]) {ok = true; break;}
                if (vis[u]) continue;
                vis[u] = true;
                for (auto &[c, v] : g[u]) {
                    if (d[v] == T(-1) || d[u] + c < d[v]) {
                        d[v] = d[u] + c;
                        q.emplace(d[v], v);
                    }
                }
            }
            {
                auto [cost, u] = qr.top();
                qr.pop();
                if (vis[u]) {ok = true; break;}
                if (visr[u]) continue;
                visr[u] = true;
                for (auto &[c, v] : gr[u]) {
                    if (dr[v] == T(-1) || dr[u] + c < dr[v]) {
                        dr[v] = dr[u] + c;
                        qr.emplace(dr[v], v);
                    }
                }
            }
        }
        if (!ok) return T(-1);
        T ans = T(-1);
        for (int i = 0; i < n; ++i) {
            if (d[i] == T(-1) || dr[i] == T(-1)) continue;
            if (ans == T(-1) || ans > d[i] + dr[i]) {
                ans = d[i] + dr[i];
            }
        }
        return ans;
    }
};
```

2. 无向图

```c++
template<typename T>
struct BiUnDirDijkstra {
    using E = pair<T, int>;
    int n;
    vector<vector<E>> g;
    BiUnDirDijkstra(int N) : n(N), g(N) {}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
        g[v].emplace_back(cost, u);
    }

    T get(int s, int t) {
        priority_queue<E, vector<E>, greater<E>> q, qr;
        vector<T> d(n, T(-1)), dr(n, T(-1)); 
        vector<bool> vis(n), visr(n);
        d[s] = 0, dr[t] = 0;
        q.emplace(0, s); qr.emplace(0, t);
        bool ok = false;
        while (!q.empty() && !qr.empty()) {
            {
                auto [cost, u] = q.top();
                q.pop();
                if (visr[u]) {ok = true; break;}
                if (vis[u]) continue;
                vis[u] = true;
                for (auto &[c, v] : g[u]) {
                    if (d[v] == T(-1) || d[u] + c < d[v]) {
                        d[v] = d[u] + c;
                        q.emplace(d[v], v);
                    }
                }
            }
            {
                auto [cost, u] = qr.top();
                qr.pop();
                if (vis[u]) {ok = true; break;}
                if (visr[u]) continue;
                visr[u] = true;
                for (auto &[c, v] : g[u]) {
                    if (dr[v] == T(-1) || dr[u] + c < dr[v]) {
                        dr[v] = dr[u] + c;
                        qr.emplace(dr[v], v);
                    }
                }
            }
        }
        if (!ok) return T(-1);
        T ans = T(-1);
        for (int i = 0; i < n; ++i) {
            if (d[i] == T(-1) || dr[i] == T(-1)) continue;
            if (ans == T(-1) || ans > d[i] + dr[i]) {
                ans = d[i] + dr[i];
            }
        }
        return ans;
    }
};
```
