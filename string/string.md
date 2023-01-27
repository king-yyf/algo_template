# 字符串算法


===

Index
---
<!-- TOC -->

- [manacher算法](#manacher)
  - [manacher模板](#manacher模板)
  - [使用方法](#manacher说明)
- [kmp算法](#kmp)
  - [kmp模板](#kmp模板)
  - [使用方法](#kmp说明)
- [z_function](#z函数)
  - [z_function模板](#z函数模板)
  - [使用方法](#z函数说明)
- [后缀数组](#后缀数组)
  - [后缀数组模板](#后缀数组模板)
  - [使用方法](#后缀数组说明)
- [回文树](#回文树)
  - [回文树模板](#回文树模板)
  - [使用方法](#回文树说明)
- [回文树](#回文树)
  - [回文树模板](#回文树模板)
  - [使用方法](#回文树说明)
- [后缀自动机](#后缀自动机)
  - [后缀自动机模板](#后缀自动机模板)
  - [使用方法](#后缀自动机说明)
- [lyndon分解](#lyndon分解)
  - [lyndon分解模板](#lyndon分解模板)
  - [使用方法](#lyndon分解说明)
- [最小表示法](#最小表示法)
  - [最小表示法模板](#最小表示法模板)
  - [使用方法](#最小表示法说明)
  
<!-- /TOC -->


## manacher

### manacher模板

```c++
void manacher(string s){
    int n = s.size();
    vector<int> d1(n), d2(n);
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (0 <= i - k && i + k < n && s[i - k] == s[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) l = i - k, r = i + k;
    }
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (0 <= i - k - 1 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) l = i - k - 1, r = i + k;
    }
}
```

### manacher说明

对于每个位置 `i = 0,1,…,n-1` ，

半径长度 d1[i]，d2[i] 均为**从位置 i 到回文串最右端位置包含的字符个数**

+ d1[i]: 以位置 i 为中心的长度为奇数的回文串个数, 即 i 为中心长度为奇数的最长回文串的半径长度
+ d2[i]: 以位置 i 为中心的长度为偶数的回文串个数, 即 i 为中心长度为偶数的最长回文串的半径长度

manacher算法求出d1,d2后可在O(1)的时间内判断s的任意区间是否是回文串,例如

判断 s[l],s[l+1],...s[r-1]是否是回文串

```c++
auto is_palindrome = [&](int l, int r) {
    if ((r - l) % 2 == 0)
        return d2[(l + r) / 2] >= (r - l) / 2;
    return d1[(l + r) / 2] >= (r - l + 1) / 2;
};
```

## kmp


### kmp模板

```c++
template <typename T>
vector<int> kmp_table(const T &s) {
    int n = s.size(), k = 0;
    vector<int> p(n);
    for (int i = 1; i < n; ++i) {
        while (k > 0 && !(s[i] == s[k])) k = p[k - 1];
        if (s[i] == s[k]) k++;
        p[i] = k;
    }
    return p;
}
template <typename T>
vector<int> kmp(const T &s, const T &t) {
    vector<int> res, p = kmp_table(s);
    int n = s.size(), m = t.size(), k = 0;
    for (int i = 0; i < m; ++i) {
        while (k && (k == n || !(t[i] == s[k]))) k = p[k - 1];
        if (t[i] == s[k]) k++;
        if (k == n) res.push_back(i - n + 1);
    }
    return res;
}
```

### kmp说明

给定两个字符串s和t,求s在t中所有出现位置的下标。

p[i]: 前(i + 1)个字符最长公共前缀和后缀的长度。 // 1 <= i <= n

例如 `s = "abcaba";`

```
  i       s[0:i]   p[i]
i = 0      a         0
i = 1      ab        0
i = 2      abc       0
i = 3      abca      1
i = 4      abcab     2
i = 5      abcaba    1
```

## z函数

### z函数模板

```c++
vector<int> z_function(string& s) {
    int n = s.size();
    vector<int> z(n, n);
    for (int i = 1, l = 0, r = 0; i < n; i += 1) {
        if (i <= r and z[i - l] < r - i + 1) z[i] = z[i - l];
        else for (z[i] = max(0, r - i + 1); i + z[i] < n && s[z[i]] == s[i + z[i]]; z[i] += 1);
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z;
}
```

### z函数说明

对于长度为n的字符串s，定义函数z[i]表示s和s[i,n-1]（即以s[i]开头的后缀）的最长公共前缀的长度，z被称为s的z函数。


## 后缀数组

### 后缀数组模板

```c++
namespace suffix_array_yyf {
    std::vector<int> sa_naive(const std::vector<int>& s) {
        int n = int(s.size());
        std::vector<int> sa(n);
        std::iota(sa.begin(), sa.end(), 0);
        std::sort(sa.begin(), sa.end(), [&](int l, int r) {
            if (l == r) return false;
            while (l < n && r < n) {
                if (s[l] != s[r]) return s[l] < s[r];
                l++, r++;
            }
            return l == n;
        });
        return sa;
    }
    std::vector<int> sa_doubling(const std::vector<int>& s) {
        int n = int(s.size());
        std::vector<int> sa(n), rnk = s, tmp(n);
        std::iota(sa.begin(), sa.end(), 0);
        for (int k = 1; k < n; k *= 2) {
            auto cmp = [&](int x, int y) {
                if (rnk[x] != rnk[y]) return rnk[x] < rnk[y];
                int rx = x + k < n ? rnk[x + k] : -1;
                int ry = y + k < n ? rnk[y + k] : -1;
                return rx < ry;
            };
            std::sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            }
            std::swap(tmp, rnk);
        }
        return sa;
    }

    template <int THRESHOLD_NAIVE = 10, int THRESHOLD_DOUBLING = 40>
    std::vector<int> sa_is(const std::vector<int>& s, int upper) {
        int n = int(s.size());
        if (n == 0) return {};
        if (n == 1) return {0};
        if (n == 2) {
            if (s[0] < s[1]) return {0, 1};
            else return {1, 0};
        }
        if (n < THRESHOLD_NAIVE) return sa_naive(s);
        if (n < THRESHOLD_DOUBLING) return sa_doubling(s);
        std::vector<int> sa(n);
        std::vector<bool> ls(n);
        for (int i = n - 2; i >= 0; i--) {
            ls[i] = (s[i] == s[i + 1]) ? ls[i + 1] : (s[i] < s[i + 1]);
        }
        std::vector<int> sum_l(upper + 1), sum_s(upper + 1);
        for (int i = 0; i < n; i++) {
            if (!ls[i]) sum_s[s[i]]++;
            else sum_l[s[i] + 1]++;
        }
        for (int i = 0; i <= upper; i++) {
            sum_s[i] += sum_l[i];
            if (i < upper) sum_l[i + 1] += sum_s[i];
        }

        auto induce = [&](const std::vector<int>& lms) {
            std::fill(sa.begin(), sa.end(), -1);
            std::vector<int> buf(upper + 1);
            std::copy(sum_s.begin(), sum_s.end(), buf.begin());
            for (auto d : lms) {
                if (d == n) continue;
                sa[buf[s[d]]++] = d;
            }
            std::copy(sum_l.begin(), sum_l.end(), buf.begin());
            sa[buf[s[n - 1]]++] = n - 1;
            for (int i = 0; i < n; i++) {
                int v = sa[i];
                if (v >= 1 && !ls[v - 1]) {
                    sa[buf[s[v - 1]]++] = v - 1;
                }
            }
            std::copy(sum_l.begin(), sum_l.end(), buf.begin());
            for (int i = n - 1; i >= 0; i--) {
                int v = sa[i];
                if (v >= 1 && ls[v - 1]) {
                    sa[--buf[s[v - 1] + 1]] = v - 1;
                }
            }
        };

        std::vector<int> lms_map(n + 1, -1);
        int m = 0;
        for (int i = 1; i < n; i++) {
            if (!ls[i - 1] && ls[i]) {
                lms_map[i] = m++;
            }
        }
        std::vector<int> lms;
        lms.reserve(m);
        for (int i = 1; i < n; i++) {
            if (!ls[i - 1] && ls[i]) {
                lms.push_back(i);
            }
        }

        induce(lms);

        if (m) {
            std::vector<int> sorted_lms;
            sorted_lms.reserve(m);
            for (int v : sa) {
                if (lms_map[v] != -1) sorted_lms.push_back(v);
            }
            std::vector<int> rec_s(m);
            int rec_upper = 0;
            rec_s[lms_map[sorted_lms[0]]] = 0;
            for (int i = 1; i < m; i++) {
                int l = sorted_lms[i - 1], r = sorted_lms[i];
                int end_l = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n;
                int end_r = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
                bool same = true;
                if (end_l - l != end_r - r) {
                    same = false;
                } else {
                    while (l < end_l) {
                        if (s[l] != s[r]) {
                            break;
                        }
                        l++, r++;
                    }
                    if (l == n || s[l] != s[r]) same = false;
                }
                if (!same) rec_upper++;
                rec_s[lms_map[sorted_lms[i]]] = rec_upper;
            }

            auto rec_sa =
                sa_is<THRESHOLD_NAIVE, THRESHOLD_DOUBLING>(rec_s, rec_upper);

            for (int i = 0; i < m; i++) {
                sorted_lms[i] = lms[rec_sa[i]];
            }
            induce(sorted_lms);
        }
        return sa;
    }
} // namespace  suffix_array_yyf

std::vector<int> suffix_array(const std::vector<int>& s, int upper) {
    assert(0 <= upper);
    for (int d : s) {
        assert(0 <= d && d <= upper);
    }
    auto sa = suffix_array_yyf::sa_is(s, upper);
    return sa;
}

template <class T> std::vector<int> suffix_array(const std::vector<T>& s) {
    int n = int(s.size());
    std::vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int l, int r) { return s[l] < s[r]; });
    std::vector<int> s2(n);
    int now = 0;
    for (int i = 0; i < n; i++) {
        if (i && s[idx[i - 1]] != s[idx[i]]) now++;
        s2[idx[i]] = now;
    }
    return suffix_array_yyf::sa_is(s2, now);
}

std::vector<int> suffix_array(const std::string& s) {
    int n = int(s.size());
    std::vector<int> s2(n);
    for (int i = 0; i < n; i++) {
        s2[i] = s[i];
    }
    return suffix_array_yyf::sa_is(s2, 255);
}

template <class T>
std::vector<int> lcp_array(const std::vector<T>& s,
                           const std::vector<int>& sa) {
    int n = int(s.size());
    assert(n >= 1);
    std::vector<int> rnk(n);
    for (int i = 0; i < n; i++) {
        rnk[sa[i]] = i;
    }
    std::vector<int> lcp(n - 1);
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (h > 0) h--;
        if (rnk[i] == 0) continue;
        int j = sa[rnk[i] - 1];
        for (; j + h < n && i + h < n; h++) {
            if (s[j + h] != s[i + h]) break;
        }
        lcp[rnk[i] - 1] = h;
    }
    return lcp;
}

std::vector<int> lcp_array(const std::string& s, const std::vector<int>& sa) {
    int n = int(s.size());
    std::vector<int> s2(n);
    for (int i = 0; i < n; i++) {
        s2[i] = s[i];
    }
    return lcp_array(s2, sa);
}
```

### 后缀数组说明

后缀数组主要关系两个数组： sa和rk， sa[i]表示表示将所有后缀排序后第i小的后缀的编号. rk[i]表示后缀i的排名,这两个数组满足性质: sa[rk[i]] = rk[sa[i]] = i

1. 获取后缀数组, 后缀数组从0编号，如果需要从1编号，输出时加一。

```c++
auto sa = suffix_array(s);
```

2. 根据sa获取 rnk数组

```c++
auto sa = suffix_array(s);
int n = int(s.size());
vector<int> rnk(n);
for (int i = 0; i < n; i++) {
    rnk[sa[i]] = i;
}
```

3. lcp_array() 输入s和sa数组求height数组

其中 `h[i] = lcp(sa[i], sa[i+1]) (i < n - 1), h[n - 1] = 0`


```c++
auto h = lcp_array(s, sa);
```

## 回文树

### 回文树模板

```c++
template<typename T, int ALPHABET_SIZE = 26, char CH = 'a'>
struct palindrome_tree {
    // node that represents a palindromic substring
    struct node_t {
        T len, pos, cnt; // 回文子串长度、首次出现位置、出现次数
        T depth, suff;   // suff: node-index of largest palindromic suffix
        T next[ALPHABET_SIZE]; // "A".next['x'] --> "xAx"
    };

    vector<char> _str;    // string of letter ordinals (e.g. 'a' is 0)
    vector<node_t> _nodes;
    T _suff;             // node-index of the current longest palindromic suffix
    long long _total;    // 回文子串总数，可到n*n级别

    palindrome_tree() {_init();}

    palindrome_tree(string &s) {
        _init();
        add_all(s);
    }

    void _init() {
        _str.clear();  _nodes.resize(3);
        _nodes[1].len = -1, _nodes[1].suff = 1;
        _nodes[2].len = 0, _nodes[2].suff = 1;
        _suff = 2, _total = 0;
    }

    template<typename C>
    void reserve_more(C& c, size_t sz) {
        if (c.size() + sz <= c.capacity()) return;
        c.reserve(std::max(c.size() + sz, c.capacity() + c.capacity() / 2));
    }

    T add_all(string &s) {
        size_t len = s.size();
        reserve_more(_str, len), reserve_more(_nodes, len);
        T c = 0;
        for (auto &ch: s) c += add(ch);
        return c;
    }

    T add(char let) {
        let = let - CH;
        _str.push_back(let);
        T i = _find_suffix(_suff, let);
        _suff = _nodes[i].next[let];
        if (_suff != 0) {
            _nodes[_suff].cnt++, _total += _nodes[_suff].depth;
            return 0;
        }
        T suff2 = _find_suffix2(i, let);
        _suff = (T)_nodes.size();
        _nodes.push_back({});
        _nodes[_suff].len = _nodes[i].len + 2;
        _nodes[_suff].pos = (T)_str.size() - _nodes[_suff].len;
        _nodes[_suff].cnt = 1;
        _nodes[_suff].suff = suff2;
        _nodes[_suff].depth = _nodes[suff2].depth + 1;
        _nodes[i].next[let] = _suff;
        _total += _nodes[_suff].depth;
        return 1;
    }

    T _find_suffix2(T i, char let) {
        if (i == 1) return 2;
        i = _find_suffix(_nodes[i].suff, let);
        return _nodes[i].next[let];
    }

    T _find_suffix(T i, char let) {
        T sz = (T)_str.size();
        while (sz < _nodes[i].len + 2 || _str[sz - _nodes[i].len - 2] != let) {
            i = _nodes[i].suff;
        }
        return i;
    }

    // This should be called only once after all elements are added!
    void propagate() {
        for (T i = (T)_nodes.size() - 1; i >= 3; i--) {
            T suff = _nodes[i].suff;
            _nodes[suff].cnt += _nodes[i].cnt;
        }
    }

    // Returns the number of total palindromic substrings, counting their multiplicities.
    long long total() const { return _total;}

    // Returns the number of distinct palindromic substrings, each counted only once.
    T distinct() const { return (T)_nodes.size() - 3;}

    // Returns the index of the node representing the longest palindromic suffix.
    T longest_suffix() const { return _suff;}

    // Returns the <length, index> of longest Palindrome substrings
    array<T, 2> longest_palindrome() const {
        T longest = 0, index = 0;
        for (int i = 3; i < (T)_nodes.size(); ++i) 
            if (_nodes[i].len > longest) 
                longest = _nodes[i].len, index = _nodes[i].pos;
        return {longest, index};
        
    } 

    // Returns the number of nodes.
    T size() const { return (T)_nodes.size();}

    // Accesses node by its index.
    node_t& operator[] (T index) { return _nodes[index];}
};
```

### 回文树说明

回文自动机（Palindromes_Automaton，PAM），也叫回文树，是高效解决回文问题的算法，能够解决很多Manacher算法解决不了的回文题。可以解决如回文串个数、本质不同回文串个数、前缀0-i内回文串个数、某下标结尾的回文串个数等。

1. 定义一个回文树

```c++
string s;
palindrome_tree<int> pt(s);
```

或者

```c++
palindrome_tree<int, 26, 'A'> pt(s);
```

2. 字符串中有多少个回文子字符串

```c++
auto res = pt.total();
```

3. 字符串的最长回文子串

```c++
auto [max_len, pos] = pt.longest_palindrome();
```

4. 不同的回文子串数目

```c++
auto res = pt.distinct();
```

## 后缀自动机

### 后缀自动机模板

模板基于 jiangly 的 SuffixAutomaton 模板。

```c++
struct SuffixAutomaton {
    static constexpr int ALPHABET_SIZE = 26, N = 1e6;
    struct Node {
        int len;
        int link;
        int next[ALPHABET_SIZE];
        Node() : len(0), link(0), next{} {}
    } t[2 * N];
    int cntNodes;
    SuffixAutomaton() : cntNodes(1), t[0].len (-1) {
        fill(t[0].next, t[0].next + ALPHABET_SIZE, 1);
    }
    int extend(int p, int c) {
        if (t[p].next[c]) {
            int q = t[p].next[c];
            if (t[q].len == t[p].len + 1)
                return q;
            int r = ++cntNodes;
            t[r].len = t[p].len + 1;
            t[r].link = t[q].link;
            copy(t[q].next, t[q].next + ALPHABET_SIZE, t[r].next);
            t[q].link = r;
            while (t[p].next[c] == q) {
                t[p].next[c] = r;
                p = t[p].link;
            }
            return r;
        }
        int cur = ++cntNodes;
        t[cur].len = t[p].len + 1;
        while (!t[p].next[c]) {
            t[p].next[c] = cur;
            p = t[p].link;
        }
        t[cur].link = extend(p, c);
        return cur;
    }
};
```

### 后缀自动机说明

[cf 1708G](https://codeforces.com/contest/1780/submission/190616778)

后缀自动机(suffix automaton, SAM) 是一个能解决许多字符串相关问题的有力的数据结构。

举个例子，以下的字符串问题都可以在线性时间内通过 SAM 解决。

+ 在另一个字符串中搜索一个字符串的所有出现位置。
+ 计算给定的字符串中有多少个不同的子串。

1. 定义一个后缀自动机

```c++
SuffixAutomaton sam;
```

## lyndon分解

Lyndon 串 : 对于字符串 s，如果 s 的字典序严格小于 s 的所有后缀的字典序，我们称 s 是 简单串 或者 Lyndon 串。

例如：`a, ab, aab, abb, ababb, abcd` 都是简单串。



### lyndon分解模板

```c++
vector<string> duval(string const& s) {
  int n = s.size(), i = 0;
  vector<string> factorization;
  while (i < n) {
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      if (s[k] < s[j])
        k = i;
      else
        k++;
      j++;
    }
    while (i <= k) {
      factorization.push_back(s.substr(i, j - k));
      i += j - k;
    }
  }
  return factorization;
}
```

### lyndon分解说明

结论1

当且仅当s的字典序严格小于它的所有非平凡(非空且不同于本身)循环同构串时，s才是简单串。

Lyndon分解： 串s的Lyndon分解记为 `s=w1w2… wk`，其中所有wi为简单串，并且他们的字典序按照非严格单减排序，即

`w1 >= w2 >= ..., >= wk`。这样的分解存在且唯一。


Duval算法 可以在O(n)时间内求出一个串的Lyndon分解。


## 最小表示法

### 最小表示法模板

```c++
string min_rep(string s){
    int k = 0, i = 0, j = 1, n = s.size();
    while (k < n && i < n && j < n) {
        if (s[(i + k) % n] == s[(j + k) % n]) k++;
        else {
            s[(i + k) % n] > s[(j + k) % n] ? i = i + k + 1 : j = j + k + 1;
            if (i == j) i++;
            k = 0;
        }
    }
    i = min(i, j);
    return s.substr(i)+s.substr(0,i);
}
```

或者

```c++
string min_cyclic_string(string s) {
  s += s;
  int n = s.size();
  int i = 0, ans = 0;
  while (i < n / 2) {
    ans = i;
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      if (s[k] < s[j])
        k = i;
      else
        k++;
      j++;
    }
    while (i <= k) i += j - k;
  }
  return s.substr(ans, n / 2);
}
```

### 最小表示法说明


当字符串 s 中可以选定一个位置 i 满足 s[i...n] + s[1...i-1] == T 则称 s 与 T 循环同构.

**最小表示**

字符串 s 的最小表示为与 s 循环同构的所有字符串中字典序最小的字符串

最小表示的O(n)算法