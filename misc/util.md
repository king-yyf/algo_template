# util



### pair 操作

```c++
template <typename T, typename U>
pair<T, U>& operator+=(pair<T, U> &p1, const pair<T, U> &p2) { p1.first += p2.first, p1.second += p2.second; return p1;}
template <typename T, typename U>
pair<T, U> operator+(const pair<T, U> &p1, const pair<T, U> &p2) { return {p1.first + p2.first, p1.second + p2.second};}
template <typename T, typename U>
pair<T, U>& operator-=(pair<T, U> &p1, const pair<T, U> &p2) { p1.first -= p2.first, p1.second -= p2.second; return p1;}
template <typename T, typename U>
pair<T, U> operator-(const pair<T, U> &p1, const pair<T, U> &p2) { return {p1.first - p2.first, p1.second - p2.second};}
template <typename T, typename U, typename V>
pair<T, U>& operator*=(pair<T, U> &p, const V m) { p.first *= m, p.second *= m; return p;}
template <typename T, typename U, typename V>
pair<T, U>& operator+=(pair<T, U> &p, const V m) { p.first += m, p.second += m; return p;}
template <typename T, typename U, typename V>
pair<T, U>& operator-=(pair<T, U> &p, const V m) { p.first -= m, p.second -= m; return p;}
```

**使用方法**

支持 `pair<T1, T2>` 的 `+ -` 操作， 以及 pair 与 数字的 加减乘。示例：

```c++
int main() {
    
    pair<int, long long> p1 = {10, 2};
    pair<int, long long> p2 = {1, 2};
    p1 += p2; // {11, 4}
    p1 += 5;  // {16, 9}

    cout << "p1 : " << p1.first << ", " << p1.second << "\n";
    return 0;
}
```

### tuple 操作

```c++
namespace tuple_ops {
    template <std::size_t N, typename F, typename ...Args>
    std::tuple<Args...>& update(std::tuple<Args...>& lhs, F&& fun) {
        if constexpr (N == std::tuple_size_v<std::tuple<Args...>>) return lhs;
        else return fun(std::get<N>(lhs)), update<N + 1>(lhs, std::forward<F>(fun));
    }
    template <std::size_t N, typename F, typename ...Args>
    std::tuple<Args...>& merge(std::tuple<Args...>& lhs, const std::tuple<Args...>& rhs, F&& fun) {
        if constexpr (N == std::tuple_size_v<std::tuple<Args...>>) return lhs;
        else return fun(std::get<N>(lhs), std::get<N>(rhs)), merge<N + 1>(lhs, rhs, std::forward<F>(fun));
    }
}
template <typename ...Args>
std::tuple<Args...>& operator+=(std::tuple<Args...>& t1, const std::tuple<Args...>& t2) {
    return tuple_ops::merge<0>(t1, t2, [](auto& x, const auto& y) { x += y; });
}
template <typename ...Args>
std::tuple<Args...>& operator-=(std::tuple<Args...>& t1, const std::tuple<Args...>& t2) {
    return tuple_ops::merge<0>(t1, t2, [](auto& x, const auto& y) { x -= y; });
}
template <typename ...Args>
std::tuple<Args...> operator+(std::tuple<Args...> t1, const std::tuple<Args...>& t2) { return std::move(t1 += t2); }
template <typename ...Args>
std::tuple<Args...> operator-(std::tuple<Args...> t1, const std::tuple<Args...>& t2) { return std::move(t1 -= t2); }

template <typename V, typename ...Args>
std::tuple<Args...>& operator+=(std::tuple<Args...>& t1, const V& v) { return tuple_ops::update<0>(t1, [&v](auto& x) { x += v; }); }
template <typename V, typename ...Args>
std::tuple<Args...>& operator-=(std::tuple<Args...>& t1, const V& v) { return tuple_ops::update<0>(t1, [&v](auto& x) { x -= v; }); }
template <typename V, typename ...Args>
std::tuple<Args...>& operator*=(std::tuple<Args...>& t1, const V& v) { return tuple_ops::update<0>(t1, [&v](auto& x) { x *= v; }); }
template <typename V, typename ...Args>
std::tuple<Args...>& operator/=(std::tuple<Args...>& t1, const V& v) { return tuple_ops::update<0>(t1, [&v](auto& x) { x /= v; }); }

template <typename V, typename ...Args>
std::tuple<Args...> operator*(const V& v, std::tuple<Args...> t1) { return std::move(t1 *= v); }
template <typename V, typename ...Args>
std::tuple<Args...> operator*(std::tuple<Args...> t1, const V& v) { return std::move(t1 *= v); }
template <typename V, typename ...Args>
std::tuple<Args...> operator/(std::tuple<Args...> t1, const V& v) { return std::move(t1 /= v); }
```


**使用方法**

支持 'tuple' 的 `+ -` 操作， 以及 tuple 与 数字的 加减乘除操作。示例：

```c++
int main() {
    tuple<int, int, long long> t1 {1, 0, 2};
    tuple<int, int, long long> t2 {0, 4, 1};

    t1 += t2;   // [1, 4, 3]
    t1 *= 3;  // [3, 12, 9]
    t1 += 2;  // [5, 14, 11]
    t1 -= 3;  // [2, 11, 8]

    auto [x, y, z] = t1;
    cout << x << ", " << y << ", " << z << "\n"; 
    return 0;
}
```

### vector 操作

```c++
template <typename T>
vector<T>& operator+=(vector<T> &a, const vector<T> &b) {
    int n = a.size(); a.resize(n + b.size());
    copy(b.begin(), b.end(), a.begin() + n);
    return a;
}
template <typename T>
vector<T> operator+(const vector<T> &a, const vector<T> &b) { vector<T> c = a; return c += b;}
template <typename T>
vector<T>& operator+=(vector<T> &a, const T &b) { a.push_back(b); return a;}
template <typename T>
vector<T> operator+(const vector<T> &a, const T &b) { vector<T> c = a; return c += b;}
template <typename T>
vector<T>& operator+=(vector<T> &a, const T &&b) { a.push_back(b); return a;}
template <typename T>
vector<T> operator+(const vector<T> &a, const T &&b) { vector<T> c = a; return c += b;}
template <typename T>
vector<T>& operator*=(vector<T> &a, const int repeat) {
    int n = a.size(); a.resize(n * repeat);
    for (int i = 1; i < repeat; ++i) copy(a.begin(), a.end(), a.begin() + i * n);
    return a;
}
template <typename T>
vector<T> operator*(vector<T> &a, const int repeat) { vector<T> c = a; return c *= repeat;}
```

1. 两个vector相加。拼接

```c++
vector<int> a {1,2,3};
vector<int> b{4,5};
a += b; // a : {1, 2, 3,4, 5}
```

2. vector 与元素相加, 相当于与 push_back()

```c++
vector<int> a {1,2,3};
a += 4;
```

