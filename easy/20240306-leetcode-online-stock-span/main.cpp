// https://leetcode.com/problems/online-stock-span/

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>


struct Price {
    int price;
    int index;

    bool operator<(int other_price) const {
        return price > other_price;                                                                 // Descending
    }
};

class StockSpanner {
private:
    std::vector<Price> prices;
    int index = 0;

public:
    int next(int price) {
        int span;
        // Find the first element of "prices" that is lesser or equal than "price" (as we have descending order)
        auto lesser_equal_iterator = std::lower_bound(prices.begin(), prices.end(), price);
        if (lesser_equal_iterator == prices.end()) {
            // Not found, all elements are greater than "price"
            span = 1;
            prices.push_back({price, index});
        } else {
            if (lesser_equal_iterator == prices.begin()) {
                span = index + 1;
            } else {
                span = index - (*(lesser_equal_iterator - 1)).index;
            }
            *lesser_equal_iterator = {price, index};
            prices.erase(lesser_equal_iterator + 1, prices.end());
        }
        index++;
        return span;
    }
};


class StockSpanner2 {
private:
    std::vector<int> prices;

public:
    int next(int price) {
        prices.push_back(price);
        int span = 0;
        for (int i = prices.size() - 1; i >= 0; i--) {
            if (prices[i] <= price) {
                span++;
            } else {
                break;
            }
        }
        return span;
    }
};


int main() {
    {
        StockSpanner solution;
        assert(solution.next(100) == 1);
        assert(solution.next(80) == 1);
        assert(solution.next(60) == 1);
        assert(solution.next(70) == 2);
        assert(solution.next(60) == 1);
        assert(solution.next(75) == 4);
        assert(solution.next(85) == 6);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> price_rnd(1, 100000);
        for (int t = 0; t < 1000; t++) {
            StockSpanner solution;
            StockSpanner2 solution2;
            for (int i = 0; i < 10000; i++) {
                int price = price_rnd(mt);
                int span1 = solution.next(price);
                int span2 = solution2.next(price);
                assert(span1 == span2);
            }
        }
    }
    return 0;
}
