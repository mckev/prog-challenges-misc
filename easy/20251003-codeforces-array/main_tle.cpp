// https://codeforces.com/contest/57/problem/C

#include <iostream>
#include <vector>


long long solve(int n) {
    std::vector<long long> arr(n);
    // Calculate no less than the preciding one
    //             1
    //           2   1
    //         6   3   1
    //       20  10  4   1
    //     70  35  15  5   1
    // We reverse each row to make it easier to calculate arr[]
    long long answer = 0;
    for (int i = 0; i <= n; i++) {
        arr.at(0) = 1;
        for (int j = 1; j < i - 1; j++) {
            arr.at(j) = (arr.at(j - 1) + arr.at(j)) % 1000000007;
        }
        if (i - 2 >= 0) {
            arr.at(i - 1) = (arr.at(i - 2) * 2) % 1000000007;
        }
    }
    for (int i = 0; i < n; i++) {
        answer = (answer + arr.at(i)) % 1000000007;
    }
    // Calculate no more and no less than the preciding one
    answer = (answer * 2 - n) % 1000000007;
    return answer;
}


int main() {
    int n; std::cin >> n;
    long long answer = solve(n);
    std::cout << answer << std::endl;
    return 0;
}
