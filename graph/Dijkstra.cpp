#include<bits/stdc++.h>
using namespace std;

// problem : https://www.acwing.com/problem/content/852/

const int DN = 1.5e5+10;
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


int main() {
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    DIJ d;
    for (int i = 0, u, v, c; i < m; ++i) {
        cin >> u >> v >> c;
        d.add(u, v, c, 1);
    }
    cout << d.dijk(1, n) << "\n";
    return 0;
}
