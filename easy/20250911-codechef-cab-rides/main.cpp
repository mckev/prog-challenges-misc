// https://www.codechef.com/problems/CABRIDE

#include <algorithm>
#include <iostream>


int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int answer = std::max(200, N * 100);
        std::cout << answer << std::endl;
    }
    return 0;
}
