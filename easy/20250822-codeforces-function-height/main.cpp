#include <iostream>


int main() {
    long long n; std::cin >> n;
    long long k; std::cin >> k;
    long long answer = (k + n - 1) / n;         // ceil(k / n);
    std::cout << answer << std::endl;
    return 0;
}
