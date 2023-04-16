// https://codingcompetitions.withgoogle.com/codejam/round/0000000000c95b94/0000000000cada38

#include <iostream>
#include <map>
#include <vector>


int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<int> cards;
        for (int n = 0; n < N; n++) {
            int card; std::cin >> card;
            cards.push_back(card);
        }
        // Construct color map
        std::map<int, int> colormap;
        int order = 0;
        for (int card : cards) {
            bool is_exist = colormap.find(card) != colormap.end();
            if (! is_exist) {
                colormap[card] = order;
                order++;
            }
        }
        // Generate answer
        std::vector<int> answers;
        int last_order = -1;
        int last_card = -1;
        for (int card : cards) {
            int order = colormap[card];
            if (order < last_order) {
                answers = {};
                break;
            }
            last_order = order;
            if (card != last_card) {
                answers.push_back(card);
            }
            last_card = card;
        }
        if (answers.size() == 0) {
            std::cout << "Case #" << t + 1 << ": IMPOSSIBLE" << std::endl;
        } else {
            std::cout << "Case #" << t + 1 << ":";
            for (int answer : answers) {
                std::cout << " " << answer;
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
