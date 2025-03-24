// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/D

#include <cassert>
#include <iostream>
#include <vector>


class Solution {
public:
    Solution(const std::string& E) {
        this->E = E;
        // Note down location of all question marks
        for (int pos = 0; pos < E.size(); pos++) {
            if (E.at(pos) == '?') {
                questionmark_pos_indexes.push_back(pos);
            }
        }
        // Reset
        for (int questionmark_pos_index = questionmark_pos_indexes.size() - 1; questionmark_pos_index >= 0; questionmark_pos_index--) {
            reset_char(questionmark_pos_index);
        }
    }

    std::string output() const {
        return E;
    }

    void decrease_k_times(int K) {
        for (int k = 0; k < K - 1; k++) {
            decrease_char(questionmark_pos_indexes.size() - 1);
        }
    }

    long long total_combinations() const {
        std::vector<long long> combinations(E.size());
        if (E.at(E.size() - 1) == '0') {
            combinations.at(E.size() - 1) = 0;
        } else {
            combinations.at(E.size() - 1) = 1;
        }
        for (int pos = E.size() - 2; pos >= 0; pos--) {
            char ch = E.at(pos);
            switch (ch) {
            case '1':
                combinations.at(pos) = combinations.at(pos + 1) + (pos + 2 <= E.size() - 1 ? combinations.at(pos + 2) : 1);
                break;
            case '2':
                if (E.at(pos + 1) == '0' || E.at(pos + 1) == '1' || E.at(pos + 1) == '2' || E.at(pos + 1) == '3' || E.at(pos + 1) == '4' || E.at(pos + 1) == '5' || E.at(pos + 1) == '6') {
                    combinations.at(pos) = combinations.at(pos + 1) + (pos + 2 <= E.size() - 1 ? combinations.at(pos + 2) : 1);
                } else {
                    combinations.at(pos) = combinations.at(pos + 1);
                }
                break;
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                combinations.at(pos) = combinations.at(pos + 1);
                break;
            case '0':
                combinations.at(pos) = 0;
                break;
            case '?':
                assert(false);
            default:
                assert(false);
            }
            combinations.at(pos) = combinations.at(pos) % 998244353;
        }
        return combinations.at(0);
    }

private:
    std::string E;
    std::vector<int> questionmark_pos_indexes;

    void reset_char(int questionmark_pos_index) {
        int pos = questionmark_pos_indexes.at(questionmark_pos_index);
        if (pos == E.size() - 1 || (pos + 2 <= E.size() - 1 && E.at(pos + 2) == '0') ) {
            if (questionmark_pos_index - 1 >= 0 && questionmark_pos_indexes.at(questionmark_pos_index - 1) == pos - 1 && E.at(pos - 1) == '2') {
                // Case when E[pos - 1] is '?' and it is set to '2'. Then we should reset E[pos] to '9' and E[pos - 1] to '1'
                E.at(pos) = '9';
            } else if (questionmark_pos_index - 1 >= 0 && questionmark_pos_indexes.at(questionmark_pos_index - 1) == pos - 1 && E.at(pos - 1) == '1') {
                // Case when E[pos - 1] is '?' and it is set to '1'. Then we should reset E[pos] to '6' and E[pos - 1] to '2'
                E.at(pos) = '6';
            } else if (pos - 1 >= 0 && E.at(pos - 1) == '2') {
                E.at(pos) = '6';
            } else if (pos - 1 >= 0 && E.at(pos - 1) == '?') {
                E.at(pos) = '6';
            } else {
                E.at(pos) = '9';
            }
        } else {
            if (pos + 1 <= E.size() - 1 && E.at(pos + 1) == '9') {
                E.at(pos) = '1';
            } else if (pos + 1 <= E.size() - 1 && E.at(pos + 1) == '8') {
                E.at(pos) = '1';
            } else if (pos + 1 <= E.size() - 1 && E.at(pos + 1) == '7') {
                E.at(pos) = '1';
            } else {
                E.at(pos) = '2';
            }
        }
    }

    void decrease_char(int questionmark_pos_index) {
        int pos = questionmark_pos_indexes.at(questionmark_pos_index);
        switch (E.at(pos)) {
            case '9':
                E.at(pos) = '8';
                break;
            case '8':
                E.at(pos) = '7';
                break;
            case '7':
                E.at(pos) = '6';
                break;
            case '6':
                E.at(pos) = '5';
                break;
            case '5':
                E.at(pos) = '4';
                break;
            case '4':
                E.at(pos) = '3';
                break;
            case '3':
                E.at(pos) = '2';
                break;
            case '2':
                E.at(pos) = '1';
                break;
            case '1':
                reset_char(questionmark_pos_index);
                decrease_char(questionmark_pos_index - 1);
                break;
            case '0':
                assert(false);
            case '?':
                assert(false);
            default:
                assert(false);
        }
    }
};


void test() {
    {
        // Case 2
        Solution solution = Solution("135201");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(1);
        assert(solution.output() == "135201");
        assert(total_combinations == 2);
    }
    {
        // Case 4
        Solution solution = Solution("1?0");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(2);
        assert(solution.output() == "110");
        assert(total_combinations == 1);
    }
    {
        // Case 5
        Solution solution = Solution("1122");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(1);
        assert(solution.output() == "1122");
        assert(total_combinations == 5);
    }
    {
        // Case 6
        Solution solution = Solution("3???????????????????3");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(1337);
        assert(solution.output() == "322222222121221112223");
        assert(total_combinations == 10946);
    }
    {
        // Case 10
        Solution solution = Solution("????????????????????????????????????????????123");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(100000);
        assert(solution.output() == "22222222222222222222222222211222211212211111123");
        assert(total_combinations == 814549564);
    }
    {
        // Case 13
        Solution solution = Solution("2??");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(8);
        assert(solution.output() == "218");
        assert(total_combinations == 3);
    }
    {
        // Case 14
        Solution solution = Solution("?");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(3);
        assert(solution.output() == "7");
        assert(total_combinations == 1);
    }
    {
        // Case 26
        Solution solution = Solution("?9");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(1);
        // 19
        assert(solution.output() == "19");
        assert(total_combinations == 2);
    }
    {
        // Case 27
        Solution solution = Solution("?10??");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(17);
        // 91026
        // 91025
        // 91024
        // 91023
        // 91022
        // 91021
        // 91019
        // 91018
        // 91017
        // 91016
        // 91015
        // 91014
        // 91013
        // 91012
        // 91011
        // 81026
        // 81025
        assert(solution.output() == "81025");
        assert(total_combinations == 2);
    }
    {
        // Case 28
        Solution solution = Solution("???20");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(18);
        // 22620
        // 22520
        // 22420
        // 22320
        // 22220
        // 22120
        // 21920
        // 21820
        // 21720
        // 21620
        // 21520
        // 21420
        // 21320
        // 21220
        // 21120
        // 12620
        // 12520
        // 12420
        assert(solution.output() == "12420");
        assert(total_combinations == 3);
    }
    {
        // Case 38
        Solution solution = Solution("7?9??195??6?28");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(32);
        assert(solution.output() == "71911195116128");
        assert(total_combinations == 60);
    }
    {
        // Case 39
        Solution solution = Solution("??53?7289?6??2??0????7117??3");
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(144);
        assert(solution.output() == "2253172892622242021117117113");
        assert(total_combinations == 4320);
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        std::string E; std::cin >> E;
        int K; std::cin >> K;
        Solution solution = Solution(E);
        long long total_combinations = solution.total_combinations();
        solution.decrease_k_times(K);
        std::cout << "Case #" << t + 1 << ": " << solution.output() << " " << total_combinations << std::endl;
    }
    return 0;
}
