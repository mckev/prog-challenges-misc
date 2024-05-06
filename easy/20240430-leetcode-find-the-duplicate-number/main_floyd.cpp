// https://leetcode.com/problems/find-the-duplicate-number/

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>


class Solution {
public:
    int findDuplicate(const std::vector<int>& nums) {
        // Floyd's cycle-finding algorithm
        // https://takeuforward.org/data-structure/starting-point-of-loop-in-a-linked-list/
        int slow_index = 0;
        int fast_index = 0;
        while (true) {
            slow_index = nums[slow_index];
            fast_index = nums[nums[fast_index]];
            if (slow_index == fast_index) break;
        }
        fast_index = 0;
        while (true) {
            slow_index = nums[slow_index];
            fast_index = nums[fast_index];
            if (slow_index == fast_index) break;
        }
        return slow_index;
    }
};


class Solution2 {
public:
    int IMPOSSIBLE = -1;
    int findDuplicate(const std::vector<int>& nums) {
        std::vector<int> count(nums.size());
        for (int num : nums) {
            count[num]++;
            if (count[num] > 1) return num;
        }
        return IMPOSSIBLE;
    }
};


int main() {
    {
        Solution solution;
        std::vector<int> nums = {1, 3, 4, 2, 2};
        assert(solution.findDuplicate(nums) == 2);
    }
    {
        Solution solution;
        std::vector<int> nums = {3, 1, 3, 4, 2};
        assert(solution.findDuplicate(nums) == 3);
    }
    {
        Solution solution;
        std::vector<int> nums = {3, 3, 3, 3, 3};
        assert(solution.findDuplicate(nums) == 3);
    }
    {
        Solution solution;
        std::vector<int> nums = {14,16,12,1,16,17,6,8,5,19,16,13,16,3,11,16,4,16,9,7};
        assert(solution.findDuplicate(nums) == 16);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 10000; T++) {
            std::vector<int> nums;
            // int N = int(rnd(mt) * 100000) + 1;                          // 1 <= N <= 100000
            int N = int(rnd(mt) * 1000) + 1;
            for (int n = 0; n < N; n++) {
                nums.push_back(n + 1);
            }
            int expected = int(rnd(mt) * N) + 1;                        // 1 <= expected <= 100000
            nums.push_back(expected);
            std::shuffle(nums.begin(), nums.end(), mt);
            int repeat = int(rnd(mt) * N);
            for (int i = 0; i < repeat; i++) {
                nums[i] = expected;
            }
            std::shuffle(nums.begin(), nums.end(), mt);
            {
                Solution2 solution;
                int answer = solution.findDuplicate(nums);
                assert(answer == expected);
            }
            {
                Solution solution;
                int answer = solution.findDuplicate(nums);
                assert(answer == expected);
            }
        }
    }
    return 0;
}
