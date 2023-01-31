#include<bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    int n;
    vector<T> a;
    static int lg(unsigned x) {return x == 0 ? -1 : 31 - __builtin_clz(x);}
    FenwickTree(int n) : n(n), a(n) {}
    FenwickTree(vector<T> &A) : FenwickTree((int)A.size()) {
        for (int i = 0; i < n; ++i) add(i, A[i]);
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) a[i - 1] += v;
    }
    T sum(int x) {
        T ans = 0;
        for (int i = min(x + 1, n); i > 0; i -= i & -i) ans += a[i - 1];
        return ans;
    }
    T sum(int l, int r) { //sum[l..r]
        return l <= r ? sum(r) - sum(l - 1) : 0; 
    }
    T get(int x) const {   // 0 <= x < n, get a[x] in O(1), Equivalent to sum(x, x).
        T s = a[x];
        int y = x + 1;
        y -= y & -y;
        while (x != y) s -= a[x - 1], x -= x & -x;
        return s;
    }
    void set(int x, T v) {  // 0 <= x < n, set a[x] = v
        add(x, v - get(x));
    }
    int kth(T k) const {
        if (k < T()) return -1;
        int x = 0;
        for (int i = 1 << lg(n); i; i /= 2) {
            if (x + i <= n && k >= a[x + i - 1]) {
                x += i, k -= a[x - 1];
            }
        }
        return x;
    }
};


int main() {
    vector<int> a {3,6,2,7,10,4};
    FenwickTree<int> fen(a);

    // a[1] + a[2] + a[3] = 6 + 2 + 7 = 15
    cout << fen.sum(1, 3) << "\n";

    // a[4] = a[4] + 2
    fen.add(4, 2);

    // 10 + 2 = 12
    cout << fen.sum(4, 4) << "\n";

    int n = a.size();
    FenwickTree<int> f(15);
    for (int i = 0; i < n; ++i) {
        cout << a[i] << " \n"[i == n - 1];
    }
    for (int i = 0; i < n; ++i) {   
        f.set(a[i], 1);    // 插入a[i]
    }
    cout << f.get(2) << ", " << f.get(6) << "\n";   // 查询 2, 6 是否存在

    cout << f.sum(4) << ", " << f.sum(6) << ", " << f.sum(16) << "\n"; // 查找 小于等于x的数目

    for (int i = 0; i < n; ++i) {
        cout << f.kth(i) << " \n"[i == n - 1];       // 第k小的值，0 <= k < n
    }
}