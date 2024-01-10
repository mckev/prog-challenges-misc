// https://leetcode.com/problems/check-if-one-string-swap-can-make-strings-equal

class Solution {
public:
    bool areAlmostEqual(string s1, string s2) {
        std::vector<int> mismatched_pos;
        for (int i = 0; i < s1.size(); i++) {
            if (s1[i] == s2[i]) continue;
            mismatched_pos.push_back(i);
        }
        if (mismatched_pos.size() == 0) return true;
        if (mismatched_pos.size() != 2) return false;
        return s1[mismatched_pos[0]] == s2[mismatched_pos[1]] && s1[mismatched_pos[1]] == s2[mismatched_pos[0]];
    }
};
