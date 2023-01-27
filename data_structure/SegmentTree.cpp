#include<bits/stdc++.h>
using namespace std;

/*
problem: https://codeforces.com/edu/course/2/lesson/4/2/practice/contest/273278/problem/D
*/

template <class S, S (*op)(S, S), S (*e)()>
struct SegTree {
    int n, size, log;
    vector<S> d;
    SegTree() : SegTree(0) {}
    explicit SegTree(int n) : SegTree(vector<S>(n, e())) {}
    explicit SegTree(const vector<S> &v) : n(int(v.size())) {
        log = ceil_pow2(n), size = 1 << log;
        d = vector<S>(2 * size, e());
        for (int i = 0; i < n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) pull(i);
    }

    int ceil_pow2(int n) {  // minimum non-neg x s.t. `n <= 2^x`
        int x = 0;
        while ((1U << x) < (unsigned int)(n)) x++;
        return x;
    }
    void set(int p, S x) {  // assert(0 <= p < n)
        p += size, d[p] = x;
        for (int i = 1; i <= log; ++i) pull(p >> i);
    }
    S get(int p) const { return d[p + size];}
    S get(int l, int r) {   // [l, r)
        S sl = e(), sr = e();
        l += size, r += size;
        while (l < r) {
            if (l & 1) sl = op(sl, d[l++]);
            if (r & 1) sr = op(d[--r], sr);
            l >>= 1, r >>= 1;
        }
        return op(sl, sr);
    }
    S get_all() const {return d[1];}

    void pull(int k) {
        d[k] = op(d[2 * k], d[2 * k + 1]);
    }
    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F>     // 0 <= l <= n, f(e()) is true
    int max_right(int l, F f) const { 
        if (l == n) return n;
        l += size;
        S x = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(x, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(x, d[l]))) {
                        x = op(x, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            x = op(x, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }
    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F>  // 0 <= r <= n, f(e()) is true
    int min_left(int r, F f) const {
        if (r == 0) return 0;
        r += size;
        S x = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], x))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], x))) {
                        x = op(d[r], x);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            x = op(d[r], x);
        } while ((r & -r) != r);
        return 0;
    }
};

using S = int;

S op(S x, S y) {
    S res;
    res = max(x, y);
    return res;
}


S e() {
    return -1;
}
int target;
bool f(S x) {
    return x < target;
}

int main() {
	ios::sync_with_stdio(false); 
	cin.tie(nullptr);
	cout << fixed << setprecision(10);

    int n, q;
    cin >> n >> q;
    vector<S> a(n);
    long long x;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    SegTree<S,op,e> seg(a);

    for (int i = 0, op, x, y; i < q; ++i) {
        cin >> op;
        if (op == 1) {
            cin >> x >> y;
            seg.set(x, y);
        } else {
            cin >> target>>y;
            if (seg.get_all() < target) {
                cout << "-1\n";
            }else{
                int p = seg.max_right<f>(y);
                cout<<(p==n?-1:p)<<"\n";
            }
        }
    }
}