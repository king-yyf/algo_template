#include<bits/stdc++.h>
using namespace std;

template <typename T>
struct RangeAddTree {
    int n;
    vector<T> a;
    RangeAddTree(int N) : n(N), a(N) {}
    RangeAddTree(vector<T> &A) : RangeAddTree((int)A.size()) {
    	for (int i = 1; i <= n; i++) {
    		a[i - 1] = A[i - 1] - (i > 1 ? A[i - 2] : 0);
    		for (int k = (i & -i) >> 1; k > 0; k >>= 1) 
    			a[i - 1] += a[i - k - 1];
    	}   
    }
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) a[i - 1] += v;
    }
    void add(int l, int r, const T &v) {  // [l, r]
        add(l, v), add(r + 1, -v);
    }
    T get(int x) const {
        T ans = 0;
        for (int i = min(x + 1, n); i > 0; i -= i & -i) ans += a[i - 1];
        return ans;
    }
};


int main() {
	vector<int> a = {3,6,2,8,4};
	RangeAddTree<int> f(a);
	int n = a.size();

	cout << "initial array : \n";
	for (int i = 0; i < n; ++i) {
		cout << f.get(i) << ", " << a[i] << "\n";
	}

	int l = 2, r = 6, v = 3;
	for (int i = l; i <= r; ++i) {
		a[i] += v;
	}
	f.add(l, r, v);


	cout << "\nafter add " << v << " on [" << l << ", " << r << "] : \n";
	for (int i = 0; i < n; ++i) {
		cout << f.get(i) << ", " << a[i] << "\n";
	}

	l = 0, r = 3, v = -1;

	for (int i = l; i <= r; ++i) {
		a[i] += v;
	}
	f.add(l, r, v);

	cout << "\nafter add " << v << " on [" << l << ", " << r << "] : \n";

	for (int i = 0; i < n; ++i) {
		cout << f.get(i) << ", " << a[i] << "\n";
	}
}