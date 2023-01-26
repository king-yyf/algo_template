#include<bits/stdc++.h>
using namespace std;

// problem: https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G
// sigmayyf

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

template <typename T>
struct RangeSumTree {
	FenwickTree<T> a, b;
	RangeSumTree(int N) : a(N + 1), b(N + 1) {}

	// add x to [l, r]
	void add(int l, int r, const T &x) { 
		a.add(l, x), a.add(r + 1, -x);
		b.add(l, x * (1 - l)), b.add(r + 1, x * r);
	}

	// sum of [l, r]  0 <= l <= r < n
	T sum(int l, int r) {
		r++;
		return a.ask(r + 1) * r + b.ask(r + 1) - a.ask(l + 1) * l - b.ask(l + 1);
	}
};

int main() {
	ios::sync_with_stdio(false); 
	cin.tie(nullptr);
	cout << fixed << setprecision(10);

	int n, q;
	cin >> n >> q;
	RangeSumTree<long long> t(n);
	int op, x, y, c;
	for (int i = 0; i < q; ++i) {
		cin >> op;
		if (op == 0) {
			x--, y--;
			cin >> x >> y >> c;
			t.add(x, y, c);


		} else {
			cin >> x >> y;
			x--,y--;
			cout << t.sum(x, y) << "\n";
		}
	}
}