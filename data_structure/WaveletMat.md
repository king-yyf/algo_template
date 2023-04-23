# 小波矩阵

小波树常用于解决区间查询，以及求区间分割点(Quantile，分位点)的问题。属于"简洁数据结构"(Succinct Data Structure) 的一种。
Wavelet Tree的复杂度是基于字母表σ的，因此如果我们要考虑的不只是字符串，而是数列，这个时间和空间消耗会非常恐怖。因此我们引入了matrix的方法来优化，时间和空间都更加优秀。假如数组中不同元素的个数有N个，则区间查询的时间复杂度为log(n), 当数组中数据值的范围较大时需要先进行离散化。

Wavelet Matrix在算法竞赛中是朴素Wavelet Tree的上位替代

### 模板代码

```c++
template <typename ...Types>
using constraints_t = std::enable_if_t<std::conjunction_v<Types...>, std::nullptr_t>;
template <typename ReturnType, typename Callable, typename ...Args>
using is_invoke_res = std::is_same<std::invoke_result_t<Callable, Args...>, ReturnType>;
class BitVec {
    using u8 = std::uint8_t;
    public:
        explicit BitVec(int n) : n(n), nl((n >> LOG_BLOCK_L) + 1), ns((n >> LOG_BLOCK_S) + 1), cum_l(nl), cum_s(ns), bits(ns) {}
        BitVec() : BitVec(0) {}
        template <typename Gen, constraints_t<is_invoke_res<bool, Gen, int>> = nullptr>
        BitVec(int n, Gen gen) : BitVec(n) { build(gen); }
        BitVec& operator=(const BitVec &bv) {
            n = bv.n, nl = bv.nl, ns = bv.ns, cum_l = bv.cum_l, cum_s = bv.cum_s, bits = bv.bits;
            return *this;
        }
        BitVec& operator=(BitVec &&bv) {
            n = bv.n, nl = bv.nl, ns = bv.ns, cum_l = std::move(bv.cum_l), cum_s = std::move(bv.cum_s), bits = std::move(bv.bits);
            return *this;
        }
        template <typename Gen, constraints_t<is_invoke_res<bool, Gen, int>> = nullptr>
        void build(Gen gen) {
            int i = 0;
            for (int index_s = 1; index_s < ns; ++index_s) {
                int count = cum_s[index_s - 1];
                for (; i < index_s << LOG_BLOCK_S; ++i) {
                    bool b = gen(i);
                    bits[index_s - 1] |= b << (i & MASK_S);
                    count += b;
                }
                if (index_s & ((1 << (LOG_BLOCK_L - LOG_BLOCK_S)) - 1)) cum_s[index_s] = count;                    
                else {
                    int index_l = i >> LOG_BLOCK_L;
                    cum_l[index_l] = cum_l[index_l - 1] + count;
                }
            }
            for (; i < n; ++i) bits[ns - 1] |= gen(i) << (i & MASK_S);
        }
        inline bool operator[](int i) const { return (bits[i >> LOG_BLOCK_S] >> (i & MASK_S)) & 1;}
        inline bool get(int i) const { return (*this)[i];} // returns the i'th val (i: 0-indexed)
        // returns the number of val in [0, i)
        inline int rank(bool val, int i) const {
            int res_1 = cum_l[i >> LOG_BLOCK_L] + cum_s[i >> LOG_BLOCK_S] + popcount8(bits[i >> LOG_BLOCK_S] & ((1 << (i & MASK_S)) - 1));
            return val ? res_1 : i - res_1;
        }
        // returns the number of val in [l, r)
        inline int rank(bool val, int l, int r) const { return rank(val, r) - rank(val, l);}
        // find the index of num'th val. (num: 1-indexed). if not exists, returns default_value.
        int select(bool val, int num, int default_value = -1) const {
            int l = -1, r = n + 1;
            while (r - l > 1) {
                int m = (l + r) >> 1;
                (rank(val, m) >= num ? r : l) = m;
            }
            return r == n + 1 ? default_value : r;
        }
    private:
        static constexpr int LOG_BLOCK_L = 8, LOG_BLOCK_S = 3, MASK_S = (1 << LOG_BLOCK_S) - 1;
        int n, nl, ns;
        std::vector<int> cum_l;
        std::vector<u8> cum_s, bits;

        static constexpr u8 popcount8(u8 x) {
            x = (x & 0b01010101) + ((x >> 1) & 0b01010101);
            x = (x & 0b00110011) + ((x >> 2) & 0b00110011);
            return (x & 0b00001111) + (x >> 4);
        }
};

template <typename T, int bit_num = std::numeric_limits<std::make_unsigned_t<T>>::digits>
class WaveletMatrix {
    public:
        WaveletMatrix() noexcept : n(0) {}
        template <typename Gen, constraints_t<is_invoke_res<T, Gen, int>> = nullptr>
        WaveletMatrix(int n, Gen generator) : n(n) { build(generator);}
        template <typename U, constraints_t<std::is_constructible<T, U>> = nullptr>
        WaveletMatrix(const std::vector<U> &a) : WaveletMatrix(a.size(), [&a](int i) { return T(a[i]); }) {}
        template <typename Gen, constraints_t<is_invoke_res<T, Gen, int>>  = nullptr>
        void build(Gen generator) {
            std::vector<T> a(n), l(n), r(n);
            for (int i = 0; i < n; ++i) a[i] = generator(i);
            for (int log = bit_num - 1; log >= 0; --log) {
                bv[log] = BitVec(n, [&a, log](int i) -> bool { return (a[i] >> log) & 1; });
                int li = 0, ri = 0;
                for (int i = 0; i < n; ++i) 
                    ((a[i] >> log) & 1 ? r[ri++] : l[li++]) = a[i];
                a.swap(l);
                std::copy(r.begin(), r.begin() + ri, a.begin() + li);
                mid[log] = li;
            }
        }
        T operator[](int i) const {  // returns WaveletMatrix[i]
            T res = 0;
            for (int log = bit_num - 1; log >= 0; --log) {
                bool b = bv[log][i];
                res |= T(b) << log;
                i = b * mid[log] + bv[log].rank(b, i);
            }
            return res;
        }
        inline T get(int i) const { return (*this)[i];}  // returns WaveletMatrix[i]
        int rank(T x, int i) const {  // [0, i)中x的数量
            check_index_bounds(x);
            int l = 0, r = i;
            for (int log = bit_num - 1; log >= 0; --log) succ(l, r, (x >> log) & 1, log);
            return r - l;
        }
        // [l, r) 中第k小 k从0开始
        T range_kth_min(int l, int r, int k, T default_value = T(-1)) const {
            if (k < 0 or k >= r - l) return default_value;
            T res = 0;
            for (int log = bit_num - 1; log >= 0; --log) {
                int cnt_0 = bv[log].rank(false, l, r);
                bool bit = k >= cnt_0;
                succ(l, r, bit, log);
                res |= T(bit) << log;
                k -= bit * cnt_0;
            }
            return res;
        }
        // [l, r) 中第k大 k从0开始
        inline T range_kth_max(int l, int r, int k, T default_value = T(-1)) const {
            return range_kth_min(l, r, r - l - 1 - k, default_value);
        }
        inline T range_min(int l, int r) const { // [l, r)中最小值
            assert(l < r); return range_kth_min(l, r, 0);
        }
        inline T range_max(int l, int r) const { // [l, r)中最大值
            assert(l < r); return range_kth_max(l, r, 0);
        }
        // [l, r) 中小于upper的元素数量
        int range_freq(int l, int r, T upper) const {
            if (r <= l) return 0;
            check_index_bounds(upper);
            int res = 0;
            for (int log = bit_num - 1; log >= 0; --log) {
                bool b = (upper >> log) & 1;
                if (b) res += bv[log].rank(false, l, r);
                succ(l, r, b, log);
            }
            return res;
        }
        // [l, r) 中在区间 [lower, upper) 的元素数量
        inline int range_freq(int l, int r, T lower, T upper) const {
            return range_freq(l, r, upper) - range_freq(l, r, lower);
        }
        // [l, r)中 满足lower <= v 的最小的v
        inline T range_min_geq(int l, int r, T lower, T default_value = T(-1)) const {
            int cnt = range_freq(l, r, lower);
            return cnt >= r - l ? default_value : range_kth_min(l, r, cnt);
        }
        // // [l, r)中 满足lower < v 的最小的v
        inline T range_min_gt(int l, int r, T lower, T default_value = T(-1)) const {
            return lower == MAX ? default_value : range_min_geq(l, r, lower + 1);
        }
        // [l, r)中 满足 v < upper 的最大的v 
        inline T range_max_lt(int l, int r, T upper, T default_value = T(-1)) const {
            int cnt = range_freq(l, r, upper);
            return cnt == 0 ? default_value : range_kth_min(l, r, cnt - 1);
        }
        // [l, r)中 满足 v <= upper 的最大的v 
        inline T range_max_leq(int l, int r, T upper, T default_value = T(-1)) const {
            if (r >= l) return default_value;
            return upper == MAX ? range_max(l, r) : range_max_lt(l, r, upper + 1);
        }
    protected:
        WaveletMatrix(int n) noexcept : n(n) {}
    private:
        static_assert(bit_num > 0);
        static constexpr T MAX = bit_num == std::numeric_limits<T>::digits ? std::numeric_limits<T>::max() : (T(1) << bit_num) - 1;
        const int n;
        std::array<BitVec, bit_num> bv;
        std::array<int, bit_num> mid;

        inline void succ(int &l, int &r, const bool b, const int log) const {
            l = b * mid[log] + bv[log].rank(b, l), r = b * mid[log] + bv[log].rank(b, r);
        }
        static constexpr void check_index_bounds(T val) {
            assert((val >> bit_num) == 0);
        }
};

template <typename T>
class CoordinateCompressorBuilder {
    public:
        struct Compressor {
                static constexpr int absent = -1;
                Compressor() : _xs(std::vector<T>{}) {}
                Compressor(const std::vector<T> &xs) : _xs(xs) { assert(is_strictly_sorted(xs));}
                int size() const { return _xs.size();}
                int comp(const T &e, int default_value = absent) const {
                    const int res = min_geq_index(e);
                    return res != size() and _xs[res] == e ? res : default_value;
                }
                T decomp(const int compres_idx) const { return _xs[compres_idx];}
                int operator[](const T &e) const { return comp(e);}
                int min_geq_index(const T &e) const {
                    return lower_bound(_xs.begin(), _xs.end(), e) - _xs.begin();
                }
            private:
                std::vector<T> _xs;
                static bool is_strictly_sorted(const std::vector<T> &v) {
                    return std::adjacent_find(v.begin(), v.end(), std::greater_equal<T>()) == v.end();
                }
        };
        CoordinateCompressorBuilder() : _xs(std::vector<T>{}) {}
        explicit CoordinateCompressorBuilder(const std::vector<T> &xs) : _xs(xs) {}
        explicit CoordinateCompressorBuilder(std::vector<T> &&xs) : _xs(std::move(xs)) {}
        template <typename Gen, constraints_t<is_invoke_res<T, Gen, int>> = nullptr>
        CoordinateCompressorBuilder(const int n, Gen generator) {
            reserve(n);
            for (int i = 0; i < n; ++i) push(generator(i));
        }
        void reserve(int n) { _xs.reserve(n);}
        void push(const T &first) { _xs.push_back(first);}
        void push(T &&first) {_xs.push_back(std::move(first));}
        template <typename Iterator>
        auto push(const Iterator &first, const Iterator &last) -> decltype(std::vector<T>{}.push_back(*first), void()) {
            for (auto it = first; it != last; ++it) _xs.push_back(*it);
        }
        template <typename Iterable>
        auto push(const Iterable &iterable) -> decltype(std::vector<T>{}.push_back(*iterable.begin()), void()) {
            push(iterable.begin(), iterable.end());
        }
        template <typename ...Args>
        void emplace(Args &&...args) {_xs.emplace_back(std::forward<Args>(args)...);}
        auto build() {
            std::sort(_xs.begin(), _xs.end()), _xs.erase(std::unique(_xs.begin(), _xs.end()), _xs.end());
            return Compressor {_xs};
        }
        static auto build(const std::vector<T> &xs) { return CoordinateCompressorBuilder(xs).build();}
        static auto build(std::vector<T> &&xs) {
            return CoordinateCompressorBuilder(std::move(xs)).build();
        }
        template <typename Gen, constraints_t<is_invoke_res<T, Gen, int>> = nullptr>
        static auto build(const int n, Gen generator) {
            return CoordinateCompressorBuilder<T>(n, generator).build();
        }
    private:
        std::vector<T> _xs;
};

template <typename T, int log_max_len = std::numeric_limits<std::make_unsigned_t<T>>::digits>
class CompressedWaveletMatrix : public WaveletMatrix<int, log_max_len> {
    public:
        CompressedWaveletMatrix() noexcept : WaveletMatrix<int, log_max_len>(0) {}
        template <typename Gen, constraints_t<is_invoke_res<T, Gen, int>> = nullptr>
        CompressedWaveletMatrix(int n, Gen generator) : WaveletMatrix<int, log_max_len>(n), comp(CoordinateCompressorBuilder<T>::build(n, generator)) {
            this->build([this, &generator](int i) { return comp[generator(i)]; });
        }
        template <typename U, constraints_t<std::is_constructible<T, U>> = nullptr>
        CompressedWaveletMatrix(const std::vector<U> &a) : CompressedWaveletMatrix(a.size(), [&a](int i) { return T(a[i]); }) {}
        inline T operator[](int i) const {
            return comp.decomp(WaveletMatrix<int, log_max_len>::operator[](i));
        }
        inline T get(int i) const {
            return (*this)[i];
        }
        inline int rank(T val, int i) const {
            int x = comp.comp(val, -1);
            if (x == -1) return 0; 
            return WaveletMatrix<int, log_max_len>::rank(x, i);
        }
        inline T range_kth_min(int l, int r, int k, T default_value = T(-1)) const {
            int x = WaveletMatrix<int, log_max_len>::range_kth_min(l, r, k, -1);
            return x == -1 ? default_value : comp.decomp(x);
        }
        inline T range_kth_max(int l, int r, int k, T default_value = T(-1)) const {
            int x = WaveletMatrix<int, log_max_len>::range_kth_max(l, r, k, -1);
            return x == -1 ? default_value : comp.decomp(x);
        }
        inline T range_min(int l, int r) const {
            return comp.decomp(WaveletMatrix<int, log_max_len>::range_min(l, r));
        }
        inline T range_max(int l, int r) const {
            return comp.decomp(WaveletMatrix<int, log_max_len>::range_max(l, r));
        }
        inline int range_freq(int l, int r, T upper) const {
            return WaveletMatrix<int, log_max_len>::range_freq(l, r, comp.min_geq_index(upper));
        }
        inline int range_freq(int l, int r, T lower, T upper) const {
            return range_freq(l, r, upper) - range_freq(l, r, lower);
        }
        // returns the minimum value v in WaveletMatrix[l, r) s.t. lower <= v
        inline T range_min_geq(int l, int r, T lower, T default_value = T(-1)) const {
            int x = WaveletMatrix<int, log_max_len>::range_min_geq(l, r, comp.min_geq_index(lower), -1);
            return x == -1 ? default_value : comp.decomp(x);
        }
        // returns the minimum value v in WaveletMatrix[l, r) s.t. lower < v
        inline T range_min_gt(int l, int r, T lower, T default_value = T(-1)) const {
            return lower == std::numeric_limits<T>::max() ? default_value : range_min_geq(l, r, lower + 1, default_value);
        }
        // returns the maximum value v in WaveletMatrix[l, r) s.t. v < upper
        inline T range_max_lt(int l, int r, T upper, T default_value = T(-1)) const {
            int x = WaveletMatrix<int, log_max_len>::range_max_lt(l, r, comp.min_geq_index(upper), -1);
            return x == -1 ? default_value : comp.decomp(x);
        }
        // returns the maximum value v in WaveletMatrix[l, r) s.t. v <= upper
        inline T range_max_leq(int l, int r, T upper, T default_value = T(-1)) const {
            if (r >= l) return default_value;
            return upper == std::numeric_limits<T>::max() ? range_max(l, r) : range_max_lt(l, r, upper + 1, default_value);
        }
    private:
        typename CoordinateCompressorBuilder<T>::Compressor comp;
};
```

### 使用说明

假设字母表大小为N，(即数组中包含的不同元素个数)

1. 定义一个 WaveletMatrix / CompressedWaveletMatrix

如果不需要离散化，选择 WaveletMatrix， 需要离散化时选择 CompressedWaveletMatrix

```c++
vector<int> a;
WaveletMatrix<int, max_log>， wm(a);
CompressedWaveletMatrix<int, max_log>， wm(a);
```

2. 查询区间 [l, r) 中 x出现的次数 时间复杂度 O(log(N))

```c++
int cnt = wm.rank(x, l, r);
```

3. 查询区间 [0, i) 中 x出现的次数 时间复杂度 O(log(N))

```c++
int cnt = wm.rank(x, i);
```

4. 查询区间 [l, r) 中第k小的数, k从0开始，时间复杂度 O(log(N))

```c++
int res = wm.range_kth_min(l, r, k);
```

5. 查询区间 [l, r) 中第k大的数, k从0开始，时间复杂度 O(log(N))

```c++
int res = wm.range_kth_max(l, r, k);
```

6. 查询区间 [l,r)中，在值域在[a, b) 中的数字个数

```c++
int cnt = wm.range_freq(l, r, a, b);
```

7. 查询区间[l,r)中 的最小值/最大值

```c++
int res = wm.range_min(l, r);
int res = wm.range_min(l, r);
```

8.  [l, r)中 满足lower < v 的最小的v

```c++
auto res = wm.range_min_gt(l, r, lower);
```

9. [l, r)中 满足lower <= v 的最小的v

```c++
auto res = wm.range_min_geq(l, r, lower);
```

10.  [l, r)中 满足 v < upper 的最大的v 

```c++
auto res = wm.range_max_lt(l, r, lower);
```

11. [l, r)中 满足 v <= upper 的最大的v 

```c++
auto res = wm.range_max_leq(l, r, lower);
```


### 模板check

[Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)

```c++
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &e : a) std::cin >> e;
    CompressedWaveletMatrix<int, 18> wm(a);
    while (q --> 0) {
        int l, r, k;
        std::cin >> l >> r >> k;
        std::cout << wm.range_kth_min(l, r, k) << '\n';
    }
    return 0;
}
```