// https://codeforces.com/problemset/problem/1183/C

#include <cassert>
#include <cmath>
#include <iostream>


int solve(int k, int n, int a, int b) {
    int answer;

    assert(b < a);

    // x  <  (k - b * n) / (a - b)
    double x = ((double) k - (double) b * n) / ((double) a - b);
    if (x == int(x)) {
        answer = int(x - 1);
    } else {
        answer = int(std::floor(x));
    }

    // Case negative means we cannot satisfy
    if (answer < 0) return -1;

    // Case we can play more than required n turns
    if (answer > n) return n;

    return answer;
}


void test() {
    // Case #1
    assert(solve(15, 5, 3, 2) == 4);
    assert(solve(15, 5, 4, 3) == -1);
    assert(solve(15, 5, 2, 1) == 5);
    assert(solve(16, 7, 5, 2) == 0);
    assert(solve(20, 5, 7, 3) == 1);

    // Case #3
    assert(solve(996876820, 939229, 661108, 2517) == -1);
}


int main() {
    test();
    int Q; std::cin >> Q;
    for (int q = 0; q < Q; q++) {
        int k; std::cin >> k;
        int n; std::cin >> n;
        int a; std::cin >> a;
        int b; std::cin >> b;
        int answer = solve(k, n, a, b);
        std::cout << answer << std::endl;
    }
    return 0;
}
