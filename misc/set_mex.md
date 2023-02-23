# 带修改的mex

mex 指集合中不包含的最小非负整数，例如 mex({0,1,3}) = 2, mex({1,2,5}) = 0


### 模板代码

```c++
struct Mex {
    map<int, int> freq;
    set<int> s;
    vector<int> a;
    Mex(vector<int> const& A) : a(A) {
        for (int i = 0; i <= a.size(); i++) s.insert(i);
        for (int x : a) {
            ++freq[x];
            s.erase(x);
        }
    }
    int mex() { return *s.begin();}
    void update(int x, int val) {
        if (--freq[a[x]] == 0) s.insert(a[x]);
        a[x] = val;
        ++freq[val];
        s.erase(val);
    }
};
```

### 使用说明

1. 定义一个Mex， 时间复杂度 O(Nlog(N))

```c++
vector<int> a{2,4,6};
Mex<int> mex(a);
```

2. 修改下标x处的值,时间复杂度 O(log(N))

```c++
mex.update(x, val);
```

3. 获取当前的mex值，时间复杂度 O(1)

```c++
auto x = mex.mex()
```
