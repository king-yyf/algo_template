# 区间维护

支持动态添加区间、删除区间、维护区间总长度。 sublime 本地补全缩写为 `range`

模板借鉴自oldyan， 继承了 std::map ，用键值对保存一个区间的左端点和右端点。


### 模板代码


```c++
template <typename T>
struct Range: map<T, T> {
    T siz = 0;
    using map<T, T>::map, map<T, T>::upper_bound, map<T, T>::begin, map<T, T>::end, map<T, T>::emplace, map<T, T>::erase;
    static T len(const pair<T, T> &p) {return p.second - p.first + 1;}
    pair<vector<pair<T, T>>, typename map<T, T>::iterator> add(pair<T, T> p) {
        vector<pair<T, T>> ds;
        auto it = upper_bound(p.first);
        if (it != begin() && prev(it)->second + 1 >= p.first) {
            if (prev(it)->second >= p.second) return {{*prev(it)}, prev(it)};
            p.first = prev(it)->first;
            ds.emplace_back(*prev(it));
            it = erase(prev(it));
        }
        for (; it != end() && it->first <= p.second + 1; it = erase(it)) {
            if (it->second > p.second) p.second = it->second;
            ds.emplace_back(*it);
        }
        for (auto &range : ds) siz -= len(range);
        auto inserted = emplace(p.first, p.second).first;
        siz += len(*inserted);
        return {ds, inserted};
    }
    pair<vector<pair<T, T>>, vector<pair<T, T>>> del(pair<T, T> p) {
        vector<pair<T, T>> ds, as;
        auto it = upper_bound(p.first);
        if (it != begin() && prev(it)->second + 1 >= p.first) {
            it = prev(it);
            ds.emplace_back(*it);
            if (it->second >= p.second) {
                if (it->second > p.second) as.emplace_back(*emplace(p.second + 1, it->second).first);
                if (it->first < p.first) {
                    it->second = p.first - 1;
                    as.emplace_back(*it);
                } else
                    erase(it);
                for (auto &range : ds) siz -= len(range);
                for (auto &range : as) siz += len(range);
                return {ds, as};
            }
            if (it->first < p.first) {
                it->second = p.first - 1;
                as.emplace_back(*it);
                it = next(it);
            } else
                it = erase(it);
        }
        for (; it != end() && it->first <= p.second; it = erase(it)) {
            ds.emplace_back(*it);
            if (it->second > p.second) as.emplace_back(*emplace(p.second + 1, it->second).first);
        }
        for (auto &range : ds) siz -= len(range);
        for (auto &range : as) siz += len(range);
        return {ds, as};
    }
    typename map<T, T>::const_iterator any(const pair<T, T> &p) const {
        if (auto it = upper_bound(p.second); it != begin() && prev(it)->second >= p.first) return prev(it);
        return end();
    }
    typename map<T, T>::const_iterator all(const pair<T, T> &p) const {
        if (auto it = upper_bound(p.second); it != begin() && prev(it)->first <= p.first && prev(it)->second >= p.second) return prev(it);
        return end();
    }
    template <typename OS>
    friend OS &operator<<(OS &os, const Range &t) {
        os << '[';
        uint32_t index = 0;
        for (auto &[l, r] : t) {
            if (index++) os << ", ";
            os << '[' << l << ", " << r << ']';
        }
        return os << ']';
    }
};
```

1. 创建一个 Range, 

+ 时间复杂度 O(1)

```c++
Range<int> rgs;
```

2. 插入区间, 

+ 均摊时间复杂度 O(log(n))

输入参数为 `pair<T,T>` ,表示插入区间的左右端点.
返回类型为: `pair<vector<pair<T, T>>, typename map<T, T>::iterator>`, 前者表示所有被新区间影响、进而删除的旧区间；后者将所有旧区间合并在一块，生成的新区间。

```c++
rgs.add({1, 10});
```

3. 移除区间 

+ 均摊时间复杂度 O(log(n))

输入参数为 `pair<T,T>` ,表示要移除区间的左右端点.
返回类型 `pair<vector<pair<T, T>>, vector<pair<T, T>>>` , 前者表示所有与要移除的区间有牵扯、进而被删除的旧区间；后者为旧区间把要移除部分移除后，剩余的区间部分。

```c++
auto [dels, inss] = rgs.del({13, 25});
```

4. 查询区间中是否有数出现过 

+ 时间复杂度 O(log(n))

只要要查询的区间中，有任何一个数字已经存在，则返回 true 。

输入参数为 `pair<T,T>` ,表示查询的区间.
返回与查询区间有交集的区间迭代器，如果都没有交集，返回 end()

```c++
auto it = rgs.any({10, 15});

if (it != rgs.end()) {
    cout << it->first << ", " << it->second << "\n";
}
```

5. 查询区间是否全部存在

+ 时间复杂度 O(log(n))

仅当要查询的区间中，每个数字都已经存在，才会返回 true 。

输入参数为 `pair<T,T>` ,表示查询的区间.
返回包含查询区间的区间迭代器，如果没有包含查询区间的区间，返回 end()

```c++
auto it = rgs.all({3, 8});

if (it != rgs.end()) {
    cout << it->first << ", " << it->second << "\n";
} 
```

6. 区间总长度

+ 时间复杂度 O(1)

```c++
cout << "length of ranges = " << rgs.siz << endl;
```