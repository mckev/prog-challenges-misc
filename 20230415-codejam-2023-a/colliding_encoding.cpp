#include <iostream>
#include <map>
#include <set>


int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        std::map<char, std::string> charmap;
        for (int n = 0; n < 26; n++) {
            char ch = 'A' + n;
            std::cin >> charmap[ch];
        }

        std::set<std::string> encoded_history;
        bool is_collision = false;
        int N; std::cin >> N;
        for (int n = 0; n < N; n++) {
            std::string word; std::cin >> word;
            std::string encoded = "";
            for (char ch : word) {
                encoded += charmap[ch];
            }
            bool is_found = encoded_history.find(encoded) != encoded_history.end();
            if (is_found) {
                is_collision = true;
            }
            encoded_history.insert(encoded);
        }
        std::cout << "Case #" << t + 1 << ": " << (is_collision ? "YES" : "NO") << std::endl;
    }
    return 0;
}
