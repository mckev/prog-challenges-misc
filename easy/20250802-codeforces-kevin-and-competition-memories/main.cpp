// https://codeforces.com/contest/2048/problem/D

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#pragma GCC optimize "trapv"                                        // Detect overflow


class BruteforceSolution {
private:
    int count_less_or_equal(const std::vector<int>& vec, int r) const {
        int counter = 0;
        for (int val : vec) {
            if (val <= r) counter++;
        }
        return counter;
    }

    int calculate_kevin_rank(const std::vector<int>& participant_ratings, const std::vector<int>& sub_problem_ratings) const {
        int n = participant_ratings.size();
        std::vector<int> number_of_problems_solved_per_participant(n);
        for (int i = 0; i < n; i++) {
            number_of_problems_solved_per_participant.at(i) = count_less_or_equal(sub_problem_ratings, participant_ratings.at(i));
        }
        // Kevin is participant[0]
        // "Kevin's rank is one plus the number of participants who solve more problems than he does"
        int rank = 1;
        for (int i = 1; i < n; i++) {
            if (number_of_problems_solved_per_participant.at(i) > number_of_problems_solved_per_participant.at(0)) {
                rank++;
            }
        }
        return rank;
    }

    std::vector<int> custom_sort(const std::vector<int>& vec, int r) const {
        // Separate elements into two buckets
        std::vector<int> less_or_equal;
        std::vector<int> greater;
        for (int val : vec) {
            if (val <= r) {
                less_or_equal.push_back(val);
            } else {
                greater.push_back(val);
            }
        }
        // Sort less_or_equal ascending
        std::sort(less_or_equal.begin(), less_or_equal.end());
        // Sort greater descending
        std::sort(greater.begin(), greater.end(), std::greater<int>());
        // Combine results
        less_or_equal.insert(less_or_equal.end(), greater.begin(), greater.end());
        return less_or_equal;
    }

    std::vector<int> get_sub_elements(const std::vector<int>& v, int start, int count) const {
        if (start >= (int) v.size()) return {};
        int end = std::min(start + count, (int) v.size());
        return std::vector<int>(v.begin() + start, v.begin() + end);
    }

public:
    std::vector<long long> solve(const std::vector<int>& participant_ratings, const std::vector<int>& problem_ratings) const {
        std::vector<long long> answers;
        std::vector<int> problem_ratings_arranged = custom_sort(problem_ratings, participant_ratings.at(0));
        int m = problem_ratings.size();
        for (int k = 1; k <= m; k++) {
            long long sum_ranks = 0;
            for (int start = 0; start < problem_ratings_arranged.size(); start += k) {
                std::vector<int> sub_problem_ratings = get_sub_elements(problem_ratings_arranged, start, k);
                if (sub_problem_ratings.size() < k) {
                    // Case we cannot get full "k" problems (could happen at the end)
                    break;
                }
                int rank = calculate_kevin_rank(participant_ratings, sub_problem_ratings);
                sum_ranks += rank;
            }
            answers.push_back(sum_ranks);
        }
        assert(answers.size() == m);
        return answers;
    }
};


class Solution {
private:
    int count_greater_equal(const std::vector<int>& vec, int r) const {
        // Note: vec must be sorted ascending
        auto it = std::lower_bound(vec.begin(), vec.end(), r);
        return std::distance(it, vec.end());
    }

    std::vector<int> custom_sort(const std::vector<int>& vec, int r) const {
        // Separate elements into two buckets
        std::vector<int> less_or_equal;
        std::vector<int> greater;
        for (int val : vec) {
            if (val <= r) {
                less_or_equal.push_back(val);
            } else {
                greater.push_back(val);
            }
        }
        // Sort less_or_equal ascending
        std::sort(less_or_equal.begin(), less_or_equal.end());
        // Sort greater descending
        std::sort(greater.begin(), greater.end(), std::greater<int>());
        // Combine results
        less_or_equal.insert(less_or_equal.end(), greater.begin(), greater.end());
        return less_or_equal;
    }

public:
    std::vector<long long> solve(const std::vector<int>& participant_ratings_all, const std::vector<int>& problem_ratings) const {
        // Eliminate participant ratings <= kevin (index 0)
        std::vector<int> participant_ratings;
        int kevin_rating = participant_ratings_all.at(0);
        for (int participant_rating : participant_ratings_all) {
            if (participant_rating > kevin_rating) {
                participant_ratings.push_back(participant_rating);
            }
        }
        std::sort(participant_ratings.begin(), participant_ratings.end());

        // Arrange problem ratings so it's sorted ascending for elements <= kevin, then followed by sorted descending
        std::vector<int> problem_ratings_arranged = custom_sort(problem_ratings, kevin_rating);

        std::vector<long long> answers;
        for (int k = 1; k <= problem_ratings.size(); k++) {
            long long sum_ranks = 0;
            for (int start = 0; start < problem_ratings_arranged.size(); start += k) {
                // Generate competition [start, end]
                int end = start + k - 1;
                if (end >= problem_ratings_arranged.size()) {
                    // Case we cannot get full "k" problems (could happen at the end)
                    break;
                }
                int hardest_problem_rating = problem_ratings_arranged.at(end);
                int rank;
                if (hardest_problem_rating <= kevin_rating) {
                    rank = 1;
                } else {
                    rank = count_greater_equal(participant_ratings, hardest_problem_rating) + 1;
                }
                sum_ranks += rank;
            }
            answers.push_back(sum_ranks);
        }
        assert(answers.size() == problem_ratings.size());
        return answers;
    }
};


void test() {
    {
        std::vector<int> participant_ratings = {4, 3, 7, 5};
        std::vector<int> problem_ratings = {2, 5, 4, 6};
        Solution solution = Solution();
        std::vector<long long> answers1 = solution.solve(participant_ratings, problem_ratings);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<long long> answers2 = bruteforce_solution.solve(participant_ratings, problem_ratings);
        assert(answers1 == (std::vector<long long>{7, 4, 2, 3}));
        assert(answers2 == answers1);
    }
    {
        std::vector<int> participant_ratings = {5, 0, 4, 8, 6};
        std::vector<int> problem_ratings = {1, 3, 9, 2, 7};
        Solution solution = Solution();
        std::vector<long long> answers1 = solution.solve(participant_ratings, problem_ratings);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<long long> answers2 = bruteforce_solution.solve(participant_ratings, problem_ratings);
        assert(answers1 == (std::vector<long long>{6, 2, 1, 1, 2}));
        assert(answers2 == answers1);
    }
    {
        std::vector<int> participant_ratings = {1, 1, 4, 5, 1, 4};
        std::vector<int> problem_ratings = {1, 9, 1, 9, 8, 1, 0};
        Solution solution = Solution();
        std::vector<long long> answers1 = solution.solve(participant_ratings, problem_ratings);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<long long> answers2 = bruteforce_solution.solve(participant_ratings, problem_ratings);
        assert(answers1 == (std::vector<long long>{7, 3, 2, 1, 1, 1, 1}));
        assert(answers2 == answers1);
    }
    {
        std::vector<int> participant_ratings = {1, 9, 1, 9, 8, 1, 0};
        std::vector<int> problem_ratings = {1, 1, 4, 5, 1, 4};
        Solution solution = Solution();
        std::vector<long long> answers1 = solution.solve(participant_ratings, problem_ratings);
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::vector<long long> answers2 = bruteforce_solution.solve(participant_ratings, problem_ratings);
        assert(answers1 == (std::vector<long long>{15, 9, 5, 4, 4, 4}));
        assert(answers2 == answers1);
    }
    {
        Solution solution = Solution();
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 500; T++) {
            int N = int(rnd(mt) * 200) + 1;                             // [1 - 20]
            std::vector<int> participant_ratings;
            for (int n = 0; n < N; n++) {
                int participant_rating = int(rnd(mt) * 1000000001);     // [0 - 1000000000]
                participant_ratings.push_back(participant_rating);
            }
            int M = int(rnd(mt) * 200) + 1;                             // [1 - 20]
            std::vector<int> problem_ratings;
            for (int m = 0; m < M; m++) {
                int problem_rating = int(rnd(mt) * 1000000001);         // [0 - 1000000000]
                problem_ratings.push_back(problem_rating);
            }
            std::vector<long long> answers1 = solution.solve(participant_ratings, problem_ratings);
            std::vector<long long> answers2 = bruteforce_solution.solve(participant_ratings, problem_ratings);
            assert(answers2 == answers1);
        }
    }
    {
        Solution solution = Solution();
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 10; T++) {
            int N = int(rnd(mt) * 300000) + 1;                          // [1 - 300000]
            std::vector<int> participant_ratings;
            for (int n = 0; n < N; n++) {
                int participant_rating = int(rnd(mt) * 1000000001);     // [0 - 1000000000]
                participant_ratings.push_back(participant_rating);
            }
            int M = int(rnd(mt) * 300000) + 1;                          // [1 - 300000]
            std::vector<int> problem_ratings;
            for (int m = 0; m < M; m++) {
                int problem_rating = int(rnd(mt) * 1000000001);         // [0 - 1000000000]
                problem_ratings.push_back(problem_rating);
            }
            std::vector<long long> answers1 = solution.solve(participant_ratings, problem_ratings);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int M; std::cin >> M;
        std::vector<int> participant_ratings;
        for (int n = 0; n < N; n++) {
            int participant_rating; std::cin >> participant_rating;
            participant_ratings.push_back(participant_rating);
        }
        std::vector<int> problem_ratings;
        for (int m = 0; m < M; m++) {
            int problem_rating; std::cin >> problem_rating;
            problem_ratings.push_back(problem_rating);
        }
        Solution solution = Solution();
        std::vector<long long> answers = solution.solve(participant_ratings, problem_ratings);
        for (long long answer : answers) {
            std::cout << answer << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
