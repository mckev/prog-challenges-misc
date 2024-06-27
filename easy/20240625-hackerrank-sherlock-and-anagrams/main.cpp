// https://www.hackerrank.com/challenges/sherlock-and-anagrams/problem

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>


int sherlockAndAnagrams(const std::string& s) {
    int count = 0;
    std::vector<std::map<char, int>> freq(s.size(), std::map<char,int>());
    for (int len = 0; len < s.size(); len++) {
        for (int i = 0; i + len < s.size(); i++) {
            char ch = s[i + len];
            freq[i][ch]++;
        }
        for (int i = 0; i + len < s.size(); i++) {
            for (int j = i + 1; j + len < s.size(); j++) {
                if (freq[i] == freq[j]) count++;
            }
        }
    }
    return count;
}

int main() {
    assert(sherlockAndAnagrams("mom") == 2);
    assert(sherlockAndAnagrams("abba") == 4);
    assert(sherlockAndAnagrams("abcd") == 0);
    assert(sherlockAndAnagrams("ifailuhkqq") == 3);
    assert(sherlockAndAnagrams("kkkk") == 10);
    assert(sherlockAndAnagrams("cdcd") == 5);

    int t;
    std::cin >> t;
    while (t--) {
        std::string s;
        std::cin >> s;
        std::cout << sherlockAndAnagrams(s) << std::endl;
    }

    return 0;
}
