# 01字典树与可持久化


## 01字典树

### 模板代码

后续尝试封装为统一的类。

```c++
// N是元素个数，K是每个元素最大的长度，int不超过32位。
const int N = 1e5 + 10, K = 30, M = K * N;
int tr[M][2], idx, cnt[M]; 
void add(int x) {
    int p = 0;
    for (int i = K - 1; ~i; --i) {
        int b = (x >> i) & 1;
        if (!tr[p][b]) tr[p][b] = ++idx;
        cnt[p = tr[p][b]]++;
    }
}
void del(int x) {
    int p = 0;
    for (int i = K - 1; ~i; --i) 
        cnt[p = tr[p][(x >> i) & 1]]--;
}
int query(int x) {
    int p = 0, ans = 0;
    for (int i = K - 1; ~i; --i) {
        int b = (x >> i) & 1;
        ans = ans << 1;
        if (cnt[tr[p][b]]) ans++, p = tr[p][b];
        else p = tr[p][!b];
    }
    return ans;
}
// 查询与x异或的结果小于 t 的数的个数
int query(int x, int t) {
	int p = 0, res = 0;
	for (int i = K - 1; ~i; --i) {
		int a = x >> i & 1, b = t >> i & 1;
		if (b == 0) p = tr[p][a];
		else res += cnt[tr[p][a]], p = tr[p][!a];
		if (!p) return res;
	}
	return res;
}
```

### 使用说明

1. 多个样例时需要对 trie 树进行初始化

```c++
idx = 0;
memset(tr, 0, sizeof tr);
memset(cnt, 0, sizeof cnt);
```

2. 插入元素 x

```c++
add(x);
```

3. 删除元素 x

```c++
del(x);
```

4. 查询 Trie 树中所有元素与 x 异或得到的最大异或结果

```c++
int ans = query(~x);
```

5. 查询与x异或的结果小于 t 的数的个数

```c++
int ans = query(x, t);
```

## 可持久化01字典树

### 模板代码

```c++
const int N = 6e5 + 4, K = 24, M = K * N;
int tr[M][2], rt[N], sum[M], idx, tot;

void add(int x) {
    int p = rt[idx];
    rt[++idx] = tot + 1;
    for (int i = K - 1; ~i; --i) {
        sum[++tot] = sum[p] + 1;
        bool b = x & (1 << i);
        tr[tot][b] = tot + 1, tr[tot][!b] = tr[p][!b];
        p = tr[p][b];
    }
}

int query(int l, int r, int x) {
    if (l > r) return 0;
    l = rt[l], r = rt[r];
    int ans = 0;
    for (int i = K - 1; ~i; --i) {
        bool b = x & (1 << i);
        if (sum[tr[r][!b]] - sum[tr[l][!b]]) 
            ans += (1 << i), l = tr[l][!b], r = tr[r][!b];
        else 
            l = tr[l][b], r = tr[r][b];
    }
    return ans;
}
```

### 使用说明

