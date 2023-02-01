#include<bits/stdc++.h>
using namespace std;

// problem : https://www.acwing.com/problem/content/submission/854/



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

int main() {
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);
    int n,m;
    cin>>n>>m;
    SPFA<int> d(n);
    for(int i=0,x,y,z;i<m;++i){
        cin>>x>>y>>z;
        x--,y--;
        d.add_edge(x,y,z);
    }
    cout << (d.cycle_detect() ? "Yes" : "No") << "\n";
}