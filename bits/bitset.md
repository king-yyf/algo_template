# bitset

===

Index
---
<!-- TOC -->

- [动态bitset](#动态bitset)
- [静态bitset](#静态bitset)

   
<!-- /TOC -->

### 动态bitset

```c++
struct DynamicBitSet {
private:
    using block = unsigned long long;
    static constexpr size_t blk_siz = numeric_limits<block>::digits;
    static constexpr size_t log_blk_siz = __builtin_ctz(blk_siz);

    struct bitref {
        block& b;
        size_t i;
        operator bool() const { return (b >> i) & 1; }
        bool test() const { return (b >> i) & 1; }
        void set() { b |= block(1) << i; }
        void reset() { b &= ~(block(1) << i); }
        void flip() { b ^= block(1) << i; }
        bitref& operator&=(bool val) { b &= block(val) << i; return *this; }
        bitref& operator|=(bool val) { b |= block(val) << i; return *this; }
        bitref& operator^=(bool val) { b ^= block(val) << i; return *this; }
        bitref& operator =(bool val) { val ? set() : reset(); return *this; }
        bitref& operator =(const bitref& v) { return (*this) = bool(v); }
    };

    size_t n;
    vector<block> blocks;
public:
    DynamicBitSet(size_t n = 0, bool fill_v = false) : n(n), blocks((n + blk_siz - 1) >> log_blk_siz, fill_v ? ~block(0) : 0) {}

    bool empty() const { return n == 0; }
    int size() const { return n; }

    void resize(size_t new_size, bool fill_v = false) {
        size_t new_block_num = (new_size + blk_siz - 1) >> log_blk_siz;
        if (new_block_num < block_num()) {
            n = new_size;
            return blocks.resize(new_block_num);
        }
        blocks.resize(new_block_num);
        size_t old_size = exchange(n, new_size);
        if (old_size <= new_size) range_update(old_size, new_size, fill_v);
    }
    void push_back(bool val) {
        if (n & (blk_siz - 1)) (*this)[n] = val;
        else blocks.push_back(val);
        ++n;
    }
    void pop_back() { if ((n & (blk_siz - 1)) == 1) blocks.pop_back(); --n;}

    friend bool operator==(const DynamicBitSet& x, const DynamicBitSet& y) {
        if (x.n != y.n) return false;
        if (x.empty()) return true;
        for (size_t i = 0; i < x.block_num() - 1; ++i) if (x.blocks[i] != y.blocks[i]) return false;
        const size_t num = x.n - ((x.block_num() - 1) << log_blk_siz);
        return get_lower_bits(x.blocks.back(), num) == get_lower_bits(y.blocks.back(), num);
    }
    friend bool operator!=(const DynamicBitSet& x, const DynamicBitSet& y) { return not (x == y); }
    friend bool operator<(const DynamicBitSet& x, const DynamicBitSet& y) {
        assert(x.n == y.n);
        if (x.empty()) return false;
        size_t num = x.n - ((x.block_num() - 1) << log_blk_siz);
        block tx = get_lower_bits(x.blocks.back(), num), ty = get_lower_bits(y.blocks.back(), num);
        if (tx != ty) return tx < ty;
        for (size_t i = x.block_num() - 1; i-- > 0;) if (x.blocks[i] != y.blocks[i]) return x.blocks[i] < y.blocks[i];
        return false;
    }
    friend bool operator<=(const DynamicBitSet& x, const DynamicBitSet& y) {
        assert(x.n == y.n);
        if (x.empty()) return true;
        size_t num = x.n - ((x.block_num() - 1) << log_blk_siz);
        block tx = get_lower_bits(x.blocks.back(), num), ty = get_lower_bits(y.blocks.back(), num);
        if (tx != ty) return tx < ty;
        for (size_t i = x.block_num() - 1; i-- > 0;) if (x.blocks[i] != y.blocks[i]) return x.blocks[i] < y.blocks[i];
        return true;
    }
    friend bool operator>(const DynamicBitSet& x, const DynamicBitSet& y) { return not (x <= y);}
    friend bool operator>=(const DynamicBitSet& x, const DynamicBitSet& y) { return not (x < y);}

    operator bool() const { return any(); }

    friend DynamicBitSet& operator&=(DynamicBitSet& x, const DynamicBitSet& y) {
        assert(x.n == y.n);
        for (size_t i = 0; i < y.block_num(); ++i) x.blocks[i] &= y.blocks[i];
        return x;
    }
    friend DynamicBitSet& operator|=(DynamicBitSet& x, const DynamicBitSet& y) {
        assert(x.n == y.n);
        for (size_t i = 0; i < y.block_num(); ++i) x.blocks[i] |= y.blocks[i];
        return x;
    }
    friend DynamicBitSet& operator^=(DynamicBitSet& x, const DynamicBitSet& y) {
        assert(x.n == y.n);
        for (size_t i = 0; i < y.block_num(); ++i) x.blocks[i] ^= y.blocks[i];
        return x;
    }
    friend DynamicBitSet operator&(DynamicBitSet x, const DynamicBitSet& y) { x &= y; return x; }
    friend DynamicBitSet operator|(DynamicBitSet x, const DynamicBitSet& y) { x |= y; return x; }
    friend DynamicBitSet operator^(DynamicBitSet x, const DynamicBitSet& y) { x ^= y; return x; }

    friend DynamicBitSet& operator<<=(DynamicBitSet &x, size_t shamt) { return x = x << shamt; }
    friend DynamicBitSet& operator>>=(DynamicBitSet &x, size_t shamt) { return x = x >> shamt; }
    friend DynamicBitSet operator<<(const DynamicBitSet &x, size_t shamt) {
        if (shamt >= x.n) return DynamicBitSet(x.size());
        DynamicBitSet res(x.size());
        size_t block_shamt = shamt >> log_blk_siz;
        size_t bit_shamt = shamt & (blk_siz - 1);
        for (size_t i = 0; i + block_shamt < res.block_num(); ++i) {
            if (bit_shamt == 0) res.blocks[i + block_shamt] = x.blocks[i];
            else {
                res.blocks[i + block_shamt] |= x.blocks[i] << bit_shamt;
                if (i + block_shamt + 1 != res.block_num()) 
                    res.blocks[i + block_shamt + 1] |= x.blocks[i] >> (blk_siz - bit_shamt);
            }
        }
        return res;
    }
    friend DynamicBitSet operator>>(const DynamicBitSet& x, size_t shamt) {
        if (shamt >= x.n) return DynamicBitSet(x.size());
        DynamicBitSet res(x.size());
        size_t block_shamt = shamt >> log_blk_siz;
        size_t bit_shamt = shamt & (blk_siz - 1);
        for (size_t i = 0; i + block_shamt < x.block_num(); ++i) {
            if (bit_shamt == 0) res.blocks[i] = x.blocks[i + block_shamt];
            else {
                res.blocks[i] |= x.blocks[i + block_shamt] >> bit_shamt;
                if (i + block_shamt + 1 != x.block_num())
                    res.blocks[i] |= x.blocks[i + block_shamt + 1] << (blk_siz - bit_shamt);
            }
        }
        res.range_reset(x.n - shamt, x.n);
        return res;
    }

    DynamicBitSet operator~() const {
        DynamicBitSet neg(n);
        for (size_t i = 0; i < block_num(); ++i) neg.blocks[i] = ~blocks[i];
        return neg;
    }

    bool operator[](size_t i) const { return (blocks[block_index(i)] >> bit_index(i)) & 1;}
    bitref operator[](size_t i) { return { blocks[block_index(i)], bit_index(i) };}

    void range_set(size_t l, size_t r) {
        assert(l <= r and r <= n);
        if (l == r) return;
        size_t lb = block_index(l), rb = block_index(r - 1);
        size_t li = bit_index(l), ri = bit_index(r);
        if (ri == 0) ri = blk_siz;
        if (lb == rb) {
            blocks[lb] |= mask_range_bits(~block(0), li, ri);
            return;
        }
        blocks[lb] |= mask_upper_bits(~block(0), blk_siz - li);
        blocks[rb] |= mask_lower_bits(~block(0), ri);
        for (size_t i = lb + 1; i < rb; ++i) blocks[i] = ~block(0);
    }
    void range_reset(size_t l, size_t r) {
        assert(l <= r and r <= n);
        if (l == r) return;
        size_t lb = block_index(l), rb = block_index(r - 1);
        size_t li = bit_index(l), ri = bit_index(r);
        if (ri == 0) ri = blk_siz;
        if (lb == rb) {
            blocks[lb] &= ~mask_range_bits(~block(0), li, ri);
            return;
        }
        blocks[lb] &= ~mask_upper_bits(~block(0), blk_siz - li);
        blocks[rb] &= ~mask_lower_bits(~block(0), ri);
        for (size_t i = lb + 1; i < rb; ++i) blocks[i] = block(0);
    }
    void range_flip(size_t l, size_t r) {
        assert(l <= r and r <= n);
        if (l == r) return;
        size_t lb = block_index(l), rb = block_index(r - 1);
        size_t li = bit_index(l), ri = bit_index(r);
        if (ri == 0) ri = blk_siz;
        if (lb == rb) {
            blocks[lb] ^= mask_range_bits(~block(0), li, ri);
            return;
        }
        blocks[lb] ^= mask_upper_bits(~block(0), blk_siz - li);
        blocks[rb] ^= mask_lower_bits(~block(0), ri);
        for (size_t i = lb + 1; i < rb; ++i) blocks[i] ^= ~block(0);
    }
    void range_update(size_t l, size_t r, bool val) { val ? range_set(l, r) : range_reset(l, r);}
    int range_count(size_t l, size_t r) const {
        assert(l <= r and r <= n);
        if (l == r) return 0;
        size_t lb = block_index(l), rb = block_index(r - 1), li = bit_index(l), ri = bit_index(r);
        if (ri == 0) ri = blk_siz;
        
        if (lb == rb) return __builtin_popcountll(blocks[lb] & mask_range_bits(~block(0), li, ri));
        int res = 0;
        res += __builtin_popcountll(blocks[lb] & mask_upper_bits(~block(0), blk_siz - li));
        res += __builtin_popcountll(blocks[rb] & mask_lower_bits(~block(0), ri));
        for (size_t i = lb + 1; i < rb; ++i) res += __builtin_popcountll(blocks[i]);
        return res;
    }

    void set() { for (block& b : blocks) b = ~block(0); }
    void reset() { for (block& b : blocks) b = 0; }
    bool all() const {
        if (empty()) return true;
        for (size_t i = 0; i < block_num() - 1; ++i) if (blocks[i] != ~block(0)) return false;
        const size_t num = n - ((block_num() - 1) << log_blk_siz);
        assert(num);
        const block upper = ((block(1) << (blk_siz - num)) - 1) << num;
        return (upper | blocks.back()) == ~block(0);
    }
    bool none() const {
        if (empty()) return true;
        for (size_t i = 0; i < block_num() - 1; ++i) if (blocks[i] != 0) return false;
        const size_t num = n - ((block_num() - 1) << log_blk_siz);
        return get_lower_bits(blocks.back(), num) == 0;
    }
    bool any() const { return not none();}
    int count() const {
        if (empty()) return 0;
        int res = 0;
        for (size_t i = 0; i < block_num() - 1; ++i) res += __builtin_popcountll(blocks[i]);
        const size_t num = n - ((block_num() - 1) << log_blk_siz);
        return res + __builtin_popcountll(get_lower_bits(blocks.back(), num));
    }

    // Returns the position of first set bit. If there is no such positions, then returns size().
    int find_first() const {
        if (empty()) return size();
        for (size_t i = 0; i < block_num(); ++i) 
            if (blocks[i] != 0) return min(n, __builtin_ctzll(blocks[i]) | (i << log_blk_siz));
        return n;
    }
    // Returns the position of first set bit after the given position (exclusive). If there is no such positions, then returns size().
    int find_next(size_t pos) const {
        size_t i = block_index(++pos);
        if (i >= blocks.size()) return n;
        block upper = mask_upper_bits(blocks[i], blk_siz - bit_index(pos));
        if (upper != 0) return min(n, __builtin_ctzll(upper) | (i << log_blk_siz));
        while (++i < block_num()) {
            if (blocks[i] != 0) return min(n, __builtin_ctzll(blocks[i]) | (i << log_blk_siz));
        }
        return n;
    }

    bool has_intersection(const DynamicBitSet& y) const {
        if (n > y.n) return y.has_intersection(*this);
        if (empty()) return false;
        for (size_t i = 0; i < block_num() - 1; ++i) if (blocks[i] & y.blocks[i]) return true;
        const size_t num = n - ((block_num() - 1) << log_blk_siz);
        return get_lower_bits(blocks.back(), num) & y.blocks[block_num() - 1];
    }
    bool is_disjoint(const DynamicBitSet& y) const { return not has_intersection(y); }

private:
    static constexpr size_t block_index(size_t i) { return i >> log_blk_siz;}
    static constexpr size_t bit_index(size_t i) { return i & (blk_siz - 1);}
    static constexpr block get_lower_bits(block b, size_t num) { return num ? (b << (blk_siz - num) >> (blk_siz - num)) : block(0);}
    static constexpr block get_upper_bits(block b, size_t num) { return num ? (b >> (blk_siz - num)) : block(0);}
    static constexpr block get_range_bits(block b, size_t l, size_t r) { return l < r ? b << (blk_siz - r) >> (blk_siz - r + l) : block(0);}
    static constexpr block mask_lower_bits(block b, size_t num) { return get_lower_bits(b, num);}
    static constexpr block mask_upper_bits(block b, size_t num) { return num ? (b >> (blk_siz - num) << (blk_siz - num)) : block(0);}
    static constexpr block mask_range_bits(block b, size_t l, size_t r) { return l < r ? b << (blk_siz - r) >> (blk_siz - r + l) << l : block(0);}
    size_t block_num() const {return blocks.size();}
};
using bset = DynamicBitSet;
```

### 使用说明

1. 创建一个bitset

```c++
bset a(n);
```

2. 部分操作和 vector 功能类似，例如

1. `push_back()`
2. `resize()`
3. `pop_back()`
4. `size()`
5. `empty()`
6. `a[i]`

3. 一些位运算相关操作和整数操作类似，例如

1. `a += b`
2. `a <= b`
3. `a == b`
4. `a |= b`
2. `a &= b`
3. `a ^= b`
4. `a << 2`

4. 区间操作

+ [l, r) 区间置1

```c++
a.range_set(l, r)
```

+ [l, r) 区间置0

```c++
a.range_reset(l, r)
```

+ [l, r) 区间中1的数目

```c++
a.range_count(l, r)
```

+ [l, r) 区间翻转

```c++
a.range_flip(l, r)
```

5. 取反

```c++
a = ~a;
```

6. 找到 x右边第一个为1的下标, 不包括x

```c++
int pos = a.find_next(x);
```

### 静态bitset

```c++
template <size_t N>
struct static_bitset {
private:
    static constexpr size_t L = (N + 63) >> 6;
public:
    static_bitset() {}
    static_bitset(array<uint64_t, L> data) : _data(data) {}

    static_bitset<N>& operator&=(const static_bitset<N>& rhs) {
        for (size_t i = 0; i < L; ++i) _data[i] &= rhs._data[i];
        return *this;
    }
    static_bitset<N>& operator|=(const static_bitset<N>& rhs) {
        for (size_t i = 0; i < L; ++i) _data[i] |= rhs._data[i];
        return *this;
    }
    static_bitset<N>& operator^=(const static_bitset<N>& rhs) {
        for (size_t i = 0; i < L; ++i) _data[i] ^= rhs._data[i];
        return *this;
    }
    static_bitset<N> operator&(const static_bitset<N>& rhs) const {
        return static_bitset<N>(_data) &= rhs;
    }
    static_bitset<N> operator|(const static_bitset<N>& rhs) const {
        return static_bitset<N>(_data) |= rhs;
    }
    static_bitset<N> operator^(const static_bitset<N>& rhs) const {
        return static_bitset<N>(_data) ^= rhs;
    }
    static_bitset<N> operator~() const {
        static_bitset<N> res;
        for (size_t i = 0; i < L; ++i) res._data[i] = ~_data[i];
        return res;
    }
    static_bitset<N>& operator<<=(const size_t shamt) {
        if (shamt >= N) {
            _data.fill(0);
            return *this;
        }
        size_t large_shamt = outer_index(shamt);
        size_t small_shamt = inner_index(shamt);
        if (small_shamt) {
            size_t rshamt = 64 - small_shamt;
            for (size_t i = L - 1; i > large_shamt; --i) {
                _data[i] = (_data[i - large_shamt] << small_shamt) | (_data[i - large_shamt - 1] >> rshamt);
            }
            _data[large_shamt] = _data[0] << small_shamt;
        } else {
            for (size_t i = L; i-- > large_shamt;) {
                _data[i] = _data[i - large_shamt];
            }
        }
        for (size_t i = large_shamt; i-- > 0;) _data[i] = 0;
        return *this;
    }
    static_bitset<N>& operator>>=(const size_t shamt) {
        if (shamt >= N) {
            _data.fill(0);
            return *this;
        }
        if (shamt == 0) {
            return *this;
        }
        size_t large_shamt = outer_index(shamt);
        size_t small_shamt = inner_index(shamt);
        if (small_shamt) {
            size_t lshamt = 64 - small_shamt;
            for (size_t i = 0; i + large_shamt + 1 < L; ++i) {
                _data[i] = (_data[i + large_shamt] >> small_shamt) | (_data[i + large_shamt + 1] << lshamt);
            }
            _data[L - large_shamt - 1] = _data[L - 1] >> small_shamt;
        } else {
            for (size_t i = 0; i + large_shamt < L; ++i) {
                _data[i] = _data[i + large_shamt];
            }
        }
        for (size_t i = L - large_shamt; i < L; ++i) _data[i] = 0;
        return *this;
    }
    static_bitset<N> operator<<(const size_t shamt) const {
        return static_bitset<N>(_data) <<= shamt;
    }
    static_bitset<N> operator>>(const size_t shamt) const {
        return static_bitset<N>(_data) >>= shamt;
    }
    bool operator!=(const static_bitset<N>& rhs) const {
        for (size_t i = L; i-- > 0;) if (_data[i] != rhs._data[i]) return true;
        return false;
    }
    bool operator==(const static_bitset<N>& rhs) const {
        return not (*this != rhs);
    }
    bool operator<(const static_bitset<N>& rhs) const {
        for (size_t i = L; i-- > 0;) if (_data[i] != rhs._data[i]) return _data[i] < rhs._data[i];
        return false;
    }
    bool operator<=(const static_bitset<N>& rhs) const {
        return not (*this > rhs);
    }
    bool operator>(const static_bitset<N>& rhs) const {
        return rhs < *this;
    }
    bool operator>=(const static_bitset<N>& rhs) const {
        return not (*this < rhs);
    }
    bool operator[](size_t i) {
        return (_data[outer_index(i)] >> inner_index(i)) & 1;
    }
    operator bool() const {
        for (const auto& e : _data) if (e) return true;
        return false;
    }
    void range_set(const size_t l, const size_t r, bool val = true) {
        if (l >= r) return;
        const size_t out_l = outer_index(l), out_r = outer_index(r - 1);
        const size_t in_l = inner_index(l), in_r = inner_index(r - 1);
        const uint64_t mask_r = in_r == 63 ? ~0ULL : (1ULL << (in_r + 1)) - 1ULL;
        const uint64_t mask_l = in_l == 0 ? 0ULL : (1ULL << in_l) - 1ULL;
        if (val) {
            if (out_l < out_r) {
                _data[out_l] |= ~0ULL ^ mask_l;
                for (size_t i = out_l + 1; i < out_r; ++i) _data[i] = ~0ULL;
                _data[out_r] |= mask_r;
            } else {
                _data[out_l] |= mask_r ^ mask_l;
            }
        } else {
            if (out_l < out_r) {
                _data[out_l] &= mask_l;
                for (size_t i = out_l + 1; i < out_r; ++i) _data[i] = 0ULL;
                _data[out_r] &= ~mask_r;
            } else {
                _data[out_l] &= ~(mask_r ^ mask_l);
            }
        }
    }
    void set(const size_t n, bool val = true) {
        if (val) {
            _data[outer_index(n)] |= uint64_t(1) << inner_index(n);
        } else {
            _data[outer_index(n)] &= ~(uint64_t(1) << inner_index(n));
        }
    }
    void flip(const size_t n) {
        _data[outer_index(n)] ^= 1ULL << inner_index(n);
    }
    void clear() {
        _data.fill(0);
    }
    const array<uint64_t, L>& data() const {
        return _data;
    }
    friend ostream& operator<<(ostream& out, const static_bitset<N>& bs) {
        for (size_t i = N; i-- > 0;) out << char('0' + bs[i]);
        return out;
    }
private:
    array<uint64_t, L> _data{};

    static constexpr size_t outer_index(size_t k) {
        return k >> 6;
    }
    static constexpr size_t inner_index(size_t k) {
        return k & 63;
    }
};
using bset = static_bitset<10>;
```