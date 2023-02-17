#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// https://atcoder.jp/contests/abc222/tasks/abc222_f

// N = 5e5时，空间约 195M，空间紧张时用 https://github.com/nealwu/competitive-programming/blob/master/rmq_lca/block_rmq_mask.cc
template<typename T, bool maximum_mode = false>
struct RMQ {
    static int lg(unsigned x) { return x == 0 ? -1 : 31 - __builtin_clz(x);}
    int n = 0;
    vector<T> a;
    vector<vector<int>> mat;
    RMQ(const vector<T> &A = {}) { if (!A.empty()) build(A);}
    int op(int x, int y) const {
        return (maximum_mode ? a[y] < a[x] : a[x] < a[y]) ? x : y; // when `a[x] == a[y]`, returns y.
    }
    void build(const vector<T> &A) {
        a = A, n = int(a.size());
        int max_log = lg(n) + 1;
        mat.resize(max_log);
        for (int k = 0; k < max_log; k++) mat[k].resize(n - (1 << k) + 1);
        for (int i = 0; i < n; i++) mat[0][i] = i;
        for (int k = 1; k < max_log; k++)
            for (int i = 0; i <= n - (1 << k); i++)
                mat[k][i] = op(mat[k - 1][i], mat[k - 1][i + (1 << (k - 1))]);
    }
    int get_idx(int x, int y) const {  // 0 <= x < y <= n
        int k = lg(y - x);
        return op(mat[k][x], mat[k][y - (1 << k)]);
    }
    T get_val(int x, int y) const { return a[get_idx(x, y)];}
};
template<typename T>
struct wLCA {
    int n = 0, now_dfn = 0;
    vector<vector<pair<int,T>>> g;
    vector<int> fa, dep, siz, euler, dfn, ent, out,tour_ls, rev_tour_ls, hv_rt, hv_rt_dep, hv_rt_fa; 
    vector<T> w_dep, up_w; 
    RMQ<int> rmq;
    bool built = false;
    wLCA(int n):n(n),g(n),fa(n, -1),dep(n),siz(n),w_dep(n),up_w(n),dfn(n),ent(n),out(n),tour_ls(n),hv_rt(n) {}
    wLCA(const vector<vector<pair<int,T>>> &G) : wLCA(int(G.size())){ g = G;}

    void add_edge(int a, int b, T w) {
        g[a].emplace_back(b, w), g[b].emplace_back(a, w);
    }
    int degree(int v) const { return int(g[v].size()) + (built && fa[v] >= 0); }
    void erase_edge(int from, int to) {
        for (auto &e : g[from])
            if (e.first == to) {
                swap(e, g[from].back());
                g[from].pop_back();
                return;
            }
    }
    void dfs(int u, int pa, T w) {
        fa[u] = pa, dep[u] = pa < 0 ? 0 : dep[pa] + 1, siz[u] = 1, w_dep[u] = w;
        erase_edge(u, pa);
        for (auto &[v, c] : g[u]) {
            up_w[v] = c;
            dfs(v, u, w + c);
            siz[u] += siz[v];
        }
        sort(g[u].begin(), g[u].end(), [&](const auto &a, const auto &b) {
            return siz[a.first] > siz[b.first];
        });
    }
    void dfs1(int u, bool hv) {
        hv_rt[u] = hv ? hv_rt[fa[u]] : u;
        dfn[u] = int(euler.size());
        euler.push_back(u);
        tour_ls[now_dfn] = u, ent[u] = now_dfn++;
        bool hv_child = true;
        for (auto &[v, c] : g[u]) {
            dfs1(v, hv_child);
            euler.push_back(u);
            hv_child = false;
        }
        out[u] = now_dfn;
    };

    void build(int root = -1) {
        if (built) return;
        if (0 <= root && root < n) dfs(root, -1, 0);
        for (int i = 0; i < n; i++) if (i != root && fa[i] < 0)
            dfs(i, -1, 0);
        euler.reserve(2 * n);

        for (int i = 0; i < n; i++) if (fa[i] < 0) {
            dfs1(i, false);
            euler.push_back(-1);
        }
        assert(int(euler.size()) == 2 * n);
        vector<int> euler_dep;
        euler_dep.reserve(euler.size());
        for (int u : euler)
            euler_dep.push_back(u < 0 ? u : dep[u]);

        rmq.build(euler_dep);
        hv_rt_dep.resize(n), hv_rt_fa.resize(n);

        for (int i = 0; i < n; i++) {
            hv_rt_dep[i] = dep[hv_rt[i]];
            hv_rt_fa[i] = fa[hv_rt[i]];
        }
        rev_tour_ls = tour_ls;
        reverse(rev_tour_ls.begin(), rev_tour_ls.end());
        built = true;
    }

    pair<int, array<int, 2>> get_diameter() const {
        assert(built);
        pair<int, int> u_max = {-1, -1}, ux_max = {-1, -1};
        pair<int, array<int, 2>> uxv_max = {-1, {-1, -1}};
        for (int node : euler) {
            if (node < 0) break;
            u_max = max(u_max, {dep[node], node});
            ux_max = max(ux_max, {u_max.first - 2 * dep[node], u_max.second});
            uxv_max = max(uxv_max, {ux_max.first + dep[node], {ux_max.second, node}});
        }
        return uxv_max;
    }
    array<int, 2> get_center() const {
        pair<int, array<int, 2>> diam = get_diameter();
        int length = diam.first, a = diam.second[0], b = diam.second[1];
        return {get_kth_node_on_path(a, b, length / 2), get_kth_node_on_path(a, b, (length + 1) / 2)};
    }
    pair<T, array<int, 2>> get_w_diameter() const {  // 获取带权图的直径
        assert(built);
        pair<T, int> u_max = {-1, -1}, ux_max = {-1, -1};
        pair<T, array<int, 2>> uxv_max = {-1, {-1, -1}};
        for (int node : euler) {
            if (node < 0) break;
            u_max = max(u_max, {w_dep[node], node});
            ux_max = max(ux_max, {u_max.first - 2 * w_dep[node], u_max.second});
            uxv_max = max(uxv_max, {ux_max.first + w_dep[node], {ux_max.second, node}});
        }
        return uxv_max;
    }
    int get_lca(int a, int b) const {
        a = dfn[a], b = dfn[b];
        if (a > b) swap(a, b);
        return euler[rmq.get_idx(a, b + 1)];
    }

    bool is_anc(int a, int b) const { return ent[a] <= ent[b] && ent[b] < out[a];}
    bool on_path(int x, int a, int b) const {return (is_anc(x, a) || is_anc(x, b)) && is_anc(get_lca(a, b), x);}

    int get_dist(int a, int b) const { return dep[a] + dep[b] - 2 * dep[get_lca(a, b)];}
    T get_w_dist(int a, int b) const {
        return w_dep[a] + w_dep[b] - 2 * w_dep[get_lca(a, b)];
    }
    int child_anc(int a, int b) const {
        assert(a != b && is_anc(a, b));
        int child = euler[rmq.get_idx(dfn[a], dfn[b] + 1) + 1];
        return child;
    }

    int get_kth_anc(int a, int k) const {
        if (k > dep[a]) return -1;
        int goal = dep[a] - k;
        while (hv_rt_dep[a] > goal)
            a = hv_rt_fa[a];
        return tour_ls[ent[a] + goal - dep[a]];
    }

    int get_kth_node_on_path(int a, int b, int k) const {
        int anc = get_lca(a, b);
        int ls = dep[a] - dep[anc], rs = dep[b] - dep[anc];
        if (k < 0 || k > ls + ls) return -1;

        return k < ls ? get_kth_anc(a, k) : get_kth_anc(b, ls + rs - k);
    }
    int get_common_node(int a, int b, int c) const {
        return get_lca(a, b) ^ get_lca(b, c) ^ get_lca(c, a);
    }

    vector<pair<int, int>> compress_tree(vector<int> nodes) const {
        if (nodes.empty()) return {};

        auto &&comp = [&](int a, int b) { return ent[a] < ent[b]; };
        sort(nodes.begin(), nodes.end(), comp);
        int k = int(nodes.size());
        for (int i = 0; i < k - 1; i++)
            nodes.push_back(get_lca(nodes[i], nodes[i + 1]));
        sort(nodes.begin() + k, nodes.end(), comp);
        inplace_merge(nodes.begin(), nodes.begin() + k, nodes.end(), comp);
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
        vector<pair<int, int>> res = {{nodes[0], -1}};
        for (int i = 1; i < int(nodes.size()); i++)
            res.emplace_back(nodes[i], get_lca(nodes[i], nodes[i - 1]));
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, u, v, w;
    cin >> n;

    wLCA<long long> g(n * 2);

    for (int i = 1; i < n ; ++i) {
        cin >> u >> v >> w;
        g.add_edge(u - 1, v - 1, w);
    }
    
    for (int i = 0; i < n; ++i) {
        cin >> w;
        g.add_edge(i, i + n, w);
    }

    g.build();

    auto diameter = g.get_w_diameter();
    u = diameter.second[0], v = diameter.second[1];

    for (int i = 0; i < n; ++i) {
        auto x = i == (u % n) ? 0ll : g.get_w_dist(i, u), y = i == (v % n) ? 0ll : g.get_w_dist(i, v);
        cout << max(x, y) << "\n";
    }
    return 0;
}
