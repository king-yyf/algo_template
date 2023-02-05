#include<bits/stdc++.h>
using namespace std;

// https://www.acwing.com/problem/content/description/364/

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

int main() {
	ios::sync_with_stdio(false); 
	cin.tie(nullptr);

	SPFA<int> s(500002);

	int n;
	cin >> n;
	for (int i = 1; i <= 50001; ++i) {
		s.add_edge(i - 1, i, 0);
		s.add_edge(i, i - 1, -1);
	}

	for (int i = 0; i < n; ++i) {
		int a, b, c;
		cin >> a >> b >> c;
		a++, b++;
		s.add_edge(a - 1, b, c);	
	}

	auto d = s.spfa_max(0);

	cout << d[50001] << "\n";
}