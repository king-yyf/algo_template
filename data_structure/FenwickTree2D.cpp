#include<bits/stdc++.h>
using namespace std;

template<typename T>
struct FenwickTree2D{
    vector<vector<T>> tr;
    int n, m;
    FenwickTree2D(int N, int M) : n(N), m(M), tr(N, vector<T>(M, 0)){}
    FenwickTree2D(vector<vector<T>> A) : n(A.size()), m(A[0].size()){
        tr.assign(n, vector<int> (m));
        for (int i = 0; i < n; ++i) 
            for (int j = 0; j < m; ++j)
                add(i, j, A[i][j]);
    }
    void add(int x, int y, int val){ // 0 <= x < n, 0 <= y < m
        for(int i = x + 1; i <= n; i += i & -i)
            for(int j = y + 1; j <= m; j += j & -j)
                tr[i - 1][j - 1] += val;
    }
    T ask(int x, int y) { // 0 <= x < n, 0 <= y < m
        T ret = 0;
        for(int i = x + 1; i > 0; i -= i & -i)
            for(int j = y + 1; j > 0; j -= j & -j)
                ret += tr[i - 1][j - 1];
        return ret;
    }
    T ask(int x1, int y1, int x2, int y2) {  // sum[x1..x2, y1..y2]
        return ask(x2, y2) - ask(x2, y1 - 1) - ask(x1 - 1, y2) + ask(x1 - 1, y1 - 1);
    }
};

int main() {
    
    vector<vector<int>> a {
        {4, 2, 5, 9, 8},
        {0, 12, 3, 6, 4},
        {15, 7, 1, 1, 2},
        {9, 0, 12, 4, 3}
    };

    int n = a.size(), m = a[0].size();  // 4, 5

    cout << "n = " << n << ", m = " << m << "\n";

    FenwickTree2D<int> fen(a);

    // 41
    cout << fen.ask(1, 3) << "\n";

    // a[2][3] = a[2][3] + 2
    fen.add(2, 3, 2);

    // 1 + 2 = 3
    cout << fen.ask(2, 3, 2, 3) << "\n";
}