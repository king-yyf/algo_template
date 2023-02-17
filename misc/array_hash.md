# 数组哈希

可以将一个数组映射到一个哈希值。


### 模板代码

```c++
uint64_t rand_addr() { char *p = new char; delete p; return uint64_t(p); }
uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}
const uint64_t FIXED_RAND = splitmix64(chrono::steady_clock::now().time_since_epoch().count() * (rand_addr() | 1));
template<typename T>
struct array_hash {
    using hash_t = uint64_t;
    int n;
    vector<T> a;
    hash_t hash;
    array_hash(int n) : n(n), a(n){ init(); }
    array_hash(const vector<T> &A) : n(A.size()){a = A; init(); }

    hash_t get(int x) const { // 0 <= x < n
        return splitmix64(a[x] ^ splitmix64(x ^ FIXED_RAND));
    }
    void init() {
        hash = 0;
        for (int i = 0; i < n; i++)
            hash += get(i);
    }
    const T& operator[](int x) const { return a[x];}
    void set(int x, const T &v) {
        hash -= get(x);
        a[x] = v;
        hash += get(x);
    }
};
```

### 使用说明

1. array_hash

```c++
vector<int> a{2,4,6};
array_hash<int> h1(a);
```

2. 获取下标x处的哈希值

```c++
auto hash = h.get(x);
```

3. 获取下标x处的原始值

```c++
auto val = h[x];
```

4. 重设x下标处的元素

```c++
h.set(x,v);
```