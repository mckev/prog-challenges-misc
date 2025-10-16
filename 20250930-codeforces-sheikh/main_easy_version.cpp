// https://codeforces.com/problemset/problem/1732/C1

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>


const int UNDEFINED = -1;

class BruteforceSolution {
public:
    std::pair<int, int> solve(const std::vector<long long>& elements, std::pair<int, int>& query) const {
        std::pair<int, int> answer;
        long long best_val = UNDEFINED;
        for (int i = query.first - 1; i < query.second; i++) {
            long long _sum = 0;
            long long _xor = 0;
            for (int j = i; j < query.second; j++) {
                _sum += elements[j];
                _xor ^= elements[j];
                long long val = _sum - _xor;
                assert(val >= 0);             // This is key for optimization
                if (best_val == UNDEFINED || val > best_val || (val == best_val && (j - i < answer.second - answer.first))) {
                    best_val = _sum - _xor;
                    answer = {i + 1, j + 1};
                }
                // std::cout << val << " ";
            }
            // std::cout << std:: endl;
        }
        return answer;
    }
};


/*
 * Segment Tree
 */

/*
1) Works for any associative operation (sum, min, max, gcd, xor, etc.)
2) 0-based indexing.
3) Change `comb()` function for your operation.
4) Change `id` (identity value):
     - sum, diff: 0
     - mult, div: 1
     - min: 1e18 or numeric_limits<T>::max()
     - max: -1e18 or numeric_limits<T>::min()
     - gcd: 0
     - lcm: 1
5) Supports point updates and range queries.
6) Build from array in O(n), query and update in O(log n).
7) query: [l, r)
*/

template <class T>
struct RangeQuerySum {
    int n;
    std::vector<T> tree;
    T id; // identity element

    // === Associative combine function ===
    T comb(T a, T b) {
        return a + b; // modify according to your operation
    }

    RangeQuerySum(int sz, T id_val = 0) {
        id = id_val;
        n = 1;
        while (n < sz)
            n <<= 1;
        tree.assign(2 * n, id);
    }

    RangeQuerySum(const std::vector<T>& arr, T id_val = 0) {
        id = id_val;
        n = 1;
        while (n < (int)arr.size())
            n <<= 1;
        tree.assign(2 * n, id);
        build(arr);
    }

    // === Build from array ===
    void build(const std::vector<T>& arr, int x, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < (int)arr.size())
                tree[x] = arr[lx];
            return;
        }
        int mid = (lx + rx) / 2;
        build(arr, 2 * x + 1, lx, mid);
        build(arr, 2 * x + 2, mid, rx);
        tree[x] = comb(tree[2 * x + 1], tree[2 * x + 2]);
    }

    void build(const std::vector<T>& arr) {
        build(arr, 0, 0, n);
    }

    // === Point Update ===
    void set(int idx, T val, int x, int lx, int rx) {
        if (rx - lx == 1) {
            tree[x] = val;
            return;
        }
        int mid = (lx + rx) / 2;
        if (idx < mid)
            set(idx, val, 2 * x + 1, lx, mid);
        else
            set(idx, val, 2 * x + 2, mid, rx);

        tree[x] = comb(tree[2 * x + 1], tree[2 * x + 2]);
    }

    void set(int idx, T val) {
        set(idx, val, 0, 0, n);
    }

    // === Range Query ===
    T query(int l, int r, int x, int lx, int rx) {
        if (lx >= r || rx <= l)
            return id;
        if (lx >= l && rx <= r)
            return tree[x];

        int mid = (lx + rx) / 2;
        T left = query(l, r, 2 * x + 1, lx, mid);
        T right = query(l, r, 2 * x + 2, mid, rx);
        return comb(left, right);
    }

    T query(int l, int r) {
        return query(l, r, 0, 0, n);
    }
};


template <class T>
struct RangeQueryXor {
    int n;
    std::vector<T> tree;
    T id; // identity element

    // === Associative combine function ===
    T comb(T a, T b) {
        return a ^ b; // modify according to your operation
    }

    RangeQueryXor(int sz, T id_val = 0) {
        id = id_val;
        n = 1;
        while (n < sz)
            n <<= 1;
        tree.assign(2 * n, id);
    }

    RangeQueryXor(const std::vector<T>& arr, T id_val = 0) {
        id = id_val;
        n = 1;
        while (n < (int)arr.size())
            n <<= 1;
        tree.assign(2 * n, id);
        build(arr);
    }

    // === Build from array ===
    void build(const std::vector<T>& arr, int x, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < (int)arr.size())
                tree[x] = arr[lx];
            return;
        }
        int mid = (lx + rx) / 2;
        build(arr, 2 * x + 1, lx, mid);
        build(arr, 2 * x + 2, mid, rx);
        tree[x] = comb(tree[2 * x + 1], tree[2 * x + 2]);
    }

    void build(const std::vector<T>& arr) {
        build(arr, 0, 0, n);
    }

    // === Point Update ===
    void set(int idx, T val, int x, int lx, int rx) {
        if (rx - lx == 1) {
            tree[x] = val;
            return;
        }
        int mid = (lx + rx) / 2;
        if (idx < mid)
            set(idx, val, 2 * x + 1, lx, mid);
        else
            set(idx, val, 2 * x + 2, mid, rx);

        tree[x] = comb(tree[2 * x + 1], tree[2 * x + 2]);
    }

    void set(int idx, T val) {
        set(idx, val, 0, 0, n);
    }

    // === Range Query ===
    T query(int l, int r, int x, int lx, int rx) {
        if (lx >= r || rx <= l)
            return id;
        if (lx >= l && rx <= r)
            return tree[x];

        int mid = (lx + rx) / 2;
        T left = query(l, r, 2 * x + 1, lx, mid);
        T right = query(l, r, 2 * x + 2, mid, rx);
        return comb(left, right);
    }

    T query(int l, int r) {
        return query(l, r, 0, 0, n);
    }
};


class Solution {
public:
    std::pair<int, int> solve(const std::vector<long long>& elements, std::pair<int, int>& query) const {
        std::pair<int, int> answer = {UNDEFINED, UNDEFINED};

        RangeQuerySum<long long> range_sum = RangeQuerySum(elements, (long long) 0);
        RangeQueryXor<long long> range_xor = RangeQueryXor(elements, (long long) 0);
        long long global_max = range_sum.query(query.first - 1, query.second) - range_xor.query(query.first - 1, query.second);
        std::vector<int> index_range; for (int i = 0; i < query.second; i++) index_range.push_back(i);

        for (int i = query.first - 1; i < query.second; i++) {
            long long local_max = range_sum.query(i, query.second) - range_xor.query(i, query.second);
            if (local_max < global_max) break;
            auto it = std::lower_bound(index_range.begin() + i, index_range.begin() + query.second, local_max, [&range_sum, &range_xor, i](int j, long long val) {
                return range_sum.query(i, j + 1) - range_xor.query(i, j + 1) < val;
            });
            int j = std::distance(index_range.begin(), it);
            if (answer.first == UNDEFINED || j - i < answer.second - answer.first) {
                answer = {i + 1, j + 1};
            }
        }
        return answer;
    }
};


void test() {
    {
        // Test #1, test case 5
        std::vector<long long> elements = {21, 32, 32, 32, 10};
        std::vector<std::pair<int, int>> queries;
        std::pair<int, int> query = {1, 5};
        Solution solution = Solution();
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::pair<int, int> answer_efficient = solution.solve(elements, query);
        std::pair<int, int> answer_bruteforce = bruteforce_solution.solve(elements, query);
        std::pair<int, int> expected = {2, 3};
        assert(answer_bruteforce == expected);
        assert(answer_efficient == expected);
    }
    {
        // Test #1, test case 6
        std::vector<long long> elements = {0, 1, 0, 1, 0, 1, 0};
        std::vector<std::pair<int, int>> queries;
        std::pair<int, int> query = {1, 7};
        Solution solution = Solution();
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::pair<int, int> answer_efficient = solution.solve(elements, query);
        std::pair<int, int> answer_bruteforce = bruteforce_solution.solve(elements, query);
        std::pair<int, int> expected = {2, 4};
        assert(answer_bruteforce == expected);
        assert(answer_efficient == expected);
    }
    {
        // Test #4
        std::vector<long long> elements;
        for (int i = 0; i < 100000; i++) {
            elements.push_back(0);
        }
        std::pair<int, int> query = {1, 100000};
        Solution solution = Solution();
        std::pair<int, int> answer_efficient = solution.solve(elements, query);
        std::pair<int, int> expected = {1, 1};
        assert(answer_efficient == expected);
    }
    {
        // Generate tests
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 100; T++) {
            int N = int(rnd(mt) * 10000) + 1;                           // [1, 10000]
            std::vector<long long> elements;
            for (int n = 0; n < N; n++) {
                long long element = (long long) (rnd(mt) * 1000000000);
                elements.push_back(element);
            }
            std::pair<int, int> query = {1, N};
            Solution solution = Solution();
            BruteforceSolution bruteforce_solution = BruteforceSolution();
            std::pair<int, int> answer_efficient = solution.solve(elements, query);
            std::pair<int, int> answer_bruteforce = bruteforce_solution.solve(elements, query);
            assert(answer_efficient == answer_bruteforce);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int Q; std::cin >> Q;                   // 1 query (Q is 1)
        assert(Q == 1);
        std::vector<long long> elements;
        for (int n = 0; n < N; n++) {
            int element; std::cin >> element;
            elements.push_back(element);
        }
        std::vector<std::pair<int, int>> queries;
        for (int q = 0; q < Q; q++) {
            int l; std::cin >> l;
            assert(l == 1);
            int r; std::cin >> r;
            assert(r == N);
            queries.push_back({l, r});
        }
        Solution solution = Solution();
        std::pair<int, int> answer = solution.solve(elements, queries[0]);
        std::cout << answer.first << " " << answer.second << std::endl;
    }
    return 0;
}
