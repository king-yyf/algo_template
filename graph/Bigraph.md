# 二分图判定


**判定定理**

**二分图不存在长度为奇数的环**

因为每一条边都是从一个集合走到另一个集合，只有走偶数次才可能回到同一个集合。


### 染色法代码


**dfs**


```c++
bool isBipartite(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> color(n);       
    
    function<bool(int, int)> dfs = [&](int u, int c) {
        color[u] = c;
        for (int v: g[u]) {
            if (!color[v]) {
                if (!dfs(v, 3 - c)) return false;
            }else if (color[v] == c) return false;
            
        }
        return true;
    };

    for (int i = 0; i < n; i++) 
        if (!color[i] && !dfs(i, 1)) return false;
    return true;
}
```

**bfs**

```c++
bool isBipartite(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> color(n);
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (!color[i]) {
            q.push(i);
            color[i] = 1;
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &v: g[u]) {
                if (!color[v]) {
                    q.push(v);
                    color[v] = 3 - color[u];
                } else if (color[v] == color[u]) return false;
            }
        }
    }
    return true;
}
```

### 并查集判定

```c++
struct DSU {
  public:
    DSU() : _n(0) {}
    explicit DSU(int n) : _n(n), parent_or_size(n, -1) {}

    int merge(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        int x = get(a), y = get(b);
        if (x == y) return x;
        if (-parent_or_size[x] < -parent_or_size[y]) std::swap(x, y);
        parent_or_size[x] += parent_or_size[y];
        parent_or_size[y] = x;
        return x;
    }

    bool same(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return get(a) == get(b);
    }

    int get(int a) {
        assert(0 <= a && a < _n);
        if (parent_or_size[a] < 0) return a;
        return parent_or_size[a] = get(parent_or_size[a]);
    }

    int size(int a) {
        assert(0 <= a && a < _n);
        return -parent_or_size[get(a)];
    }

    std::vector<std::vector<int>> groups() {
        std::vector<int> leader_buf(_n), group_size(_n);
        for (int i = 0; i < _n; i++) {
            leader_buf[i] = get(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<int>> result(_n);
        for (int i = 0; i < _n; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < _n; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(
            std::remove_if(result.begin(), result.end(),
                           [&](const std::vector<int>& v) { return v.empty(); }),
            result.end());
        return result;
    }

  private:
    int _n;
    // root node: -1 * component size
    // otherwise: parent
    std::vector<int> parent_or_size;
};

bool isBipartite(vector<vector<int>>& g) {
    int n = g.size();
    DSU d(n);
    for (int i = 0; i < n; ++i) {
        for (auto &v : g[i]) {
            if (d.same(i, v)) return false;
            d.merge(g[i][0], v);
        }
    }
    return true;
}
```