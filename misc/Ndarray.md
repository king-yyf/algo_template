# n维数组

类似 numpy ndarray, 用来建立高维数组。


### 模板代码

```c++
template <typename T, int DIM> struct Ndarray {
    static_assert(DIM >= 0, "DIM must >= 0");
    array<int, DIM> shape, strides; T* data;
    Ndarray(array<int, DIM> sp, array<int, DIM> st, T* d) : shape(sp), strides(st), data(d) {}
    Ndarray() : shape{0}, strides{0}, data(nullptr) {}
    int flatten_index(array<int, DIM> idx, bool checked = false) const {
        int res = 0;
        for (int i = 0; i < DIM; i++) { 
            if (checked) assert(0 <= idx[i] && idx[i] < shape[i]);
            res += idx[i] * strides[i]; 
        }
        return res;
    }
    T& operator[] (array<int, DIM> idx) const { return data[flatten_index(idx)];}
    T& at(array<int, DIM> idx) const { return data[flatten_index(idx, true)];}
    template <int D = DIM>
    typename enable_if<(0 < D), Ndarray<T, DIM-1>>::type operator[] (int idx) const {
        array<int, DIM-1> nshape; copy(shape.begin()+1, shape.end(), nshape.begin());
        array<int, DIM-1> nstrides; copy(strides.begin()+1, strides.end(), nstrides.begin());
        T* ndata = data + (strides[0] * idx);
        return Ndarray<T, DIM-1>(nshape, nstrides, ndata);
    }
    template <int D = DIM> typename enable_if<(0 < D), Ndarray<T, DIM-1>>::type at(int x) const { 
        assert(0 <= x && x < shape[0]); return operator[](x);
    }
    template <int D = DIM> typename enable_if<(0 == D), T&>::type operator * () const { return *data;}
};
 
template <typename T, int DIM> struct ndarr {
    static_assert(DIM >= 0, "DIM must >= 0");
    array<int, DIM> shape, strides;
    int len; T* data;
    ndarr() : shape{0}, strides{0}, len(0), data(nullptr) {}
    explicit ndarr(array<int, DIM> shape_, const T& t = T()) {
        shape = shape_, len = 1;
        for (int i = DIM-1; i >= 0; i--) { strides[i] = len; len *= shape[i];}
        data = new T[len];
        fill(data, data + len, t);
    }
    ndarr(const ndarr& o) : shape(o.shape), strides(o.strides), len(o.len), data(new T[len]) {
        for (int i = 0; i < len; i++) data[i] = o.data[i];
    }
    ndarr& operator=(ndarr&& o) noexcept {
        swap(shape, o.shape); swap(strides, o.strides);
        swap(len, o.len); swap(data, o.data);
        return *this;
    }
    ndarr(ndarr&& o) : ndarr() { *this = move(o);}
    ndarr& operator=(const ndarr& o) { return *this = ndarr(o);}
    ~ndarr() { delete[] data; }
    using view_t = Ndarray<T, DIM>;
    view_t view() { return Ndarray<T, DIM>(shape, strides, data);}
    operator view_t() { return view(); }
    using const_view_t = Ndarray<const T, DIM>;
    const_view_t view() const { return Ndarray<const T, DIM>(shape, strides, data);}
    operator const_view_t() const { return view(); }
    T& operator[] (array<int, DIM> idx) { return view()[idx]; }
 
    T& at(array<int, DIM> idx) { return view().at(idx); }
    const T& operator[] (array<int, DIM> idx) const { return view()[idx]; }
    const T& at(array<int, DIM> idx) const { return view().at(idx); }
    template <int D = DIM> typename enable_if<(0 < D), Ndarray<T, DIM-1>>::type operator[] (int x) { return view()[x];}
    template <int D = DIM> typename enable_if<(0 < D), Ndarray<T, DIM-1>>::type at(int x) { return view().at(x);}
    template <int D = DIM> typename enable_if<(0 < D), Ndarray<const T, DIM-1>>::type operator[] (int x) const { return view()[x];}
    template <int D = DIM> typename enable_if<(0 < D), Ndarray<const T, DIM-1>>::type at(int x) const { return view().at(x);}
    template <int D = DIM> typename enable_if<(0 == D), T&>::type operator * () { return *view(); }
    template <int D = DIM> typename enable_if<(0 == D), const T&>::type operator * () const { return *view();}
};
```

### 使用说明

1. 创建一个ndarr

创建一个int类型，shape 为 3 * 4 * 10 的三维数组。 初始值位-1

```c++
ndarr<int, 3> f({3,4,10}, -1);
```

2. 访问 某个下标的元素

```c++
int x = f[{2, 3, 2}];
```

3. 给某个下标处的元素赋值

```c++
f[{i, j, k}] = 2;
```