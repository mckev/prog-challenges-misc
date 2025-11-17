// https://leetcode.com/problems/count-distinct-integers-after-removing-zeros/description/

#include <cassert>
#include <random>
#include <set>
#include <vector>


class SolutionBruteforce {
public:
    long long countDistinct(long long n) const {
        long long answer = 0;
        std::set<long long> history;
        for (long long x = 1; x <= n; x++) {
            std::string s = std::to_string(x);
            // s shall not contain '0'
            if (s.find('0') != std::string::npos) continue;
            // s shall not be processed before. Is this necessary?
            bool is_found = history.find(x) != history.end(); if (is_found) continue; history.insert(x);
            answer++;
        }
        return answer;
    }
};


class Solution {
private:
    const int MAX_DIGITS = 16;
    std::vector<long long> power_of_nine;
    std::vector<long long> initial_counters;
public:
    Solution(): power_of_nine(MAX_DIGITS), initial_counters(MAX_DIGITS) {
        // Create lookup table
        power_of_nine.at(0) = 1;
        for (int i = 1; i < MAX_DIGITS; i++) {
            power_of_nine.at(i) = power_of_nine.at(i - 1) * 9;
        }
        initial_counters.at(1) = 9;
        for (int i = 2; i < MAX_DIGITS; i++) {
            initial_counters.at(i) = initial_counters.at(i - 1) + (9 * power_of_nine.at(i - 1));
        }
    }

    long long countDistinct(long long n) const {
        std::string s = std::to_string(n);
        long long answer = initial_counters.at(s.size() - 1);
        for (int i = 0; i < s.size(); i++) {
            int digit = s.at(i) - '0';
            if (digit == 0) return answer;
            int pos = s.size() - i - 1;
            answer += power_of_nine.at(pos) * (digit - 1);
        }
        answer++;
        return answer;
    }
};


void test() {
    // Power of Nine                                        729               81             9    1
    // Initial Counters 10000,1000,100,10      7380=819+(729*9)    819=90+(81*9)    90=9+(9*9)    9
    SolutionBruteforce solution_bruteforce = SolutionBruteforce();
    Solution solution_efficient = Solution();

    assert(solution_bruteforce.countDistinct(9) == 9);
    assert(solution_bruteforce.countDistinct(10) == 9);
    assert(solution_bruteforce.countDistinct(20) == 18);
    assert(solution_bruteforce.countDistinct(100) == 90);
    assert(solution_bruteforce.countDistinct(110) == 90);
    assert(solution_bruteforce.countDistinct(120) == 99);
    assert(solution_bruteforce.countDistinct(200) == 171);           // 90 + 81
    assert(solution_bruteforce.countDistinct(300) == 252);           // 171 + 81
    assert(solution_bruteforce.countDistinct(400) == 333);           // 252 + 81
    assert(solution_bruteforce.countDistinct(1000) == 819);          // 819 + 729
    assert(solution_bruteforce.countDistinct(2000) == 1548);         // 1548 + 729
    assert(solution_bruteforce.countDistinct(2100) == 1548);
    assert(solution_bruteforce.countDistinct(2200) == 1629);         // 1548 + 81
    assert(solution_bruteforce.countDistinct(3000) == 2277);
    assert(solution_bruteforce.countDistinct(10'000) == 7380);
    assert(solution_bruteforce.countDistinct(100'000) == 66429);
    assert(solution_bruteforce.countDistinct(1'000'000) == 597870);
    assert(solution_efficient.countDistinct(2345) == 1742);          // 819 + 1 * 729 + 2 * 81 + 3 * 9 + 5
    assert(solution_efficient.countDistinct(3000) == 2277);
    assert(solution_efficient.countDistinct(2200) == 1629);
    assert(solution_efficient.countDistinct(5432) == 3998);
    assert(solution_efficient.countDistinct(406) == 333);            // 90 + 3 * 81
    assert(solution_efficient.countDistinct(8086) == 5922);          // 819 + 7 * 729 + 0 + 7 * 9 + 6
    assert(solution_efficient.countDistinct(1'000'000'000'000'000) == 231'627'523'606'479);

    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);             // [0, 1)
        for (int t = 0; t < 1000; t++) {
            long long n = int(rnd(mt) * 10000) + 1;
            long long answer_bruteforce = solution_bruteforce.countDistinct(n);
            long long answer_efficient = solution_efficient.countDistinct(n);
            assert(answer_efficient == answer_bruteforce);
        }
    }
}


int main() {
    test();
    return 0;
}
