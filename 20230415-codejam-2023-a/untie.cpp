#include <iostream>
#include <vector>


bool is_replace_first_card(const std::string& original_cards) {
    std::string cards = original_cards;
    for (int i = 1; i < cards.size(); i++) {
        char card = cards[i];
        char last_card = cards[i - 1];
        if (card == last_card) {
            cards[i] = 'X';
        }
    }
    if (cards.size() > 1) {
        char card = cards[0];
        char last_card = cards[cards.size() - 1];
        if (card == last_card) {
            cards[0] = 'X';
            return true;
        }
    }
    return false;
}

int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        std::string cards; std::cin >> cards;
        int num_changed = 0;
        if (is_replace_first_card(cards)) {
            cards[0] = 'X';
            num_changed++;
        }
        for (int i = 1; i < cards.size(); i++) {
            char card = cards[i];
            char last_card = cards[i - 1];
            if (card == last_card) {
                cards[i] = 'X';
                num_changed++;
            }
        }
        std::cout << "Case #" << t + 1 << ": " << num_changed << std::endl;
    }
    return 0;
}
