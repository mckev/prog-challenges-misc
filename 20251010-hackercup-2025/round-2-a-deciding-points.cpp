// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-2/problems/A

#include <cassert>
#include <iostream>


bool solve(int N, int M) {
    // Game can end if N >= M
    if (N < M) return false;
    // However in case of deuce (at 2 * (M - 1)) then N cannot be odd
    if (N <= 2 * (M - 1)) return true;
    return N % 2 == 0;
}


void test() {
    assert(solve(4, 3) == true);
    assert(solve(5, 3) == false);
    assert(solve(3, 3) == true);
    assert(solve(6, 3) == true);
    assert(solve(1, 1) == false);
    assert(solve(138, 6) == true);
    // Other cases
    assert(solve(97, 99) == false);
    assert(solve(997184809, 100) == false);
    assert(solve(524548163, 21) == false);
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int M; std::cin >> M;
        bool answer = solve(N, M);
        if (answer) {
            std::cout << "Case #" << t + 1 << ": YES" << std::endl;
        } else {
            std::cout << "Case #" << t + 1 << ": NO" << std::endl;
        }
    }
    return 0;
}
