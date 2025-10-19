// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/A1

#include <cassert>
#include <iostream>
#include <vector>


int solve(const std::vector<int>& A) {
    int answer = 0;
    int N = A.size(); assert (N >= 1);
    for (int i = 1; i < N; i++) {
        answer = std::max(answer, std::abs(A.at(i) - A.at(i - 1)));
    }
    return answer;
}


int main() {
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
