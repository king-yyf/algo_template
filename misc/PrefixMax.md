# 在线前缀最值

用来在线维护一个map，该map支持查询key小于x的value最值。map中的value是随着key的递增而递增的。

例如 如果map中存在一个 `(4,10)`, 向里面插入 `(5,8)` 时，该数据会被忽略，因为对于一切小于5的key, 10 永远是比8更优的解。


### 模板代码

```c++
template<typename K, typename V, bool max_mode, V e>
struct PrefixMax {
    static bool op(V a, V b) { return max_mode ? b < a : a < b;}
    map<K, V> st;

    int size() const { return int(st.size());}
    V get(K x) const { // 查找 key小于 x 的数对中 value 的最大值
        auto it = st.lower_bound(x);
        return it == st.begin() ? e : prev(it)->second;
    }
    void insert(K key, V value) {
        auto it = st.upper_bound(key);
        if (it != st.begin() && !op(value, prev(it)->second)) return;
        if (it != st.begin() && prev(it)->first == key)
            st.erase(prev(it));
        while (it != st.end() && !op(it->second, value))
            it = st.erase(it);
        st.insert(it, {key, value});
    }
};
template<typename T_PrefixMax>
void merge(T_PrefixMax &x, T_PrefixMax &y) {
    if (x.size() < y.size()) swap(x, y);
    for (auto &p : y.st) x.insert(p.first, p.second);
    y.st.clear();
}
```

### 使用说明

1. 创建一个PrefixMax

四个参数分别为 key_type, value_type, max_mode, 单位元e，如果key不存在将返回e，对于任意可能的值x, e满足

+ 如果是最大值模式： max(e, x) = x
+ 如果是最小值模式： min(e, x) = x

```c++
int e = 0;
PrefixMax<int, int, true, e> pre_max;
```

2. 插入一个键值对

```c++
pre_max.insert({k, v});
```

3. 查询key小于x的多有数对中 value的最大值

```c++
auto res = pre_max.get(x);
```