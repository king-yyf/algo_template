# 单调队列

单调队列模板


### 模板代码

1. 滑动窗口最大值

```c++
template <typename T> 
vector<T> slideMax(vector<T>& a, int k) {
    deque<int> dq;
    vector<T> res;
    for (int i = 0; i < a.size(); ++i) {
        while (!dq.empty() && a[dq.back()] < a[i])
            dq.pop_back();
        dq.push_back(i);
        while (dq.front() <= i - k) dq.pop_front();
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}
```

2. 滑动窗口最小值

```c++
template <typename T> 
vector<T> slideMin(vector<T>& a, int k) {
    deque<int> dq;
    vector<T> res;
    for (int i = 0; i < a.size(); ++i) {
        while (!dq.empty() && a[dq.back()] > a[i])
            dq.pop_back();
        dq.push_back(i);
        while (dq.front() <= i - k) dq.pop_front();
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}
```
