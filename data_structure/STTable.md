# Sparse Table

ST 表是用于解决 可重复贡献问题 的数据结构。

sublime 代码补全的缩写为 **stt**

**可重复贡献问题**

可重复贡献问题 是指对于运算 opt，满足 `x opt x = x`，则对应的区间询问就是一个可重复贡献问题。

另外，opt还必须满足结合律才能使用 ST 表求解。

ST 表可以处理问题示例

+ 区间最值(最大值、最小值)
+ 区间 gcd
+ 区间按位与 
+ 区间按位或

### 模板代码


```c++
template <class T, T (*op)(T, T)>
class ST {
 public:
  int n;
  vector<vector<T>> mat;
 
  ST(const vector<T>& a) {
    n = static_cast<int>(a.size());
    int max_log = 32 - __builtin_clz(n);
    mat.resize(max_log);
    mat[0] = a;
    for (int j = 1; j < max_log; j++) {
      mat[j].resize(n - (1 << j) + 1);
      for (int i = 0; i <= n - (1 << j); i++) {
        mat[j][i] = op(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
      }
    }
  }
 
  T get(int from, int to) const {
    assert(0 <= from && from <= to && to <= n - 1);
    int lg = 32 - __builtin_clz(to - from + 1) - 1;
    return op(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
  }
};
```

### 使用说明

1. 定义一个 SparseTable

```c++
int op(int x,int y) {return max(x,y);}
vector<int> a;
ST<int,op> s(a);
```


2. 查询区间 [l, r] 聚合值 

```c++
s.get(l, r)
```
