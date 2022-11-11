// https://codingcompetitions.withgoogle.com/codejam/round/0000000000877ba5/0000000000aa8e9c

#include <iostream>

std::string solve(const std::string& S) {
    std::string output;
    std::string pocket;
    for (int i = 0; i < S.size(); i++) {
        output += S[i];
        if (i + 1 >= S.size()) continue;
        if (S[i + 1] < S[i]) {
            // Case descending : Do not repeat
            pocket = "";
        } else if (S[i + 1] > S[i]) {
            // Case ascending : Repeat
            output += pocket + S[i];
            pocket = "";
        } else if (S[i + 1] == S[i]) {
            pocket += S[i];
        }
    }
    return output;
}

int main() {
    int T;
    std::cin >> T;
    for (int t = 0; t < T; t++) {
        std::string S; std::cin >> S;
        std::cout << "Case #" << t + 1 << ": " << solve(S) << std::endl;
    }
    // std::cout << solve("PEEL") << std::endl;
    // std::cout << solve("AAAAAAAAAA") << std::endl;
    // std::cout << solve("CODEJAMDAY") << std::endl;
    // std::cout << solve("HELLOWORLD") << std::endl;
    return 0;
}
