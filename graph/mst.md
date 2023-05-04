# 最小生成树



### KruscalMST模板

依赖 DSU，本地 sumlime 补全缩写 `mst`


```c++
template <typename T, typename Comp>
struct KruscalMST {
    KruscalMST() : KruscalMST(0) {}
    explicit KruscalMST(const int n) : _n(n) {}

    void add_edge(const int u, const int v, const T& cost) { _built = false; es.emplace_back(u, v, cost);}
    void add_edge(const tuple<int, int, T>& e) { _built = false; es.push_back(e);}
    bool build() {
        _built = true, _weight_sum = 0;
        if (_n == 0) return true;
        DSU uf(_n);
        sort(es.begin(), es.end(), [this](const auto& u, const auto& v) { return _comp(std::get<2>(u), std::get<2>(v));});
        for (auto& [u, v, w] : es) {
            if (uf.same(u, v)) u = v = _n;
            else { uf.merge(u, v); _weight_sum += w;}
        }
        es.erase(std::remove_if(es.begin(), es.end(), [this](auto& e) { return std::get<0>(e) == _n; }), es.end());
        return int(es.size()) == _n - 1;
    }
    T get_weight() const { assert(_built); return _weight_sum;}
    const vector<tuple<int, int, T>>& get_mst() const { assert(_built); return es;}
private:
    int _n;
    T _weight_sum;
    Comp _comp{};
    vector<tuple<int, int, T>> es;
    bool _built = false;
};
template <typename T> using minMst = KruscalMST<T, less<T>>;
template <typename T> using maxMst = KruscalMST<T, greater<T>>;
```

### 使用说明

1. 创建一个 n 个节点的图

```c++
minMst<int> g(n);
```

2. 添加边

```c++
g.add_edge(u, v, w); // b.add_edge(u, v, 1);
```
或者

```c++
tuple<int, int, int> e;
g.add_edge(e);
```
3. 求最小生成树 时间复杂度 `O(E*log(E))`

```c++
g.build()
```

4. get_weight
 
+ 如果图连通，返回最小生成树的权重之和
+ 如果不连通，返回最小生成森林的权重之和

5. get_mst()

+ 如果图连通，返回最小生成树中的边
+ 如果不连通，返回最小生成森林中的边


### 模板check

[acwing 859](https://www.acwing.com/problem/content/861/)

给定一个 n 个点 m 条边的无向图，求最小生成树

+ 1 <= n <= 1e5
+ 1 <= m <= 2e5

```c++
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    minMst<int> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g.add_edge(--u, --v, w);    
    }
    
    if(g.build()) {
        cout << g.get_weight() << '\n';
    } else cout << "impossible\n";

    return 0;
}
```
