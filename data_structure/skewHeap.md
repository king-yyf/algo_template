# 斜堆

斜堆是一个支持合并操作的堆数据结构
功能：
+ 添加一个元素 O(logn)
+ 弹出堆顶元素 O(logn)
+ 合并两个堆 O(logn + logm)
+ 给堆中所有元素都加上一个值 O(1)
### 模板代码


```c++
template <typename T, bool min_mode = true>
struct SkewHeap {
    struct node {
        node *l, *r;
        T key, laz;
        bool operator < (const node& o) const {
            return  min_mode ? key + laz < o.key + o.laz : key + laz > o.key + o.laz;
        }
    } *root;
    SkewHeap() : root(0) { }
    void propagate(node *a) {
        if (a->laz == 0) return;
        a->key += a->laz;
        if (a->l) a->l->laz += a->laz;
        if (a->r) a->r->laz += a->laz;
        a->laz = 0;
    }
    node *merge(node *a, node *b) {
        if (!a || !b) return a ? a : b;
        if (!((*a) < (*b))) swap(a, b);
        propagate(a); propagate(b);
        a->r = merge(b, a->r);
        swap(a->l, a->r);
        return a;
    }
    void push(T key) {
        node *n = new node();
        n->l = n->r = 0;
        n->key = key; n->laz = 0;
        root = merge(root, n);
    }
    void pop() {
        propagate(root);
        node *temp = root;
        root = merge(root->l, root->r);
    }
    T top() {
        propagate(root);
        return root->key;
    }
    bool empty() { 
        return !root;
    }
    void add(T laz) {
        if (root) root->laz += laz;
    }
    void merge(SkewHeap x) { // destroy x
        root = merge(root, x.root);
    }
};
```

### 使用说明

1. 定义一个 SkewHeap

默认小根堆，如果定义大根堆，设置 min_mode = false.

```c++
SkewHeap<int> q;
SkewHeap<int,min_mode = false> qmx;
```

2. 插入元素

```c++
q.push(x);
```

3. 返回堆顶元素

```c++
q.top();
```

4. 弹出堆顶

```c++
q.pop();
```

5. 合并堆

```c++
q.merge(q2);
```

6. 查询堆是否为空

```c++
q.empty();
```
