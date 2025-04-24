// Kevin and Puzzle : https://codeforces.com/problemset/problem/2061/C

#include <cassert>
#include <iostream>
#include <vector>


class Solution {
    // To speed up
    std::vector<long long> caches;
    const long long UNDEFINED = -1;

    long long trace_path(const std::vector<int>& liars, int pos, int liar, bool is_honest) {
        if (is_honest) {
            // Incorrect path
            if (liar != liars[pos]) return 0;
        }
        if (pos == liars.size() - 1) {
            // Path is completed
            return 1;
        }

        if (is_honest && caches[pos] != UNDEFINED) return caches[pos];
        long long num_config;
        if (is_honest) {
            num_config = trace_path(liars, pos + 1, liar, true) + trace_path(liars, pos + 1, liar + 1, false);
            caches[pos] = num_config;
        } else {
            num_config = trace_path(liars, pos + 1, liar, true);
        }
        return num_config % 998244353;
    }

    public:
    int solve(const std::vector<int>& liars) {
        std::vector<long long> _caches(liars.size(), UNDEFINED); caches = _caches;
        long long num_config = trace_path(liars, 0, 0, true) + trace_path(liars, 0, 1, false);
        return num_config % 998244353;
    }
};


void test() {
    {
        Solution solution = Solution();
        std::vector<int> liars = {0, 1, 2};
        int num_config = solution.solve(liars);
        assert(num_config == 1);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {0, 0, 0, 0, 0};
        int num_config = solution.solve(liars);
        assert(num_config == 2);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {0, 0, 1, 1, 2};
        int num_config = solution.solve(liars);
        assert(num_config == 3);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {0, 1, 2, 3, 4};
        int num_config = solution.solve(liars);
        assert(num_config == 0);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {0, 0, 1, 1, 1};
        int num_config = solution.solve(liars);
        assert(num_config == 4);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {5, 1, 5, 2, 5};
        int num_config = solution.solve(liars);
        assert(num_config == 1);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {0};
        int num_config = solution.solve(liars);
        assert(num_config == 2);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {2, 3, 1, 1};
        int num_config = solution.solve(liars);
        assert(num_config == 0);
    }
    {
        Solution solution = Solution();
        std::vector<int> liars = {0, 0, 0, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 9, 9, 10, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 14, 14, 15, 15, 15, 16, 16, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 29, 29, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 35, 35, 35, 35, 35, 36, 36, 36};
        int num_config = solution.solve(liars);
        assert(num_config == 442368);
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<int> liars;
        for (int n = 0; n < N; n++) {
            int liar; std::cin >> liar;
            liars.push_back(liar);
        }
        Solution solution = Solution();
        int num_config = solution.solve(liars);
        std::cout << num_config << std::endl;
    }
    return 0;
}
