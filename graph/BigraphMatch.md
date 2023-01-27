# 二分图判定


## 二分图最大匹配

设G为二分图，若在G的子图M中，任意两条边都没有公共节点，那么称M为二分图G的一组匹配，在二分图中，包含边数最多的一组匹配成为二分图的最大匹配。


### 匈牙利算法

g只需要加单向边，男选女
顶点编号从0开始 n1为男生的数目

时间复杂度： `O(n*m)`


```c++
int maxMatch(vector<vector<int>> &g, int n1) {
    int n = g.size(), ans = 0;
    vector<int> vis(n), f(n, -1);

    function<bool(int)> dfs = [&](int u) {
        for(auto& v: g[u]) {
            if(vis[v]) continue;
            vis[v] = 1;
            if (f[v] == -1 || dfs(f[v])) {
                f[v] = u;
                return true;
            }
        }
        return false;
    };

    for (int i = 0; i < n1; ++i) {
        vis.assign(n, 0);
        if (dfs(i)) ans++;
    }
    return ans;
}
```



### dinic 算法

二分图最大匹配是网络流的特殊形式。

假设左半边的点集为G, 右半边的点集为M, 从起点到G连一条长度为1的边, 从M到终点连一条长度为1的边 ,从G到M连一条长度为1的边。 后跑一遍最大流dinic算法 即为所求的二分图的最大匹配



## 二分图最大权完美匹配

给定一张带边权的二分图，其左部、右部点数相等，均为n个点，如果最大匹配有n条边，则称二分图的完美匹配。 二分图边权和最大的完美匹配，称二分图的最大权完美匹配



### KM算法

时间复杂度 O(n^3)

+ w:vector w(n + 1,vector<long long>(n + 1, -inf)); 权重矩阵
+ 顶点下标从1-n.
+ 需要满足存在完美匹配条件，每个点的相匹配点记录在f数组


```c++

const long long inf = 1e18;
long long KM(vector<vector<long long>> &w) {
    int n = w.size();  // 定点数为n-1个，1 - (n-1)
    vector<long long> ex(n), ey(n);
    vector<int> f(n, -1);

    auto bfs = [&](int u) {
        vector<int> vy(n), pre(n, -1);
        vector<long long> slack(n, inf);
        long long x, y = 0, yy = 0, d;
        f[y] = u;
        while (1) {
            x = f[y], d = inf, vy[y] = 1;
            for (int i = 1; i < n; ++i) {
                if (vy[i]) continue;
                if (slack[i] > ex[x] + ey[i] - w[x][i]) {
                    slack[i] = ex[x] + ey[i] - w[x][i];
                    pre[i] = y;
                }
                if (slack[i] < d) {
                    d = slack[i], yy = i;
                }
            }
            for (int i = 0; i < n; ++i) {
                if (vy[i]) ex[f[i]] -= d, ey[i] += d;
                else slack[i] -= d;
            }
            y = yy;
            if (f[y] == -1) break;
        }
        while (y) {
            f[y] = f[pre[y]];
            y = pre[y];
        }

    };

    for (int i = 1; i < n; ++i) bfs(i);
    
    long long res = 0;
    for (int i = 1; i < n; ++i) 
        if(f[i] != -1) res += w[f[i]][i];
    return res;
}
```