// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/A

#include <iostream>
#include <vector>


const char UNDEFINED = ' ';
const char ALLOWED = 'Y';
const char NOT_ALLOWED = 'N';


std::vector<std::string> solve(int N, const std::string& inbound, const std::string& outbound) {
    std::vector<std::string> answer(N, std::string(N, UNDEFINED));      // Create N x N matrix
    for (int country = 0; country < N; country++) {
        answer[country][country] = ALLOWED;                             // Always allow from own country
        // Traverse from the country to the right side
        char state = ALLOWED;
        for (int n = country + 1; n < N; n++) {
            int prev_country = n - 1;
            if (outbound[prev_country] == NOT_ALLOWED) state = NOT_ALLOWED;
            if (inbound[n] == NOT_ALLOWED) state = NOT_ALLOWED;
            answer[country][n] = state;
        }
        // Traverse from the country to the left side
        state = ALLOWED;
        for (int n = country - 1; n >= 0; n--) {
            int prev_country = n + 1;
            if (outbound[prev_country] == NOT_ALLOWED) state = NOT_ALLOWED;
            if (inbound[n] == NOT_ALLOWED) state = NOT_ALLOWED;
            answer[country][n] = state;
        }
    }
    return answer;
}


int main() {
    // {
    //     int N = 5;
    //     std::string inbound = "YNNYY";
    //     std::string outbound = "YYYNY";
    //     solve(N, inbound, outbound);
    // }
    // {
    //     int N = 10;
    //     std::string inbound = "NYYYNNYYYY";
    //     std::string outbound = "YYNYYNYYNY";
    //     solve(N, inbound, outbound);
    // }
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::string inbound; std::cin >> inbound;
        std::string outbound; std::cin >> outbound;
        std::vector<std::string> answer = solve(N, inbound, outbound);
        std::cout << "Case #" << t + 1 << ":" << std::endl;
        for (const std::string& answer_each : answer) {
            std::cout << answer_each << std::endl;
        }
    }
    return 0;
}
