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

```

### 后缀数组说明

后缀数组主要关系两个数组： sa和rk， sa[i]表示表示将所有后缀排序后第i小的后缀的编号. rk[i]表示后缀i的排名,这两个数组满足性质: sa[rk[i]] = rk[sa[i]] = i
