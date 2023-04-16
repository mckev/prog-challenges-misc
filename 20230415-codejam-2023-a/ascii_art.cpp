// https://codingcompetitions.withgoogle.com/codejam/round/0000000000c95b94/0000000000cad9c2

#include <iostream>


const int CHARSET = 26;

int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        long long n; std::cin >> n;
        int repeat = 1;
        while (n > CHARSET * repeat) {
            n -= CHARSET * repeat;
            repeat++;
        }
        int ch = 'A' + (n - 1) / repeat;
        std::cout << "Case #" << t + 1 << ": " << char(ch) << std::endl;
    }
    return 0;
}
