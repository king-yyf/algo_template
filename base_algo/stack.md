# 单调栈

单调栈模板 sumlime 本地代码补全缩写为 `stack`


### 模板代码

```c++
template <typename T, class LF, class RF>
void monoStack(vector<T> &a, vector<int> &lt, vector<int> &rt, LF f1, RF f2) {
    int n = a.size();
    lt.assign(n, -1), rt.assign(n, n);
    stack<int> sk;
    for (int i = 0; i < n; ++i) {
        while(!sk.empty() && f1(a[i],a[sk.top()])) {
            rt[sk.top()] = i;
            sk.pop();
        }
        sk.push(i);
    }
    sk = stack<int>();
    for (int i = n - 1; i >= 0; --i) {
        while (!sk.empty() && f2(a[i],a[sk.top()])) {
            lt[sk.top()] = i;
            sk.pop();
        }
        sk.push(i);
    }
}
template <typename T>  // 左右第一个更小的数，即以当前元素为最小值的区间
void le_le(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, less<T>(), less<T>());
}
template <typename T>  // 左右第一个更小的数，右侧是小于等于
void le_le_eq(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, less<T>(), less_equal<T>());
}
template <typename T> // 左右第一个更大的数，即以当前元素为最大值的区间
void gt_gt(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, greater<T>(), greater<T>());
}
template <typename T> // 左右第一个更大的数，右侧是大于等于
void gt_gt_eq(vector<T> &a, vector<int> &lt, vector<int> &rt) {
    monoStack(a, lt, rt, greater<T>(), greater_equal<T>());
}
```

### 使用说明


1. 求左边和右边第一个比当前元素小的元素下标

即以当前元素为最小值的区间

```c++
vector<int> a {5,6,3,6,4,7,3,8};
vector<int> left, right;
le_le(a, left, right);
```
2. 求左边第一个比当前元素小，右边第一个小于等于当前元素的元素下标

一边不等于，一边等于，常用于不重复计数。

```c++
vector<int> a {5,6,3,6,4,7,3,8};
vector<int> left, right;
le_le_eq(a, left, right);
```

3. 求左边和右边第一个比当前元素大的元素下标

```c++
vector<int> a {5,6,3,6,4,7,3,8};
vector<int> left, right;
gt_gt(a, left, right);
```

4. 求左边第一个比当前元素大，右边第一个大于等于当前元素的元素下标

即以当前元素为最大值的区间

```c++
vector<int> a {5,6,3,6,4,7,3,8};
vector<int> left, right;
gt_gt_eq(a, left, right);
```