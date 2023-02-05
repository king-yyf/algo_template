# spfa 求负环

spfa 算法是用于求最短路的算法，同时可以用spfa判断负环。 但是有被卡的可能。


### 模板代码

spfa 求最短路

```c++
template<typename T>
struct SPFA {
    using E = pair<T, int>;
    const T inf = numeric_limits<T>::max() / 2;
    int n; 
    vector<vector<E>> g;    // cost, to
    SPFA(int N) : n(N), g(N){}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
    }

    void add_bidir_edge(int u, int v, T cost) {
        add_edge(u, v, cost);
        add_edge(v, u, cost);
    }
    
    vector<T> spfa(int s) {  // unreachable : inf
        vector<T> d(n, inf);
        vector<int> vis(n, 0);
        queue<int> q;
        q.push(s);
        d[s] = 0, vis[s] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            vis[u] = 0;
            for (auto &[c, v] : g[u]) {
                if (d[v] > d[u] + c) {
                    d[v] = d[u] + c;
                    if (vis[v]) continue;
                    q.push(v);
                    vis[v] = 1;
                }
            }
        }
        return d;
    }

    vector<T> spfa_slf(int s) {  // unreachable : inf
        vector<T> d(n, inf);
        vector<int> vis(n, 0);
        deque<int> q;
        q.push_back(s);
        d[s] = 0, vis[s] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            vis[u] = 0;
            for (auto &[c, v] : g[u]) {
                if (d[v] > d[u] + c) {
                    d[v] = d[u] + c;
                    if (vis[v]) continue;
                    if (!q.empty() && d[v] > d[q.front()]) q.push_back(v);
                    else q.push_front(v);
                    vis[v] = 1;
                }
            }
        }
        return d;
    }
};
```

spfa 判负环

```c++
template<typename T>
struct SPFA {
    using E = pair<T, int>;
    const T inf = numeric_limits<T>::max() / 2;
    int n;
    vector<vector<E>> g;    // cost, to
    SPFA(int N) : n(N), g(N){}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
    }

    void add_bidir_edge(int u, int v, T cost) {
        add_edge(u, v, cost);
        add_edge(v, u, cost);
    }
    
    bool cycle_detect() {
        vector<T> d(n);
        vector<int> pre(n, -1);
        vector<bool> inq(n, false);
        queue<int> q;
        for (int i = 0; i < n; ++i) 
            q.push(i);
        int cnt = 0;
        auto detect = [&]() {
            vector<int> p;
            vector<bool> ins(n, false), vis(n, false);
            for (int i = 0; i < n; ++i) {
                if (vis[i]) continue;
                for (int j = i; j != -1; j = pre[j]) {
                    if (!vis[j]) {
                        vis[j] = true;
                        p.push_back(j);
                        ins[j] = true;
                    } else {
                        if (ins[j]) return true;
                        break;
                    }
                }
                for (int j : p) ins[j] = false;
                p.clear();
            }
            return false;
        };

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = false;
            for (auto &[c, v] : g[u]) {
                if (d[v] > d[u] +c) {
                    pre[v] = u;
                    d[v] = d[u] + c;
                    if (++cnt == n) {
                        cnt = 0;
                        if (detect()) return true;
                    }
                    if (!inq[v]) {
                        q.push(v);
                        inq[v] = true;
                    }
                }
            }
        }

        return detect();
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

