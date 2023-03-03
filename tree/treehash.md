# 树哈希

树哈希将一颗树结构转成哈希值储存起来，以降低复杂度，在判断一些树是否同构等问题场景中广泛使用。

**哈希方法**

我们计算以u为根节点的子树G的哈希值

+ 如果u是单独的叶节点, 定义 p(u) = 1,
+ 否则，设d为树的高度，设u有k个直接相连的子节点，其哈希值分别为p1,p2...pk
+ 则子树G的哈希值 `P(G)=(x+p1)*(x+p2)*...*(x+pk)`,其中x为哈希的base，一般为一个随机的质数。


## 模板

```c++
// 3哈希容易tle，默认双哈希, u128在cf上需要选c++20
namespace YYF {
using i64 = long long;
using u64 = unsigned long long;
using u128 = __uint128_t;
template <int BASE_NUM = 2>
struct Hash : array<u64, BASE_NUM> {
  using array<u64, BASE_NUM>::operator[];
  static constexpr int n = BASE_NUM;
  Hash() : array<u64, BASE_NUM>() {}
  static constexpr u64 md = (1ull << 61) - 1;
  constexpr static Hash set(const i64 &a) {
    Hash res;
    fill(begin(res), end(res), cast(a));
    return res;
  }
  Hash &operator+=(const Hash &r) {
    for (int i = 0; i < n; i++) if (((*this)[i] += r[i]) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator+=(const i64 &r) {
    u64 s = cast(r);
    for (int i = 0; i < n; i++) if (((*this)[i] += s) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator-=(const Hash &r) {
    for (int i = 0; i < n; i++) if (((*this)[i] += md - r[i]) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator-=(const i64 &r) {
    u64 s = cast(r);
    for (int i = 0; i < n; i++) if (((*this)[i] += md - s) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator*=(const Hash &r) {
    for (int i = 0; i < n; i++) (*this)[i] = modmul((*this)[i], r[i]);
    return *this;
  }
  Hash &operator*=(const i64 &r) {
    u64 s = cast(r);
    for (int i = 0; i < n; i++) (*this)[i] = modmul((*this)[i], s);
    return *this;
  }
  Hash operator+(const Hash &r) { return Hash(*this) += r; }
  Hash operator+(const i64 &r) { return Hash(*this) += r; }
  Hash operator-(const Hash &r) { return Hash(*this) -= r; }
  Hash operator-(const i64 &r) { return Hash(*this) -= r; }
  Hash operator*(const Hash &r) { return Hash(*this) *= r; }
  Hash operator*(const i64 &r) { return Hash(*this) *= r; }
  Hash operator-() const {
    Hash res;
    for (int i = 0; i < n; i++) res[i] = (*this)[i] == 0 ? 0 : md - (*this)[i];
    return res;
  }
  friend Hash pfma(const Hash &a, const Hash &b, const Hash &c) {
    Hash res;
    for (int i = 0; i < n; i++) res[i] = modfma(a[i], b[i], c[i]);
    return res;
  }
  friend Hash pfma(const Hash &a, const Hash &b, const i64 &c) {
    Hash res; u64 s = cast(c);
    for (int i = 0; i < n; i++) res[i] = modfma(a[i], b[i], s);
    return res;
  }
  Hash pow(long long e) {
    Hash a{*this}, res{Hash::set(1)};
    for (; e; a *= a, e >>= 1) if (e & 1) res *= a;
    return res;
  }
  static Hash get_basis() {
    static auto rand_t =
        chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
    static mt19937_64 rng(rand_t);
    Hash h;
    for (int i = 0; i < n; i++) {
      while (isPrimitive(h[i] = rng() % (md - 1) + 1) == false);
    }
    return h;
  }

 private:
  static u64 modpow(u64 a, u64 b) {
    u64 r = 1;
    for (a %= md; b; a = modmul(a, a), b >>= 1) r = modmul(r, a);
    return r;
  }
  static bool isPrimitive(u64 x) {
    for (auto &d : vector<u64>{2, 3, 5, 7, 11, 13, 31, 41, 61, 151, 331, 1321})
      if (modpow(x, (md - 1) / d) <= 1) return false;
    return true;
  }
  static inline constexpr u64 cast(const long long &a) {
    return a < 0 ? a + md : a;
  }
  static inline constexpr u64 modmul(const u64 &a, const u64 &b) {
    u128 ret = u128(a) * b;
    ret = (ret & md) + (ret >> 61);
    return ret >= md ? ret - md : ret;
  }
  static inline constexpr u64 modfma(const u64 &a, const u64 &b, const u64 &c) {
    u128 ret = u128(a) * b + c;
    ret = (ret & md) + (ret >> 61);
    return ret >= md ? ret - md : ret;
  }
};
} 
template <typename G>
struct TreeHash {
  using Hash = YYF::Hash<2>;
  const G& g;
  int n, root;
  vector<Hash> hash;
  vector<int> dep;
  vector<bool> syn;
  static vector<Hash>& xs() {
    static vector<Hash> _xs;
    return _xs;
  }
  TreeHash(const G& _g, int rt = 0) : n(g.size()), g(_g), dep(n, 0), hash(n), root(rt){
    while ((int)xs().size() <= n) xs().push_back(Hash::get_basis());
    dfs(root, -1);
  }
  void isomorphic() {
    syn.assign(n, false);
    dfs2(root, -1);
  }
 private:
  int dfs(int u, int p) {
    int d = 0;
    for (auto& v : g[u]) if (v != p) 
      d = max(d, dfs(v, u) + 1);
    Hash x = xs()[d], h = Hash::set(1);
    for (auto& v : g[u]) if (v != p) h = h * (x + hash[v]);
    hash[u] = h;
    return dep[u] = d;
  }
  void dfs2(int u, int p) {
    map<Hash,int> cnt, mp;
    for (auto &v : g[u]) if (v != p) {
      cnt[hash[v]]++;
      if (!mp.count(hash[v])) mp[hash[v]] = v;
      dfs2(v, u);
    }
    int d = -1;
    for (auto & [hsh, v] : cnt) if (v & 1) {
      if (~d) return;
      d = mp[hsh];
    }
    if (d == -1 || syn[d]) syn[u] = true;
  }
};
```

**使用方法**

1. 定义树哈希，并初始化每个节点的哈希值。

```c++
vector<vector<int>> g(n);
TreeHash hs(g);
```

2. 获取每个节点的哈希值, hash[i]的长度等于选择哈希模数的数量

```c++
for (int i = 0; i < n; ++i) {
    cout << hs.hash[i][0] << ", " << hs.hash[i][1] << "\n";
}
```

3. 判断树中的以每个节点为根的子树是否是对称树, 整颗树对称只需判断 syn[root]即可。

```c++
hs.isomorphic();
for (int i = 0; i < n; ++i) {
    cout << hs.syn[i] << "\n";
}
```