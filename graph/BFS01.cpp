#include<bits/stdc++.h>
using namespace std;

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


int main() {
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    BFS01<int> d(n);
    Dijkstra<int> d1(n);
    for (int i = 0, u, v, c; i < m; ++i) {
        cin >> u >> v >> c;
        u--, v--;
        d.add_edge(u, v, c);
        d1.add_edge(u, v, c);
    }
    auto ans = d.bfs(0), c = d1.dijkstra(0);
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << ", " << c[i] << "\n";    
    }
    return 0;
}
