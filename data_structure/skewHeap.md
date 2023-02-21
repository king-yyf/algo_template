# 斜堆

堆数据结构，支持插入元素，查询最大/小值，删除最大/小值。

其中 push和查询min/max操作的均摊复杂度位O(1)
pop_min()和pop_max()的复杂度位O(log(n))

### 模板代码


```c++

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
