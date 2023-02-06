#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void wt(vector<int> &a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        cout << a[i] << " \n"[i == n - 1];    
    }
}

template <typename T>
struct Range: map<T, T> {
    T siz = 0;
    using map<T, T>::map, map<T, T>::upper_bound, map<T, T>::begin, map<T, T>::end, map<T, T>::emplace, map<T, T>::erase;
    static T len(const pair<T, T> &p) {return p.second - p.first + 1;}
    pair<vector<pair<T, T>>, typename map<T, T>::iterator> add(pair<T, T> p) {
        vector<pair<T, T>> ds;
        auto it = upper_bound(p.first);
        if (it != begin() && prev(it)->second + 1 >= p.first) {
            if (prev(it)->second >= p.second) return {{*prev(it)}, prev(it)};
            p.first = prev(it)->first;
            ds.emplace_back(*prev(it));
            it = erase(prev(it));
        }
        for (; it != end() && it->first <= p.second + 1; it = erase(it)) {
            if (it->second > p.second) p.second = it->second;
            ds.emplace_back(*it);
        }
        for (auto &range : ds) siz -= len(range);
        auto inserted = emplace(p.first, p.second).first;
        siz += len(*inserted);
        return {ds, inserted};
    }
    pair<vector<pair<T, T>>, vector<pair<T, T>>> del(pair<T, T> p) {
        vector<pair<T, T>> ds, as;
        auto it = upper_bound(p.first);
        if (it != begin() && prev(it)->second + 1 >= p.first) {
            it = prev(it);
            ds.emplace_back(*it);
            if (it->second >= p.second) {
                if (it->second > p.second) as.emplace_back(*emplace(p.second + 1, it->second).first);
                if (it->first < p.first) {
                    it->second = p.first - 1;
                    as.emplace_back(*it);
                } else
                    erase(it);
                for (auto &range : ds) siz -= len(range);
                for (auto &range : as) siz += len(range);
                return {ds, as};
            }
            if (it->first < p.first) {
                it->second = p.first - 1;
                as.emplace_back(*it);
                it = next(it);
            } else
                it = erase(it);
        }
        for (; it != end() && it->first <= p.second; it = erase(it)) {
            ds.emplace_back(*it);
            if (it->second > p.second) as.emplace_back(*emplace(p.second + 1, it->second).first);
        }
        for (auto &range : ds) siz -= len(range);
        for (auto &range : as) siz += len(range);
        return {ds, as};
    }
    typename map<T, T>::const_iterator any(const pair<T, T> &p) const {
        if (auto it = upper_bound(p.second); it != begin() && prev(it)->second >= p.first) return prev(it);
        return end();
    }
    typename map<T, T>::const_iterator all(const pair<T, T> &p) const {
        if (auto it = upper_bound(p.second); it != begin() && prev(it)->first <= p.first && prev(it)->second >= p.second) return prev(it);
        return end();
    }
    template <typename OS>
    friend OS &operator<<(OS &os, const Range &t) {
        os << '[';
        uint32_t index = 0;
        for (auto &[l, r] : t) {
            if (index++) os << ", ";
            os << '[' << l << ", " << r << ']';
        }
        return os << ']';
    }
};


int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    Range<int> rgs;

    rgs.add({1, 10});
    rgs.add({21, 30});
    rgs.add({5, 15});

    cout << "after addRange:" << rgs << endl;

    //移除区间
    auto [dels, inss] = rgs.del({13, 25});
    cout << "after removeRange:" << rgs << endl;

    cout << "during removing,\n";
    for (auto &[l, r] : dels) {
        cout << '[' << l << ',' << r << "] is deleted\n";
    }
    for (auto &[l, r] : inss) {
        cout << '[' << l << ',' << r << "] is inserted\n";
    }

    auto it = rgs.all({3, 8});

    if (it != rgs.end()) {
        cout << it->first << ", " << it->second << "\n";
    } 

    cout << rgs << endl;

    //维护区间总长度
    cout << "length of ranges = " << rgs.siz << endl;
    
    return 0;
}

输出
/*

after addRange:[[1, 15], [21, 30]]
after removeRange:[[1, 12], [26, 30]]
during removing,
[1,15] is deleted
[21,30] is deleted
[1,12] is inserted
[26,30] is inserted
1, 12
[[1, 12], [26, 30]]
length of ranges = 17

*/