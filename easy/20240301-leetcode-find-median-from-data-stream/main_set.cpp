// https://leetcode.com/problems/find-median-from-data-stream

#include <cassert>
#include <iostream>
#include <random>
#include <set>


class MedianFinder {
private:
    std::multiset<int> datas;
    std::multiset<int>::iterator middle;

public:
    void addNum(int num) {
        datas.insert(num);
        if (datas.size() == 1) {
            middle = datas.begin();
            return;
        }
        if (num >= *middle) {
            if (datas.size() % 2 == 0) middle++;
        } else {
            if (datas.size() % 2 == 1) middle--;
        }
    }

    double findMedian() {
        double median;
        if (datas.size() % 2 == 0) {
            auto before_middle = middle; before_middle--;
            median = ((double) *middle + *before_middle) / 2;
        } else {
            median = *middle;
        }
        return median;
    }
};


class MedianFinder2 {
private:
    std::multiset<int> datas;

public:
    void addNum(int num) {
        datas.insert(num);
    }

    double findMedian2() {
        double median;
        auto middle = datas.begin();
        std::advance(middle, datas.size() / 2);
        if (datas.size() % 2 == 0) {
            // 0  1  2  3
            auto before_middle = middle; before_middle--;
            median = ((double) *middle + *before_middle) / 2;
        } else {
            // 0  1  2
            median = *middle;
        }
        return median;
    }
};


int main() {
    {
        double median;
        MedianFinder solution = MedianFinder();
        solution.addNum(1);
        solution.addNum(2);
        median = solution.findMedian();
        assert(median == 1.5);
        solution.addNum(3);
        median = solution.findMedian();
        assert(median == 2.0);
        solution.addNum(4);
        median = solution.findMedian();
        assert(median == 2.5);
        solution.addNum(5);
        median = solution.findMedian();
        assert(median == 3.0);
    }
    {
        double median;
        MedianFinder solution = MedianFinder();
        solution.addNum(5);
        solution.addNum(4);
        median = solution.findMedian();
        assert(median == 4.5);
        solution.addNum(3);
        median = solution.findMedian();
        assert(median == 4.0);
        solution.addNum(2);
        median = solution.findMedian();
        assert(median == 3.5);
        solution.addNum(1);
        median = solution.findMedian();
        assert(median == 3.0);
    }
    {
        MedianFinder solution1 = MedianFinder();
        MedianFinder2 solution2 = MedianFinder2();

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> dist(-1000, 1000);
        for (int i = 0; i < 50000; i++) {
            int num = dist(mt);
            solution1.addNum(num);
            double median1 = solution1.findMedian();
            solution2.addNum(num);
            double median2 = solution2.findMedian2();
            assert(median1 == median2);
        }
    }
    return 0;
}
