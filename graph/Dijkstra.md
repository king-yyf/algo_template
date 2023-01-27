# Dijkstra最短路

Dijkstra 算法是一种求解 **非负权图** 上单源最短路径的算法。

**时间复杂度**

本模板使用优先队列实现， O(mlog(m))

### 模板代码


```c++
const int DN = 1e5+10;
class DIJ {
public:

    struct Edge{
        int t, c; //to,cost
        Edge(int u, int v): t(u), c(v) {}
        bool operator < (const Edge& o) const {
            return c > o.c;
        }
    };

    using T = int;
    T dis[DN];
    bool vis[DN];
    vector<Edge> g[DN];

    DIJ(){}
    //添加边，dir 是否是有向图
    void add(int f, int t, int c, bool dir = 0) {
        g[f].emplace_back(t, c);
        if(dir == 0)
            g[t].emplace_back(f, c);
    }

    //从s到t的最短路,如果不存在路径输出-1.
    T dijk(int s, int t) {
        memset(dis,0x3f, sizeof(dis));
        memset(vis, 0, sizeof(vis));
        dis[s] = 0;
        priority_queue<Edge, vector<Edge>> pq;
        pq.push({s,0});
        while (!pq.empty()) {
            int v = pq.top().t;
            pq.pop();
            if(vis[v]) continue;
            vis[v] = 1;
            for (auto& e : g[v]) {
                if (dis[e.t] > dis[v] + e.c) {
                    dis[e.t] = dis[v] + e.c;
                    pq.push({e.t, dis[e.t]});
                }
            }
        }
        return dis[t] < 0x3f3f3f3f ? dis[t] : -1;
    }
};
```

### 使用说明

1. 定义一个 DIJ

```c++
DIJ d;
```


2. 添加 从 from 到 to， 边权为 cost 的边, 默认是无向边。

```c++
d.add(from, to, cost);
```

如果是有向边， 设置 dir = 1.

```c++
d.add(from, to, cost, 1);
```


3. 求 s 到 t 的最短路

```c++
int dis = d.dijk(s, t);
```
