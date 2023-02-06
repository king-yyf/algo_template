#include <bits/stdc++.h>
using namespace std;

// https://codeforces.com/contest/1730/problem/E

const int MX = 1e6 + 5;
vector<int> divs[MX];

template <typename T, class LF, class RF>
void monoStack(vector<T> &a, vector<int> &lt, vector<int> &rt, LF f1, RF f2) {
    int n = a.size();
    lt.assign(n, -1), rt.assign(n, n);
    stack<int> sk;
    for (int i = 0; i < n; ++i) {
        while(!sk.empty() && f1(a[i],a[sk.top()])) {
            rt[sk.top()] = i;
            sk.pop();
        }
        sk.push(i);
    }
    sk = stack<int>();
    for (int i = n - 1; i >= 0; --i) {
        while (!sk.empty() && f2(a[i],a[sk.top()])) {
            lt[sk.top()] = i;
            sk.pop();
        }
        sk.push(i);
    }
}
template <typename T>
void le_le(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, less<T>(), less<T>());
}
template <typename T>
void le_le_eq(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, less<T>(), less_equal<T>());
}
template <typename T>
void gt_gt(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, greater<T>(), greater<T>());
}
template <typename T>
void gt_gt_eq(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, greater<T>(), greater_equal<T>());
}

void solve() {
    int n, m;
    cin >> n;
    vector<int> a(n);
    
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        m = max(a[i], m);
    }
    vector<vector<int>> pos(m + 1);
    vector<int> ind(m + 1);

    for (int i = 0; i < n; ++i) 
        pos[a[i]].push_back(i);

    vector<int> lmx, rmx, lmn, rmn;

    le_le(a,lmn,rmn);
    gt_gt_eq(a,lmx,rmx);
    
    long long ans = 0;

    for (int i = 0; i < n; ++i) {
        for (int x : divs[a[i]]) {
            if (ind[x] >= 1) {
                int j = pos[x][ind[x] - 1];
                if (j > lmx[i] && i < rmn[j]) {
                    ans += (j - max(lmx[i], lmn[j])) * 1ll * (min(rmx[i], rmn[j]) - i);
                }
            }

            if (ind[x] < pos[x].size()) {
                int j = pos[x][ind[x]];
                if (j < rmx[i] && lmn[j] < i) {
                    int t = ind[x] >= 1 ? pos[x][ind[x] - 1] : -1;
                    ans += (i - max({lmx[i], lmn[j], t})) * 1ll * (min(rmx[i], rmn[j]) - j); 
                }
            }
        }
        ind[a[i]]++;
    }
    cout << ans << "\n";

}

int main() {
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);

    for (int i = 1; i < MX; ++i) {
        for (int j = i; j < MX; j += i) {
            divs[j].push_back(i);
        }
    }
    int t;
    cin >> t;
    while(t--){
        solve();
    }
}