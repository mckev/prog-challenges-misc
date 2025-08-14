// https://www.codechef.com/problems/SUBDIV

#include <cassert>
#include <iostream>
#include <set>


class Solution {
public:
    int solve(int N) {
        if (N == 0) {
            return 0;
        } else if (N % 2 == 1) {
            return N / 2 + 1;
        } else {
            return N / 2 + (N / 2 - 1) / 2 + 1;
        }
    }
};


class BruteforceSolution {
private:
    std::set<int> numbers;

    void recurse(int N) {
        if (N <= 0) return;
        bool is_exist = numbers.find(N) != numbers.end();
        if (is_exist) return;
        numbers.insert(N);

        if (N > 2) {
            recurse(N - 2);
        }
        if (N > 1 && N % 2 == 0) {
            recurse(N / 2);
        }
    }

public:
    int solve(int N) {
        numbers.clear();
        recurse(N);
        return numbers.size();
    }
};


void test() {
    {
        int N = 1;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 1);
        assert(answer2 == answer1);
    }
    {
        int N = 2;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 2);
        assert(answer2 == answer1);
    }
    {
        int N = 4;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 3);
        assert(answer2 == answer1);
    }
    {
        int N = 6;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 5);
        assert(answer2 == answer1);
    }
    {
        int N = 8;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 6);
        assert(answer2 == answer1);
    }
    {
        int N = 9;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 5);
        assert(answer2 == answer1);
    }
    {
        int N = 100;
        Solution solution = Solution();
        int answer1 = solution.solve(N);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        int answer2 = bruteforce_solution.solve(N);
        assert(answer1 == 75);
        assert(answer2 == answer1);
    }
    {
        for (int N = 0; N < 1000; N++) {
            Solution solution = Solution();
            int answer1 = solution.solve(N);
            BruteforceSolution bruteforce_solution = BruteforceSolution();
            int answer2 = bruteforce_solution.solve(N);
            assert(answer2 == answer1);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        Solution solution = Solution();
        int answer = solution.solve(N);
        std::cout << answer << std::endl;
    }
    return 0;
}
