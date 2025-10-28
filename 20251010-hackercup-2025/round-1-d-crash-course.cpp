// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/D

#include <cassert>
#include <iostream>
#include <vector>


std::string solve(const std::string& S) {
    int counter = 0;
    for (int i = S.size() - 1; i >=0; ) {
        // Process Bob
        for (; i >= 0 && S.at(i) == 'B'; i--) counter--;
        // Process Alice
        for (; i >= 0 && S.at(i) == 'A'; i--) counter++;
        if (counter > 0) {
            return "Alice";
        }
    }
    if (counter == 0) {
        // Special case when at the end, Alice got same number of items as Bob. It means counter is never positive.
        // So Alive shall not sacrifice any item to Bob.
        // However, since Alice moves first, then Bob have more items to pick and win.
        return "Bob";
    }
    return "Bob";
}


void test() {
    {
        std::string S = "ABBAAAB";
        std::string answer = solve(S);
        assert(answer == "Alice");
    }
    {
        std::string S = "A";
        std::string answer = solve(S);
        assert(answer == "Alice");
    }
    {
        std::string S = "B";
        std::string answer = solve(S);
        assert(answer == "Bob");
    }
    {
        std::string S = "AB";
        std::string answer = solve(S);
        assert(answer == "Bob");
    }
    {
        std::string S = "AAAAAA";
        std::string answer = solve(S);
        assert(answer == "Alice");
    }
    {
        std::string S = "BBBBBBA";
        std::string answer = solve(S);
        assert(answer == "Alice");
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::string S; std::cin >> S;
        std::string answer = solve(S);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
