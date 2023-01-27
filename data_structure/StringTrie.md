# 字符串字典树


该字典树模板可以用来求解：

+ 给定字符串s，统计trie中多少个字符串等于s
+ 给定字符串s，统计trie中多少个字符串是s的前缀字符串
+ 给定字符串s，统计s是trie中多少个字符串的前缀
+ 给定字符串s，对s的每个前缀串t， 统计t是trie中多少个字符串的前缀，并对所有t求和


### 模板代码


```c++
// 最小字母CH是'a'，字母集大小K是26
template<char CH = 'a', int K = 26>
struct trie {
    struct node {
        array<int, K> child;
        int cnt[2]{0, 0};
        node () { memset(&child[0], -1, K * sizeof(int));}
    };

    vector<node> tr = {node()};

    trie(int tot_len = -1) {
        if (tot_len >= 0) tr.reserve(tot_len + 1);
    }

    int add(const string &s) {
        int p = 0; 
        for (char ch: s) {
            int u = ch - CH;
            if (tr[p].child[u] < 0) {
                tr[p].child[u] = int(tr.size());
                tr.emplace_back();
            }
            p = tr[p].child[u];
            tr[p].cnt[0]++;
        }
        tr[p].cnt[1]++;
        return p;
    }

    // prefix_of_s=1: trie中多少个字符串等于 s (如果count_prefix=1，求多少个字符串是s的前缀)
    // prefix_of_s=0: s是trie中多少个字符串的前缀 (如果count_prefix=1，对s的每个前缀也进行累加)
    int get(const string &s, bool prefix_of_s = 0, bool count_prefix = 0) {
        int p = 0, ans = 0;
        for (char ch: s) {
            if (count_prefix) ans += tr[p].cnt[prefix_of_s];
            p = tr[p].child[ch - CH];
            if (p < 0) break;
        }
        if (p >= 0) ans += tr[p].cnt[prefix_of_s];
        return ans;
    }
};
```

### 使用说明

1. 初始化

如果 s 中全为小写字母，可以定义为

```c++
trie t;
```
或者

```c++
trie<'a', 26> t;
```

2. 字符下标编号为0-(K-1)，必要时对字符进行离散化

例如，如果既有小写字母又有大写字母，又有数字，共62种字符，可以建立一个字符映射，对每个字符映射为新的字符，再求解。

```c++
trie<0, 62> t;
auto get=[&](char c) {
    if (c >='a' && c <= 'z'){
        return c - 'a'; // 0 - 25
    }else if(c >= 'A' && c <= 'Z'){
        return c - 'A' + 26; // 26 - 51
    }
    return c - '0' + 52;  // 52 - 61
};
```

3. 给字典树添加字符串

```c++
string s;
t.add(s);
```

4. 查询一个字符串在trie中出现了多少次。

```c++
string s;
int ans = t.get(s, 1, 0);
```

5. 查询trie中有多少个字符串是s的前缀

```c++
string s;
int ans = t.get(s, 1, 1);
```

6. 查询s是trie中多少个字符串的前缀

```c++
string s;
int ans = t.get(s, 0, 0);
```

7. 对s的每个前缀字符串s1, 查询s1是trie中多少个字符串的前缀, 并对所有字符串s1的结果进行求和

```c++
string s;
t.get(s, 0, 1);
```
