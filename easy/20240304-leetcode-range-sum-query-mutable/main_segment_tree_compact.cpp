// https://leetcode.com/problems/range-sum-query-mutable/

#include <cassert>
#include <random>
#include <vector>


class NumArray {
private:
    // Range Sum using Segment Tree (compacted)
    std::vector<int> nums;
    std::vector<int> nodes;
    const int ROOT_NODE = 0;

    void construct_node(int node, int start, int end) {                             // Range: [start, end)
        if (start == end - 1) {
            // We are on a leaf
            nodes[node] = nums[start];
        } else {
            // We are on a branch
            int mid = (start + end) / 2;
            int left_node = node * 2 + 1;
            int right_node = node * 2 + 2;
            construct_node(left_node, start, mid);
            construct_node(right_node, mid, end);
            nodes[node] = nodes[left_node] + nodes[right_node];
        }
    }

    int find_node(int node, int start, int end, const int index) const {
        if (start == end - 1) {
            // We are on a leaf. Yes, we found it!
            assert(start == index);
            return node;
        } else {
            int mid = (start + end) / 2;
            int left_node = node * 2 + 1;
            int right_node = node * 2 + 2;
            if (index < mid) {
                // It should be on left node
                return find_node(left_node, start, mid, index);
            } else {
                // It should be on right node
                return find_node(right_node, mid, end, index);
            }
        }
    }

    int sum_range(int node, int start, int end, int left, int right) const {
        if (left >= right) return 0;
        if (left == start && right == end) {
            return nodes[node];
        }
        assert (start != end - 1);
        if (right - left < 16) {
            // Shortcut: If number of nodes are small, then directly compute
            int sum = 0;
            for (int i = left; i < right; i++) {
                sum += nums[i];
            }
            return sum;
        }
        int mid = (start + end) / 2;
        int left_node = node * 2 + 1;
        int right_node = node * 2 + 2;
        return sum_range(left_node, start, mid, left, std::min(right, mid)) + sum_range(right_node, mid, end, std::max(left, mid), right);
    }

public:
    NumArray(const std::vector<int>& _nums) {
        nums = _nums;
        nodes = std::vector<int>(65536, 0);
        construct_node(ROOT_NODE, 0, nums.size());
    }

    void update(int index, int val) {
        nums[index] = val;
        int node = find_node(ROOT_NODE, 0, nums.size(), index);
        int delta = val - nodes[node];
        while (true) {
            nodes[node] += delta;
            if (node == ROOT_NODE) break;
            int parent_node = (node - 1) / 2;
            node = parent_node;
        }
    }

    int sumRange(int left, int right) const {                                       // Range: [left, right]
        return sum_range(ROOT_NODE, 0, nums.size(), left, right + 1);
    }
};


class NumArray2 {
private:
    std::vector<int> nums;

public:
    NumArray2(const std::vector<int>& _nums) {
        nums = _nums;
    }

    void update(int index, int val) {
        nums[index] = val;
    }

    int sumRange(int left, int right) const {                                       // Range: [left, right]
        int sum = 0;
        for (int i = left; i <= right; i++) {
            sum += nums[i];
        }
        return sum;
    }
};


int main() {
    {
        std::vector<int> nums = {1, 3, 5};
        NumArray solution = NumArray(nums);
        int sum;
        sum = solution.sumRange(0, 2);
        assert(sum == 9);
        solution.update(1, 2);
        sum = solution.sumRange(0, 2);
        assert(sum == 8);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);
        for (int t = 0; t < 1000; t++) {
            int len = int(rnd(mt) * 30000) + 1;
            std::vector<int> nums;
            for (int i = 0; i < len; i++) {
                int num = int(rnd(mt) * 200) - 100;
                nums.push_back(num);
            }
            NumArray solution = NumArray(nums);
            NumArray2 solution2 = NumArray2(nums);
            for (int i = 0; i < 100; i++) {
                int range1 = int(rnd(mt) * len);
                int range2 = int(rnd(mt) * (len - range1)) + range1;
                if (i % 10 == 0) {
                    int index = int(rnd(mt) * len);
                    int num = int(rnd(mt) * 200) - 100;
                    solution.update(index, num);
                    solution2.update(index, num);
                }
                int sum1 = solution.sumRange(range1, range2);
                int sum2 = solution2.sumRange(range1, range2);
                assert(sum1 == sum2);
            }
        }
    }
    return 0;
}
