#include<bits/stdc++.h>
using namespace std;

/*
https://codeforces.com/edu/course/2/lesson/4/3/practice/contest/274545/problem/E

区间和
*/


template <class S,           // 线段树维护的数据信息
          S (*op)(S, S),    // 左右子节点信息合并到当前节点
          S (*e)(),
          class F,          // 懒标记维护的信息
          S (*tag)(F, S),  // 查询时给当去节点打上懒标记
          F (*merge)(F, F),  // 懒标记合并
          F (*id)()>        // 懒标记的默认值, 用于清空父节点的懒标记
struct LazySegTree {
    int n, size, log;
    vector<S> d;
    vector<F> lz;
    void pull(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void push_down(int k, F f) {
        d[k] = tag(f, d[k]);
        if (k < size) lz[k] = merge(f, lz[k]);
    }
    void push(int k) {
        push_down(2 * k, lz[k]), push_down(2 * k + 1, lz[k]);
        lz[k] = id();
    }
    LazySegTree() : LazySegTree(0) {}
    explicit LazySegTree(int N) : LazySegTree(vector<S>(N, e())) {}
    explicit LazySegTree(const vector<S>& v) : n(int(v.size())) {
        log = ceil_lg(n), size = 1 << log;
        d = vector<S>(2 * size, e()), lz = vector<F>(size, id());
        for (int i = 0; i < n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) pull(i);
    }
    int ceil_lg(int n) {   // minimum non-neg x s.t. `n <= 2^x`
        int x = n == 0 ? 0 :  32 - __builtin_clz(n);
        return  __builtin_popcount(n) == 1 ? x - 1: x;
    }
    void set(int p, S x) {   // 0 <= p < n
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) pull(p >> i);
    }
    S get(int p) {     // Assert 0 <= p < n
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }
    S get(int l, int r) {   // op(a[l], ..., a[r - 1])
        if (l == r) return e();
        l += size, r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        S sl = e(), sr = e();
        while (l < r) {
            if (l & 1) sl = op(sl, d[l++]);
            if (r & 1) sr = op(d[--r], sr);
            l >>= 1, r >>= 1;
        }
        return op(sl, sr);
    }
    S get_all() { return d[1]; }
    void apply(int p, F f) {   // 0 <= p < n
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = tag(f, d[p]);
        for (int i = 1; i <= log; i++) pull(p >> i);
    }
    void apply(int l, int r, F f) {  // a[i] = f(a[i]), [l, r)
        if (l == r) return;
        l += size, r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        while (l < r) {
            if (l & 1) push_down(l++, f);
            if (r & 1) push_down(--r, f);
            l >>= 1, r >>= 1;
        }
        l = l2, r = r2;
        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) pull(l >> i);
            if (((r >> i) << i) != r) pull((r - 1) >> i);
        }
    }
    template <bool (*g)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template <class G> int max_right(int l, G g) { // 0 <= l <= n, g(e()) is true
        if (l == n) return n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) sm = op(sm, d[l]), l++;
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template <bool (*g)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template <class G> int min_left(int r, G g) { // 0 <= r <= n, g(e()) is true
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm)))  sm = op(d[r], sm), r--;
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};

struct S {
	long long sum;
	int size;
};
using F = long long;
S op(S x, S y) {
    S s;
    s = S{x.sum + y.sum, x.size + y.size};
    return s;
}
S e() {
    return S();
};
S tag(F f, S s) { return S{s.sum + f * s.size, s.size}; }
F merge(F x, F y) { return x + y; }
F id() { return 0; }
// LazySegTree<S, op, e, F, tag, merge, id> seg(vector<S>(n,{0,1}));

int main() {
	ios::sync_with_stdio(false); 
	cin.tie(nullptr);

	int n, m;
	cin >> n >> m;

	LazySegTree<S, op, e, F, tag, merge, id> seg(vector<S>(n,{0,1}));

    for (int i = 0, t; i < m; ++i) {
        cin >> t;
        if (t == 1) {
            int l, r, v;
            cin >> l >> r >> v;
            seg.apply(l, r, v);
        } else {
            int l;
            cin >> l;
            cout << seg.get(l).sum << "\n";
        }
    }
	
}
