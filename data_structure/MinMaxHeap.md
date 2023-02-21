# 最大最小堆

堆数据结构，支持插入元素，查询最大/小值，删除最大/小值。

其中 push和查询min/max操作的均摊复杂度位O(1)
pop_min()和pop_max()的复杂度位O(log(n))

### 模板代码


```c++
template <class T, class L = less<T>, class G = greater<T>>
struct MinMaxHeap {
    priority_queue<T, vector<T>, G> minh, minp;
    priority_queue<T, vector<T>, L> maxh, maxp;
    void norm() {
        while (!minp.empty() && minp.top() == minh.top()) {
            minp.pop();
            minh.pop();
        }
        while (!maxp.empty() && maxp.top() == maxh.top()) {
            maxp.pop();
            maxh.pop();
        }
    }
    void push(T x) { minh.push(x); maxh.push(x); }
    T min() { norm(); return minh.top(); }
    T max() { norm(); return maxh.top(); }
    void pop_min() { norm(); maxp.push(minh.top()); minh.pop(); }
    void pop_max() { norm(); minp.push(maxh.top()); maxh.pop(); }
};
```

### 使用说明

1. 定义一个 MinMaxHeap

```c++
MinMaxHeap<int> q;
```


2. 插入元素

```c++
q.push(x);
```

3. 查询最大/最小值

```c++
q.max(); q.min();
```

4. 删除最大值/最小值，最值存在多个时删除其中的一个

```c++
q.pop_min(); q.pop_max();
```
