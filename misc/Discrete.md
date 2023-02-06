# 数组离散化

数组离散化代码, sumlime 本地代码补全缩写为 `dst`


### 模板代码

```c++
template <class T>
struct Discrete {
    vector<T> xs;
    Discrete(const vector<T>& v) {
        xs = v;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
    }
    int get(const T& x) const {
        return lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    }
    inline int operator()(const T& x) const { return get(x); }
    T operator[](int i) { return xs[i]; }
    int size() const { return xs.size(); }
};
```

### 使用说明

1. 离散化一个数组

```c++
vector<int> a {2,6,4,6,8};
Discrete<int> v(a);
```

2. 查找某个元素在离散化后对应的下标

```c++
int pos = v.get(a[i]);
```

或者

```c++
int pos = v(a[i]);
```

3. 获取离散化后的元素个数

```c++
int m = v.size();
```