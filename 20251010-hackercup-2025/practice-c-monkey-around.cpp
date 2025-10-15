// https://www.facebook.com/codingcompetitions/hacker-cup/2025/practice-round/problems/C

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>


struct Group {
    int start_pos;
    int max;                // Elements: [1, max]
    int rotation;
};


const int UNDEFINED = -1;
const int ROTATE = 2;


std::vector<std::pair<int, int>> solve(const std::vector<int>& A) {
    // Approach:
    //   1. Split into groups. Each group has its own rotation number.
    //   2. Assumption: Earlier group shall be greedy.

    // Find positions of all 1. Each will form its own group.
    std::vector<int> position_of_ones;
    for (int i = 0; i < A.size(); i++) {
        if (A.at(i) == 1) {
            position_of_ones.push_back(i);
        }
    }

    // Construct groups
    std::vector<Group> groups;
    int i;
    for (i = 0; i < A.size(); ) {
        int position_of_one = position_of_ones.at(groups.size());
        Group group = {};
        group.start_pos = i;
        group.rotation = position_of_one - group.start_pos;
        if (group.rotation == 0) {
            // The group starts at 1. Do greedy.
            assert(A.at(i) == 1);
            group.max = 1;
            for (; i < A.size() && A.at(i) == group.max; i++) {
                group.max++;
            }
            group.max--;
            groups.push_back(group);
        } else {
            // From [start pos, position of one) there must be the maximum number
            assert(group.start_pos < position_of_one);
            assert(position_of_one <= A.size());
            auto it = std::max_element(A.begin() + group.start_pos, A.begin() + position_of_one);
            group.max = *it;
            assert(group.max > 0);
            for (int j = 0; j < group.max; j++, i++) {
                assert(A.at(i) == ((group.max - group.rotation + j) % group.max) + 1);                      // Example:  ... 4 5 6 7 8 1 2 3
            }
            groups.push_back(group);
        }
    }
    assert(i == A.size());
    assert(groups.size() == position_of_ones.size());

    // Calculate required rotation starting from last element
    std::vector<std::pair<int, int>> steps;
    long long total_rotation = 0;
    for (int i = groups.size() - 1; i >= 0; i--) {
        assert(groups.at(i).max > 0);
        // int rotation = (groups.at(i).max - groups.at(i).rotation) - total_rotation;
        // while (rotation < 0) rotation += groups.at(i).max;
        int rotation = (groups.at(i).max - ((groups.at(i).rotation + total_rotation) % groups.at(i).max)) % groups.at(i).max;
        assert(rotation >= 0);
        assert(rotation < groups.at(i).max);
        total_rotation += rotation;
        groups.at(i).rotation = rotation;
    }

    // Construct steps
    for (const Group& group : groups) {
        steps.push_back(std::make_pair(1, group.max));
        for (int i = 0; i < group.rotation; i++) {
            steps.push_back(std::make_pair(ROTATE, ROTATE));
        }
    }
    assert (steps.size() <= 2 * A.size());
    return steps;
}


void verify(const std::vector<std::pair<int, int>>& steps, const std::vector<int>& A) {
    std::vector<std::vector<int>> groups;
    for (const std::pair<int, int>& step : steps) {
        if (step.first == 1) {
            std::vector<int> group;
            for (int i = 1; i <= step.second; i++) group.push_back(i);
            groups.push_back(group);
        } else if (step.first == ROTATE) {
            for (std::vector<int>& group : groups) {
                assert(! group.empty());
                std::rotate(group.begin(), group.begin() + 1, group.end());
            }
        } else {
            assert(false);
        }
    }
    std::vector<int> expected;
    for (const std::vector<int>& group : groups) {
        expected.insert(expected.end(), group.begin(), group.end());
    }
    assert(A == expected);
}


void test() {
    {
        std::vector<int> A = {2, 3, 4, 5, 1, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 5, 1, 2, 3, 4};
        std::vector<std::pair<int, int>> steps = solve(A);
        verify(steps, A);
        std::vector<std::pair<int, int>> expected = {
            {1, 5},
            {2, 2},
            {2, 2},
            {2, 2},
            {1, 10},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
            {1, 5},
            {2, 2},
            {2, 2},
            {2, 2},
            {2, 2},
        };
        assert(steps == expected);
    }
    {
        std::vector<int> A = {1, 2, 3, 3, 4, 1, 2};
        std::vector<std::pair<int, int>> steps = solve(A);
        verify(steps, A);
        std::vector<std::pair<int, int>> expected = {
            {1, 3},
            {2, 2},
            {1, 4},
            {2, 2},
            {2, 2},
        };
        assert(steps == expected);
    }
    {
        std::vector<int> A = {3, 1, 2, 1};
        std::vector<std::pair<int, int>> steps = solve(A);
        verify(steps, A);
        std::vector<std::pair<int, int>> expected = {
            {1, 3},
            {2, 2},
            {2, 2},
            {1, 1},
        };
        assert(steps == expected);
    }
    {
        // Stress test: Small sets
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 100; T++) {
            std::vector<int> A;
            int group_size = int(rnd(mt) * 1000) + 1;                   // [1, 1000]
            for (int g = 0; g < group_size; g++) {
                int max = int(rnd(mt) * 10) + 1;
                int rotation = int(rnd(mt) * max);                      // [0, max)
                for (int j = 0; j < max; j++) {
                    A.push_back((max - rotation + j) % max + 1);
                }
            }
            std::vector<std::pair<int, int>> steps = solve(A);
            verify(steps, A);
        }
    }
    {
        // Stress test: Large set
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 1; T++) {
            std::vector<int> A;
            int group_size = int(rnd(mt) * 50) + 1;                     // [1, 50]
            for (int g = 0; g < group_size; g++) {
                int max = int(rnd(mt) * 75000) + 1;
                int rotation = int(rnd(mt) * max);                      // [0, max)
                for (int j = 0; j < max; j++) {
                    A.push_back((max - rotation + j) % max + 1);
                }
            }
            std::vector<std::pair<int, int>> steps = solve(A);
            verify(steps, A);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<int> A;
        for (int n = 0; n < N; n++) {
            int a; std::cin >> a;
            A.push_back(a);
        }
        std::vector<std::pair<int, int>> steps = solve(A);
        std::cout << "Case #" << t + 1 << ": " << steps.size() << std::endl;
        for (const std::pair<int, int>& step: steps) {
            if (step.first == ROTATE) {
                std::cout << 2 << std::endl;
            } else {
                std::cout << step.first << " " << step.second << std::endl;
            }
        }
    }
    return 0;
}
