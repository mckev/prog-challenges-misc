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


class Solution {
private:
        static long long range_query_sum(std::vector<long long>& prefix_sum, int l, int r) {
            // [l - r], 0 based indexing
            return prefix_sum.at(r) - (l - 1 >= 0 ? prefix_sum.at(l - 1) : 0);
        }
        static long long range_query_xor(std::vector<long long>& prefix_xor, int l, int r) {
            // [l - r], 0 based indexing
            return prefix_xor.at(r) ^ (l - 1 >= 0 ? prefix_xor.at(l - 1) : 0);
        }

public:
    std::vector<std::pair<int, int>> solve(const std::vector<long long>& elements, std::vector<std::pair<int, int>>& queries) const {
        std::vector<std::pair<int, int>> answers;

        // Optimization: Index Range (for lower bound)
        std::vector<int> index_range(elements.size());
        for (int i = 0; i < elements.size(); i++) {
            index_range[i] = i;
        }

        // Optimization: Range Query
        std::vector<long long> prefix_sum(elements.size());
        for (int i = 0; i < elements.size(); i++) {
            prefix_sum[i] = (i - 1 >= 0 ? prefix_sum[i - 1] : 0) + elements[i];
        }
        std::vector<long long> prefix_xor(elements.size());
        for (int i = 0; i < elements.size(); i++) {
            prefix_xor[i] = (i - 1 >= 0 ? prefix_xor[i - 1] : 0) ^ elements[i];
        }

        // Optimization: Skip zero
        std::vector<int> skip_zero(elements.size(), 0);
        for (int i = elements.size() - 1; i >= 0; i--) {
            if (elements[i] != 0) continue;
            skip_zero[i] = (i + 1 < elements.size() ? skip_zero[i + 1] : 0) + 1;
        }

        // Solve
        for (const std::pair<int, int>& query : queries) {
            std::pair<int, int> answer = {UNDEFINED, UNDEFINED};
            long long global_max = range_query_sum(prefix_sum, query.first - 1, query.second - 1) - range_query_xor(prefix_xor, query.first - 1, query.second - 1);
            for (int i = query.first - 1; i < query.second; ) {
                // Example elements[]: 21, 32, 32, 32, 10
                // Case i == 0:   0  0 64 64 64
                // Here we can calculate the local_max (e.g. 64) in O(1) using Range Query
                long long local_max = range_query_sum(prefix_sum, i, query.second - 1) - range_query_xor(prefix_xor, i, query.second - 1);
                if (local_max < global_max) break;
                // Here we can find the first index "j" to reach local_max (e.g. index 2) in O(log N) using Binary Search (i.e. lower bound)
                auto it = std::lower_bound(index_range.begin() + i, index_range.begin() + query.second, local_max, [&prefix_sum, &prefix_xor, i](int j, long long val) {
                    return range_query_sum(prefix_sum, i, j) - range_query_xor(prefix_xor, i, j) < val;
                });
                int j = std::distance(index_range.begin(), it);
                if (answer.first == UNDEFINED || j - i < answer.second - answer.first) {
                    answer = {i + 1, j + 1};
                }
                // Skip elements of 0, as it does not affect sum and xor
                if (elements[i] == 0) {
                    i += skip_zero.at(i);
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
            int N = int(rnd(mt) * 10) + 1;
            std::vector<long long> elements;
            for (int n = 0; n < N; n++) {
                long long element = (long long) (rnd(mt) * 10);
                elements.push_back(element);
            }
            std::vector<std::pair<int, int>> queries;
            for (int n = 0; n < N; n++) {
                int l = int(rnd(mt) * N) + 1;                           // [1, N]
                int r = int(rnd(mt) * N) + 1;                           // [1, N]
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
    test();
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
