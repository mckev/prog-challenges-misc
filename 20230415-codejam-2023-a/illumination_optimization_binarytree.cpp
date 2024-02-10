// https://zibada.guru/gcj/2023a/problems/#B

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>


struct Interval {
    int64_t min;
    int64_t max;

    bool operator<(const Interval& other) const {
        return min < other.min;
    };
};

namespace BinaryTree {
    struct Node {
        Interval interval;        // interval of the Node and all its child Nodes recursively
        Node* left;
        Node* right;
    };

    Node* construct_node(const std::vector<Interval>& intervals_input, int index_min, int index_max) {
        // Note: intervals_input must be sorted!
        Node* node = new Node;
        if (index_min == index_max - 1) {
            // We are on a leaf
            node->interval = intervals_input[index_min];
            node->left = nullptr;
            node->right = nullptr;
        } else {
            // We are on a branch
            node->interval = {std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min()};
            node->left = construct_node(intervals_input, index_min, (index_min + index_max) / 2);
            node->right = construct_node(intervals_input, (index_min + index_max) / 2, index_max);
            if (node->left != nullptr) {
                node->interval = {std::min(node->interval.min, node->left->interval.min), std::max(node->interval.max, node->left->interval.max)};
            }
            if (node->right != nullptr) {
                node->interval = {std::min(node->interval.min, node->right->interval.min), std::max(node->interval.max, node->right->interval.max)};
            }
        }
        return node;
    }

    void find_overlapped_intervals(int64_t val, const Node* node, std::vector<Interval>& output) {
        if (node == nullptr) return;
        if (val < node->interval.min || val >= node->interval.max) return;          // val is overlapped when bulb_interval.min <= val < bulb_interval.max
        if (node->left == nullptr && node->right == nullptr) {
            // We are on a leaf
            output.push_back(node->interval);
        } else {
            // We are on a branch
            find_overlapped_intervals(val, node->left, output);
            find_overlapped_intervals(val, node->right, output);
        }
    }
}


const int64_t IMPOSSIBLE = -1;

int solve(int64_t M, std::vector<Interval>& bulb_intervals) {
    // Construct Node
    std::sort(bulb_intervals.begin(), bulb_intervals.end());
    BinaryTree::Node* node = BinaryTree::construct_node(bulb_intervals, 0, bulb_intervals.size());
    // Traverse the road
    int answer = 0;
    for (int64_t m = 0; m < M; ) {
        // Find all bulbs overlapped with m
        std::vector<Interval> overlapped_intervals;
        find_overlapped_intervals(m, node, overlapped_intervals);
        if (overlapped_intervals.size() == 0) return IMPOSSIBLE;
        for (const Interval& overlapped_interval : overlapped_intervals) {
            m = std::max(m, overlapped_interval.max);
        }
        answer++;
    }
    return answer;
}


int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int64_t M; std::cin >> M;       // road length
        int R; std::cin >> R;           // illumination radius
        int N; std::cin >> N;           // number of lights
        std::vector<Interval> bulb_intervals;
        for (int n = 0; n < N; n++) {
            int64_t bulb_pos; std::cin >> bulb_pos;
            bulb_intervals.push_back({bulb_pos - R, bulb_pos + R});
        }
        int answer = solve(M, bulb_intervals);
        if (answer == IMPOSSIBLE) {
            std::cout << "Case #" << t + 1 << ": IMPOSSIBLE" << std::endl;
        } else {
            std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
        }
    }
    return 0;
}
