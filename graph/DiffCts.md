# 差分约束

差分约束系统 是一种特殊的 n 元一次不等式组，它包含 n 个变量 `x[1],x[2]...,x[n]` 以及 m 个约束条件，每个约束条件是由两个其中的变量做差构成的，形如 'x[i] <= x[j] + c[k]'，其中 '1 <= i,j<=n, i~=j, 1<=k<=m, c[k]是常数'。


**求解不等式组可行解**

1. 满足条件

存在一个超级源点，从源点出发的，一定可以走到所有的边

2. 求解步骤

+ 将每个不等式 x[i] <= x[j] + c[k], 转化成一条从 x[j] 走到 x[i] 长度为 c[k] 的一条边
+ 找一个超级源点，使得该源点可以遍历所有的边
+ 从源点求求一遍单源最短路，
  + 结果1:如果存在负环，则原不等式组无解
  + 结果2:如果没有负环，则dist[i] 就是原不等式组的一个可行解

3. 求每个变量的最大值或最小值

+ 最小值：最长路
+ 最大值：最短路，例如求x[i]的最大值，求的是所有从x[i]出发构成的不等式连, x[i]<=x[j]+c1<=...<=0+c1+c2+..,，所计算出的上界，，最终x[i]的最大值是所有上界的最小值。

4. 处理 x[i] <= c, 其中c是一个常数这类不等式

建一个超级源点 x[0], 不等式转化为 x[i] <= x[0] + c, 从0到i建一条长度为c的边。




### 模板代码

可以直接使用


```c++
template<typename T>
struct SPFA {
    using E = pair<T, int>;
    const T inf = numeric_limits<T>::max() / 2;
    int n;
    vector<vector<E>> g;    // cost, to
    vector<T> d;
    SPFA(int N) : n(N), g(N){}

    void add_edge(int u, int v, T cost) {
        g[u].emplace_back(cost, v);
    }

    void add_bidir_edge(int u, int v, T cost) {
        add_edge(u, v, cost);
        add_edge(v, u, cost);
    }
    
    bool cycle_detect() {
        d.assign(n, T(0));
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

    vector<T> spfa_max(int s) {  // 求最长路
        d.assign(n, -inf);
        return spfa(s, less<T>());
    } 

    vector<T> spfa_min(int s) {  // 求最长路
        d.assign(n, inf);
        return spfa(s, greater<T>());
    } 
    template <class Compare>
    vector<T> spfa(int s, Compare f) {  // unreachable : inf
        vector<int> vis(n, 0);
        queue<int> q;
        q.push(s);
        d[s] = 0, vis[s] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            vis[u] = 0;
            for (auto &[c, v] : g[u]) {
                if (f(d[v],d[u] + c)) {
                    d[v] = d[u] + c;
                    if (vis[v]) continue;
                    q.push(v);
                    vis[v] = 1;
                }
            }
        }
        return d;
    }
};
```

### 使用说明

初始化和添加边和SPFA用法一致，在建完图解不等式问题时

1. 判断是否有可行解

```c++
SPFA<int> s(n);
bool flag = s.cycle_detect();
```

2. 求满足可行解的最小值

```c++
SPFA<int> f(n);
auto d = f.spfa_max(s);
```

3. 求满足可行解的最大值

```c++
SPFA<int> f(n);
auto d = f.spfa_min(s);
```

