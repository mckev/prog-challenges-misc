// https://codeforces.com/problemset/problem/1732/C2

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>


const int UNDEFINED = -1;
const int MAX_ELEMENTS = 100000;


class BruteforceSolution {
public:
    std::vector<std::pair<int, int>> solve(const std::vector<long long>& elements, std::vector<std::pair<int, int>>& queries) const {
        std::vector<std::pair<int, int>> answers;
        for (const std::pair<int, int>& query : queries) {
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
            answers.push_back(answer);
        }
        return answers;
    }
};


/*
 * Disjoint Sparse Table
 *
 * Usage:
 * - Initialize with a vector<T>.
 * - Call init() to prepare internal structures.
 * - query(l, r) returns the combined result over [l, r].
 *
 * Notes:
 * - 0-based indexing.
 * - Modify `comb` and `id` for different associative operations:
 *   For sum: comb = +, id = 0
 *   For product: comb = *, id = 1
 *   For gcd: comb = gcd, id = 0
 *   For min: comb = min, id = large_value (e.g. 1e18)
 * - If TLE occurs, consider using integer arrays instead of vectors.
 */

template <class T>
struct RangeQuerySum {
    int n;                                  // log size
    std::vector<std::vector<T>> stor;       // precomputed prefix/suffix combinations
    std::vector<T> a;                       // input array, padded to power of two
    T id = 0;                               // identity element for `comb`

    // Define associative operation here
    T comb(const T& x, const T& y) const { return x + y; }

    RangeQuerySum(const std::vector<T>& arr) : a(arr) {
        n = 0;
        while ((1 << n) < (int)a.size()) ++n;
        a.resize(1 << n, id);
        stor.assign(1 << n, std::vector<T>(n + 1, id));
    }

    void fill(int l, int r, int depth) {
        if (depth < 0) return;
        int m = (l + r) / 2;
        T prefix = id;
        for (int i = m - 1; i >= l; --i)
            stor[i][depth] = prefix = comb(a[i], prefix);
        T suffix = id;
        for (int i = m; i < r; ++i)
            stor[i][depth] = suffix = comb(suffix, a[i]);
        fill(l, m, depth - 1);
        fill(m, r, depth - 1);
    }

    void init() {
        fill(0, 1 << n, n - 1);
    }

    // Query associative combination over [l, r]
    T query(int l, int r) const {
        if (l == r) return a[l];
        int t = 31 - __builtin_clz(l ^ r);
        return comb(stor.at(l).at(t), stor.at(r).at(t));
    }
};


template <class T>
struct RangeQueryXor {
    int n;                                  // log size
    std::vector<std::vector<T>> stor;       // precomputed prefix/suffix combinations
    std::vector<T> a;                       // input array, padded to power of two
    T id = 0;                               // identity element for `comb`

    // Define associative operation here
    T comb(const T& x, const T& y) const { return x ^ y; }

    RangeQueryXor(const std::vector<T>& arr) : a(arr) {
        n = 0;
        while ((1 << n) < (int)a.size()) ++n;
        a.resize(1 << n, id);
        stor.assign(1 << n, std::vector<T>(n + 1, id));
    }

    void fill(int l, int r, int depth) {
        if (depth < 0) return;
        int m = (l + r) / 2;
        T prefix = id;
        for (int i = m - 1; i >= l; --i)
            stor[i][depth] = prefix = comb(a[i], prefix);
        T suffix = id;
        for (int i = m; i < r; ++i)
            stor[i][depth] = suffix = comb(suffix, a[i]);
        fill(l, m, depth - 1);
        fill(m, r, depth - 1);
    }

    void init() {
        fill(0, 1 << n, n - 1);
    }

    // Query associative combination over [l, r]
    T query(int l, int r) const {
        if (l == r) return a[l];
        int t = 31 - __builtin_clz(l ^ r);
        return comb(stor.at(l).at(t), stor.at(r).at(t));
    }
};


static std::vector<int> index_range;
class Solution {
public:
    std::vector<std::pair<int, int>> solve(const std::vector<long long>& elements, std::vector<std::pair<int, int>>& queries) const {
        std::vector<std::pair<int, int>> answers;
        RangeQuerySum<long long> range_sum = RangeQuerySum(elements); range_sum.init();
        RangeQueryXor<long long> range_xor = RangeQueryXor(elements); range_xor.init();
        std::vector<int> skip_zero(elements.size(), 0);
        {
            for (int i = elements.size() - 1; i >= 0; i--) {
                if (elements[i] != 0) continue;
                if (i + 1 < elements.size()) {
                    skip_zero[i] = skip_zero[i + 1] + 1;
                } else {
                    skip_zero[i] = 1;
                }
            }
        }
        for (const std::pair<int, int>& query : queries) {
            std::pair<int, int> answer = {UNDEFINED, UNDEFINED};
            long long global_max = range_sum.query(query.first - 1, query.second - 1) - range_xor.query(query.first - 1, query.second - 1);
            for (int i = query.first - 1; i < query.second; ) {
                long long local_max = range_sum.query(i, query.second - 1) - range_xor.query(i, query.second - 1);
                if (local_max < global_max) break;
                auto it = std::lower_bound(index_range.begin() + i, index_range.begin() + query.second, local_max, [&range_sum, &range_xor, i](int j, long long val) {
                    return range_sum.query(i, j) - range_xor.query(i, j) < val;
                });
                int j = std::distance(index_range.begin(), it);
                if (answer.first == UNDEFINED || j - i < answer.second - answer.first) {
                    answer = {i + 1, j + 1};
                }
                if (elements[i] == 0) {
                    i += skip_zero[i];
                } else {
                    i++;
                }
            }
            answers.push_back(answer);
        }
        return answers;
    }
};


void test() {
    {
        // Test #1, test case 5
        std::vector<long long> elements = {21, 32, 32, 32, 10};
        std::vector<std::pair<int, int>> queries = {
            {1, 5},
            {1, 4},
            {1, 3},
            {2, 5},
            {3, 5}
        };
        Solution solution = Solution();
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<std::pair<int, int>> answers_efficient = solution.solve(elements, queries);
        std::vector<std::pair<int, int>> answers_bruteforce = bruteforce_solution.solve(elements, queries);
        std::vector<std::pair<int, int>> expected = {
            {2, 3},
            {2, 3},
            {2, 3},
            {2, 3},
            {3, 4}
        };
        assert(answers_bruteforce == expected);
        assert(answers_efficient == expected);
    }
    {
        // Test #1, test case 6
        std::vector<long long> elements = {0, 1, 0, 1, 0, 1, 0};
        std::vector<std::pair<int, int>> queries = {
            {1, 7},
            {3, 6},
            {2, 5},
            {1, 4},
            {4, 7},
            {2, 6},
            {2, 7}
        };
        Solution solution = Solution();
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<std::pair<int, int>> answers_efficient = solution.solve(elements, queries);
        std::vector<std::pair<int, int>> answers_bruteforce = bruteforce_solution.solve(elements, queries);
        std::vector<std::pair<int, int>> expected = {
            {2, 4},
            {4, 6},
            {2, 4},
            {2, 4},
            {4, 6},
            {2, 4},
            {2, 4}
        };
        assert(answers_bruteforce == expected);
        assert(answers_efficient == expected);
    }
    {
        // Test #2, test case 5
        std::vector<long long> elements = {5, 5, 4, 1};
        std::vector<std::pair<int, int>> queries = {
            {1, 4},
            {3, 4},
            {1, 3},
            {3, 4}
        };
        Solution solution = Solution();
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<std::pair<int, int>> answers_efficient = solution.solve(elements, queries);
        std::vector<std::pair<int, int>> answers_bruteforce = bruteforce_solution.solve(elements, queries);
        assert(answers_efficient == answers_bruteforce);
    }
    {
        // Test #4
        std::vector<long long> elements;
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            elements.push_back(0);
        }
        elements[90000] = 1;
        elements[90001] = 1;
        std::vector<std::pair<int, int>> queries;
        for (int q = 0; q < MAX_ELEMENTS; q++) {
            std::pair<int, int> query = {1, MAX_ELEMENTS};
            queries.push_back(query);
        }
        Solution solution = Solution();
        std::vector<std::pair<int, int>> answers_efficient = solution.solve(elements, queries);
        for (const std::pair<int, int>& answer_efficient : answers_efficient) {
            std::pair<int, int> expected = {90001, 90002};
            assert(answer_efficient == expected);
        }
    }
    {
        // Generate tests
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 100000; T++) {
            int N = int(rnd(mt) * 10) + 1;                           // [1, 10000]
            std::vector<long long> elements;
            for (int n = 0; n < N; n++) {
                long long element = (long long) (rnd(mt) * 10);
                elements.push_back(element);
            }
            std::vector<std::pair<int, int>> queries;
            for (int n = 0; n < N; n++) {
                int l = int(rnd(mt) * N) + 1;
                int r = int(rnd(mt) * N) + 1;
                if (l > r) {
                    int swap = l;
                    l = r;
                    r = swap;
                }
                std::pair<int, int> query = {l, r};
                queries.push_back(query);

            }
            Solution solution = Solution();
            BruteforceSolution bruteforce_solution = BruteforceSolution();
            std::vector<std::pair<int, int>> answers_efficient = solution.solve(elements, queries);
            std::vector<std::pair<int, int>> answers_bruteforce = bruteforce_solution.solve(elements, queries);
            assert(answers_efficient == answers_bruteforce);
        }
    }
}


int main() {
    for (int i = 0; i <= MAX_ELEMENTS; i++) index_range.push_back(i);
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int Q; std::cin >> Q;
        std::vector<long long> elements;
        for (int n = 0; n < N; n++) {
            int element; std::cin >> element;
            elements.push_back(element);
        }
        std::vector<std::pair<int, int>> queries;
        for (int q = 0; q < Q; q++) {
            int l; std::cin >> l;
            int r; std::cin >> r;
            queries.push_back({l, r});
        }
        Solution solution = Solution();
        std::vector<std::pair<int, int>> answers = solution.solve(elements, queries);
        for (const std::pair<int, int>& answer : answers) {
            std::cout << answer.first << " " << answer.second << std::endl;
        }
    }
    return 0;
}
