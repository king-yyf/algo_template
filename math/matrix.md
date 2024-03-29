# modint

素数取模模板, 模板修改自suisen, 本地 sublime 代码补全缩写为 `mat`


### 模板代码

```c++
template <typename T>
struct Mat {
    vector<vector<T>> dat;

    Mat() = default;
    Mat(int n) : Mat(n, n) {}
    Mat(int n, int m, T fill_value = T(0)) : dat(n, vector<T>(m, fill_value)) {}
    Mat(const vector<vector<T>>& dat) : dat(dat) {}

    const vector<T>& operator[](int i) const { return dat[i]; }
    vector<T>& operator[](int i) { return dat[i]; }

    operator vector<vector<T>>() const { return dat; }

    friend bool operator==(const Mat<T>& A, const Mat<T>& B) { return A.dat == B.dat; }
    friend bool operator!=(const Mat<T>& A, const Mat<T>& B) { return A.dat != B.dat; }

    pair<int, int> shape() const { return dat.empty() ? make_pair<int, int>(0, 0) : make_pair<int, int>(dat.size(), dat[0].size()); }
    int row_size() const { return dat.size(); }
    int col_size() const { return dat.empty() ? 0 : dat[0].size(); }

    friend Mat<T>& operator+=(Mat<T>& A, const Mat<T>& B) {
        assert(A.shape() == B.shape());
        auto [n, m] = A.shape();
        for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) A.dat[i][j] += B.dat[i][j];
        return A;
    }
    friend Mat<T>& operator-=(Mat<T>& A, const Mat<T>& B) {
        assert(A.shape() == B.shape());
        auto [n, m] = A.shape();
        for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) A.dat[i][j] -= B.dat[i][j];
        return A;
    }
    friend Mat<T>& operator*=(Mat<T>& A, const Mat<T>& B) { return A = A * B; }
    friend Mat<T>& operator*=(Mat<T>& A, const T& val) {
        for (auto& row : A.dat) for (auto& elm : row) elm *= val;
        return A;
    }
    friend Mat<T>& operator/=(Mat<T>& A, const T& val) { return A *= T(1) / val; }
    friend Mat<T>& operator/=(Mat<T>& A, const Mat<T>& B) { return A *= B.inv(); }

    friend Mat<T> operator+(Mat<T> A, const Mat<T>& B) { A += B; return A; }
    friend Mat<T> operator-(Mat<T> A, const Mat<T>& B) { A -= B; return A; }
    friend Mat<T> operator*(const Mat<T>& A, const Mat<T>& B) {
        assert(A.col_size() == B.row_size());
        const int n = A.row_size(), m = A.col_size(), l = B.col_size();

        if (min({ n, m, l }) <= 70) {
            Mat<T> C(n, l);
            for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) for (int k = 0; k < l; ++k) {
                C.dat[i][k] += A.dat[i][j] * B.dat[j][k];
            }
            return C;
        }
        // strassen
        const int nl = 0, nm = n >> 1, nr = nm + nm;
        const int ml = 0, mm = m >> 1, mr = mm + mm;
        const int ll = 0, lm = l >> 1, lr = lm + lm;
        auto A00 = A.submat(nl, nm, ml, mm), A01 = A.submat(nl, nm, mm, mr);
        auto A10 = A.submat(nm, nr, ml, mm), A11 = A.submat(nm, nr, mm, mr);
        auto B00 = B.submat(ml, mm, ll, lm), B01 = B.submat(ml, mm, lm, lr);
        auto B10 = B.submat(mm, mr, ll, lm), B11 = B.submat(mm, mr, lm, lr);
        auto P0 = (A00 + A11) * (B00 + B11), P1 = (A10 + A11) * B00;
        auto P2 = A00 * (B01 - B11), P3 = A11 * (B10 - B00);
        auto P4 = (A00 + A01) * B11, P5 = (A10 - A00) * (B00 + B01), P6 = (A01 - A11) * (B10 + B11);

        Mat<T> C(n, l);

        C.assign_submat(nl, ll, P0 + P3 - P4 + P6), C.assign_submat(nl, lm, P2 + P4);
        C.assign_submat(nm, ll, P1 + P3), C.assign_submat(nm, lm, P0 + P2 - P1 + P5);
        if (l != lr) for (int i = 0; i < nr; ++i) for (int j = 0; j < mr; ++j) C.dat[i][lr] += A.dat[i][j] * B.dat[j][lr];
        if (m != mr) for (int i = 0; i < nr; ++i) for (int k = 0; k < l; ++k) C.dat[i][k] += A.dat[i][mr] * B.dat[mr][k];
        if (n != nr) for (int j = 0; j < m; ++j) for (int k = 0; k < l; ++k) C.dat[nr][k] += A.dat[nr][j] * B.dat[j][k];

        return C;
    }
    friend Mat<T> operator*(const T& val, Mat<T> A) { A *= val; return A; }
    friend Mat<T> operator*(Mat<T> A, const T& val) { A *= val; return A; }
    friend Mat<T> operator/(const Mat<T>& A, const Mat<T>& B) { return A * B.inv(); }
    friend Mat<T> operator/(Mat<T> A, const T& val) { A /= val; return A; }
    friend Mat<T> operator/(const T& val, const Mat<T>& A) { return val * A.inv(); }

    friend vector<T> operator*(const Mat<T>& A, const vector<T>& x) {
        const auto [n, m] = A.shape();
        assert(m == int(x.size()));
        vector<T> b(n, T(0));
        for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) b[i] += A.dat[i][j] * x[j];
        return b;
    }

    static Mat<T> e0(int n) { return Mat<T>(n, Identity::ADD); }
    static Mat<T> e1(int n) { return Mat<T>(n, Identity::MUL); }

    Mat<T> pow(long long b) const {
        assert_square();
        assert(b >= 0);
        Mat<T> res = e1(row_size()), p = *this;
        for (; b; b >>= 1) {
            if (b & 1) res *= p;
            p *= p;
        }
        return res;
    }
    Mat<T> inv() const { return *safe_inv(); }

    optional<Mat<T>> safe_inv() const {
        assert_square();
        Mat<T> A = *this;
        const int n = A.row_size();
        for (int i = 0; i < n; ++i) {
            A[i].resize(2 * n, T{ 0 });
            A[i][n + i] = T{ 1 };
        }
        for (int i = 0; i < n; ++i) {
            for (int k = i; k < n; ++k) if (A[k][i] != T{ 0 }) {
                swap(A[i], A[k]);
                T c = T{ 1 } / A[i][i];
                for (int j = i; j < 2 * n; ++j) A[i][j] *= c;
                break;
            }
            if (A[i][i] == T{ 0 }) return nullopt;
            for (int k = 0; k < n; ++k) if (k != i and A[k][i] != T{ 0 }) {
                T c = A[k][i];
                for (int j = i; j < 2 * n; ++j) A[k][j] -= c * A[i][j];
            }
        }
        for (auto& row : A.dat) row.erase(row.begin(), row.begin() + n);
        return A;
    }

    T det() const {
        assert_square();
        Mat<T> A = *this;
        bool sgn = false;
        const int n = A.row_size();
        for (int j = 0; j < n; ++j) for (int i = j + 1; i < n; ++i) if (A[i][j] != T{ 0 }) {
            swap(A[j], A[i]);
            T q = A[i][j] / A[j][j];
            for (int k = j; k < n; ++k) A[i][k] -= A[j][k] * q;
            sgn = not sgn;
        }
        T res = sgn ? T{ -1 } : T{ +1 };
        for (int i = 0; i < n; ++i) res *= A[i][i];
        return res;
    }
    T det_arbitrary_mod() const {
        assert_square();
        Mat<T> A = *this;
        bool sgn = false;
        const int n = A.row_size();
        for (int j = 0; j < n; ++j) for (int i = j + 1; i < n; ++i) {
            for (; A[i][j].val(); sgn = not sgn) {
                swap(A[j], A[i]);
                T q = A[i][j].val() / A[j][j].val();
                for (int k = j; k < n; ++k) A[i][k] -= A[j][k] * q;
            }
        }
        T res = sgn ? -1 : +1;
        for (int i = 0; i < n; ++i) res *= A[i][i];
        return res;
    }
    void assert_square() const { assert(row_size() == col_size()); }

    Mat<T> submat(int r1, int r2, int c1, int c2) const {
        Mat<T> A(r2 - r1, c2 - c1);
        for (int i = r1; i < r2; ++i) for (int j = c1; j < c2; ++j) {
            A[i - r1][j - c1] = dat[i][j];
        }
        return A;
    }
    void assign_submat(int r1, int c1, const Mat<T>& A) {
        const int n = A.row_size(), m = A.col_size();
        assert(r1 + n <= row_size() and c1 + m <= col_size());
        for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) {
            dat[r1 + i][c1 + j] = A[i][j];
        }
    }
private:
    enum class Identity { ADD, MUL };
    Mat(int n, Identity ident) : Mat<T>::Mat(n) {
        if (ident == Identity::MUL) for (int i = 0; i < n; ++i) dat[i][i] = 1;
    }
};
```

### 使用说明


1. 声明 Matrix 并赋值

```c++
Mat<mint> A(n);
for (int i = 0; i < n; ++i) {
    for (int j = 0, val; j < n; ++j) {
        std::cin >> val, A[i][j] = val;
    }
}
```
或者

```c++
vector<vector<mint>> a {{0, 1}, {1, 0}};
Mat<mint> A(a);
```

2. 矩阵四则运算

维护需要满足四则运算条件

```c++
Mat<mint> A(a), B(b);

A += B;
A -= B;
A *= B;
```

3. 矩阵快速幂

求矩阵的k次方

```c++
Mat<mint> A(a);
long long k = 1e18;

auto T = A.pow(k);
```

4. 矩阵行列式

```c++
Mat<mint> A(a);
int det = A.det().val();
```


