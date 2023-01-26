#include<bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    int n;
    vector<T> a;
    FenwickTree(int n) : n(n), a(n) {}
    FenwickTree(vector<T> &A) : FenwickTree((int)A.size()) {
        for (int i = 0; i < n; ++i) add(i, A[i]);
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) a[i - 1] += v;
    }
    T ask(int x) {
        T ans = 0;
        for (int i = min(x + 1, n); i > 0; i -= i & -i) ans += a[i - 1];
        return ans;
    }
    T ask(int l, int r) { //sum[l..r]
        if (l > r) return 0;
        return ask(r) - ask(l - 1);
    }
};


int main() {
    vector<int> a {3,6,2,7,10,4};
    FenwickTree<int> fen(a);

    // a[1] + a[2] + a[3] = 6 + 2 + 7 = 15
    cout << fen.ask(1, 3) << "\n";

    // a[4] = a[4] + 2
    fen.add(4, 2);

    // 10 + 2 = 12
    cout << fen.ask(4, 4) << "\n";
}