// https://leetcode.com/problems/repeated-dna-sequences/

#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>


class Solution {
public:
    std::vector<std::string> findRepeatedDnaSequences(const std::string& s) {
        std::unordered_map<std::string, int> counter; for (int i = 0; i < (int) s.size() - 10 + 1; i++) counter[s.substr(i, 10)] += 1;
        std::vector<std::string> answers; for (const auto& [k, v] : counter) { if (v > 1) answers.push_back(k); }
        return answers;
    }
};


int main() {
    {
        Solution solution = Solution();
        std::vector<std::string> answer = solution.findRepeatedDnaSequences("AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT");
        std::vector<std::string> expected = {"AAAAACCCCC", "CCCCCAAAAA"};
        assert(answer == expected);
    }
    {
        Solution solution = Solution();
        std::vector<std::string> answer = solution.findRepeatedDnaSequences("AAAAAAAAAAAAA");
        std::vector<std::string> expected = {"AAAAAAAAAA"};
        assert(answer == expected);
    }
    {
        Solution solution = Solution();
        std::vector<std::string> answer = solution.findRepeatedDnaSequences("AAAAAAAAAAA");
        std::vector<std::string> expected = {"AAAAAAAAAA"};
        assert(answer == expected);
    }
    return 0;
}
