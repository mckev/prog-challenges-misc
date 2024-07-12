#include <iostream>

bool solution(int S, int D, int K) {
    int buns = S * 2 + D * 2;
    int patties = S + D * 2;
    if (buns > K && patties >= K) {
        return true;
    } else {
        return false;
    }
}

int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int S, D, K; std::cin >> S >> D >> K;
        bool answer = solution(S, D, K);
        std::cout << "Case #" << t + 1 << ": " << (answer ? "YES" : "NO") << std::endl;
    }
    return 0;
}
