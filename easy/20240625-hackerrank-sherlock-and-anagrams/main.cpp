#include <cassert>
#include <iostream>
#include <map>
#include <string>


std::map<char, int> calcFreq(const std::string& s, int i, int len) {
    std::map<char, int> freq;
    for (int l = 0; l < len; l++) {
        freq[s[i + l]]++;
    }
    return freq;
}

int sherlockAndAnagrams(const std::string& s) {
    int count = 0;
    for (int len = 1; len <= s.size(); len++) {
        for (int i = 0; i + len <= s.size(); i++) {
            std::map<char, int> freq1 = calcFreq(s, i, len);
            for (int j = i + 1; j + len <= s.size(); j++) {
                if (freq1 == calcFreq(s, j, len)) {
                    count++;
                }
            }
        }
    }
    return count;
}

int main() {
    // assert(sherlockAndAnagrams("mom") == 2);
    // assert(sherlockAndAnagrams("abba") == 4);
    // assert(sherlockAndAnagrams("abcd") == 0);
    // assert(sherlockAndAnagrams("ifailuhkqq") == 3);
    // assert(sherlockAndAnagrams("kkkk") == 10);
    // assert(sherlockAndAnagrams("cdcd") == 5);

    int t;
    std::cin >> t;
    while (t--) {
        std::string s;
        std::cin >> s;
        std::cout << sherlockAndAnagrams(s) << std::endl;
    }
    return 0;
}
