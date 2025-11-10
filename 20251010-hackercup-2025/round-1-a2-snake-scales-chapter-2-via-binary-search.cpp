// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/A2

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <vector>


bool is_solved(const std::vector<int>& A, int ladder) {
    std::vector<bool> is_reached(A.size());
    bool is_on_platform = false;
    for (int i = 0; i < A.size(); i++) {
        if (A.at(i) <= ladder) {
            is_on_platform = true;
            is_reached.at(i) = true;
        } else if (is_on_platform && (i - 1 >= 0) && std::abs(A.at(i) - A.at(i - 1)) <= ladder) {
            is_reached.at(i) = true;
        } else {
            is_on_platform = false;
        }
    }
    for (int i = A.size() - 1; i >= 0; i--) {
        if (A.at(i) <= ladder) {
            is_on_platform = true;
            is_reached.at(i) = true;
        } else if (is_on_platform && (i + 1 < A.size()) && std::abs(A.at(i) - A.at(i + 1)) <= ladder) {
            is_reached.at(i) = true;
        } else {
            is_on_platform = false;
        }
    }
    return std::all_of(is_reached.begin(), is_reached.end(), [](bool v) { return v; });
}


int solve(const std::vector<int>& A) {
    int high = 1'000'000'000;
    int low = 1;
    while (low <= high) {
        int mid = (high + low) / 2;
        if (is_solved(A, mid)) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return low;
}


void test() {
    {
        std::vector<int> A = { 2, 4, 5, 1, 4 };
        int answer = solve(A);
        assert(answer == 3);
    }
    {
        std::vector<int> A = { 13, 10, 11 };
        int answer = solve(A);
        assert(answer == 10);
    }
    {
        std::vector<int> A = { 1, 3, 3, 7 };
        int answer = solve(A);
        assert(answer == 4);
    }
    {
        std::vector<int> A = { 42 };
        int answer = solve(A);
        assert(answer == 42);
    }
    {
        std::vector<int> A = { 5, 50, 42 };
        int answer = solve(A);
        assert(answer == 42);
    }
    {
        std::vector<int> A = { 4, 2, 5, 6, 4, 2, 1 };
        int answer = solve(A);
        assert(answer == 2);
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<int> A;
        for (int n = 0; n < N; n++) {
            int a; std::cin >> a;
            A.push_back(a);
        }
        int answer = solve(A);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
