// https://leetcode.com/problems/range-sum-query-mutable/

#include <cassert>
#include <random>
#include <vector>


class NumArray {
private:
    // Range Sum using Segment Tree
    struct Node {
        int index_min, index_max;                                               // Range: [index_min, index_max)
        int sum;
        Node* parent;
        Node* left;
        Node* right;
    };

    static Node* construct_node(Node* parent, const std::vector<int>& nums, int index_min, int index_max) {
        Node* node = new Node;
        node->index_min = index_min;
        node->index_max = index_max;
        node->parent = parent;
        if (index_min == index_max - 1) {
            // We are on a leaf
            node->sum = nums[index_min];
            node->left = nullptr;
            node->right = nullptr;
        } else {
            // We are on a branch
            node->left = construct_node(node, nums, index_min, (index_min + index_max) / 2);
            node->right = construct_node(node, nums, (index_min + index_max) / 2, index_max);
            node->sum = node->left->sum + node->right->sum;
        }
        return node;
    }

    static Node* find_node(Node* node, int index) {
        assert(node->index_min < node->index_max);
        assert(index >= node->index_min);
        assert(index < node->index_max);
        if (node->left == nullptr && node->right == nullptr) {
            // We are on a leaf
            assert(node->index_min == index);
            assert(node->index_max == index + 1);
            return node;
        } else {
            // We are on a branch
            if (index < node->left->index_max) {
                assert(node->left->index_min <= index);
                assert(index < node->left->index_max);
                return find_node(node->left, index);
            } else {
                assert(node->right->index_min <= index);
                assert(index < node->right->index_max);
                return find_node(node->right, index);
            }
        }
    }

    static int sum_range(Node* node, int index_min, int index_max) {
        if (index_min >= index_max) return 0;
        assert(node->index_min < node->index_max);
        assert(index_min >= node->index_min);
        assert(index_max <= node->index_max);
        if (index_min == node->index_min && index_max == node->index_max) {
            return node->sum;
        }
        assert(node->left->index_max == node->right->index_min);
        int sum = sum_range(node->left, index_min, std::min(index_max, node->left->index_max)) + sum_range(node->right, std::max(index_min, node->right->index_min), index_max);
        return sum;
    }

    Node* root;

public:
    NumArray(const std::vector<int>& nums) {
        root = construct_node(nullptr, nums, 0, nums.size());
    }

    void update(int index, int val) {
        Node* node = find_node(root, index);
        int delta = val - node->sum;
        while (node != nullptr) {
            node->sum += delta;
            node = node->parent;
        }
    }

    int sumRange(int left, int right) {                                         // Range: [left, right]
        int sum = sum_range(root, left, right + 1);
        return sum;
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

    int sumRange(int left, int right) {                                         // Range: [left, right]
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
