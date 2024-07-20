// https://www.facebook.com/codingcompetitions/hacker-cup/2020/round-1/problems/A3
// How to run:
//    $ ulimit -s unlimited
//    $ cat main.cpp; rm -f main.out; g++ -Ofast -o main.out main.cpp; cat perimetric_chapter_3_input.txt | time ./main.out

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>


namespace IntervalTree {
    struct Interval {
        int low, high;
        int height;                                     // specific data
    };
    struct Node {
        Interval interval;
        int min, max;
        Node* left;
        Node* right;
    };
    static Node* new_node(Interval interval) {
        Node* node = new Node;
        node->interval = interval;
        node->min = interval.low;
        node->max = interval.high;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    Node* upsert_node(Node* node, const Interval interval) {
        // Ref: https://www.geeksforgeeks.org/interval-tree/
        if (node == nullptr) return new_node(interval);
        if (interval.low < node->interval.low) {
            node->left = upsert_node(node->left, interval);
        } else {
            node->right = upsert_node(node->right, interval);
        }
        node->min = std::min(node->min, interval.low);
        node->max = std::max(node->max, interval.high);
        return node;
    }
    void find_overlapped_intervals(const int val, const Node* node, std::vector<Interval>& output) {
        if (node == nullptr) return;
        if (val < node->min || val >= node->max) {
            return;
        }
        if (val >= node->interval.low && val < node->interval.high) {
            output.push_back(node->interval);
        }
        find_overlapped_intervals(val, node->left, output);
        find_overlapped_intervals(val, node->right, output);
    }
    void test() {
        IntervalTree::Node* node = nullptr;
        node = IntervalTree::upsert_node(node, {-1, 0});
        IntervalTree::upsert_node(node, {0, 3});
        IntervalTree::upsert_node(node, {0, 3});
        IntervalTree::upsert_node(node, {-1, 5});
        IntervalTree::upsert_node(node, {10, 11});
        IntervalTree::upsert_node(node, {9, 12});
        std::vector<IntervalTree::Interval> intervals_output;
        int val = 10;
        IntervalTree::find_overlapped_intervals(val, node, intervals_output);
        for (const IntervalTree::Interval& interval : intervals_output) {
            std::cout << "[" << interval.low << ", " << interval.high << ")" << std::endl;
        }
    }
};


// Solution
long long modulo(long long x) {
    if (x < 0) {
        x = x % 1000000007 + 1000000007;
    }
    return x % 1000000007;
}

int get_max_height(IntervalTree::Node* height_ranges, int x) {
    int max_height = 0;
    std::vector<IntervalTree::Interval> intervals_output;
    IntervalTree::find_overlapped_intervals(x, height_ranges, intervals_output);
    for (const IntervalTree::Interval& interval : intervals_output) {
        max_height = std::max(max_height, interval.height);
    }
    return max_height;
}

int solve(const std::vector<int>& L, const std::vector<int>& W, const std::vector<int>& H) {
    std::vector<long long> P;

    // Initialize Height Ranges
    IntervalTree::Node* height_ranges = nullptr;
    int N = L.size();

    // Building Boundaries
    struct BuildingBoundary {
        int x;
        mutable int height;
        bool operator<(const BuildingBoundary& other) const {
            // For set
            return x < other.x;
        }
        bool operator<(int other_x) const {
            // For lower_bound()
            return x < other_x;
        }
        static bool upper_bound_comparator(int x, const BuildingBoundary& other) {
            // For upper_bound()
            return x < other.x;
        }
    };
    std::set<BuildingBoundary> building_boundaries;

    long long p = 0;
    for (int n = 0; n < N; n++) {
        // Perimeter before
        long long prev_p = 0;
        {
            int prev_x = 0;
            int prev_height = 0;
            auto it_start = std::lower_bound(building_boundaries.begin(), building_boundaries.end(), L[n]); if (it_start != building_boundaries.begin()) it_start--;
            auto it_end = std::upper_bound(building_boundaries.begin(), building_boundaries.end(), L[n] + W[n], BuildingBoundary::upper_bound_comparator); if (it_end != building_boundaries.end()) it_end++;
            for (auto it = it_start; it != it_end; it++) {
                BuildingBoundary building_boundary = *it;
                int x = building_boundary.x;
                int height = building_boundary.height;
                if (prev_height > 0) {
                    prev_p += 2 * (x - prev_x);                         // the base and top
                }
                prev_p += std::abs(prev_height - height);               // the side
                prev_x = x;
                prev_height = height;
            }
        }

        // Process rectangle
        {
            // We remove previous boundaries, as the new building is taller
            auto it_start = std::lower_bound(building_boundaries.begin(), building_boundaries.end(), L[n]);
            auto it_end = std::lower_bound(building_boundaries.begin(), building_boundaries.end(), L[n] + W[n]);
            for (auto it = it_start; it != it_end; ) {
                it = building_boundaries.erase(it);
            }
        }
        building_boundaries.insert({ L[n], H[n] });
        // We need to calculate the height after the building
        building_boundaries.insert({ L[n] + W[n], get_max_height(height_ranges, L[n] + W[n]) });
        IntervalTree::Interval interval = {L[n], L[n] + W[n], H[n]};
        height_ranges = IntervalTree::upsert_node(height_ranges, interval);

        // Perimeter after
        long long cur_p = 0;
        {
            int prev_x = 0;
            int prev_height = 0;
            auto it_start = std::lower_bound(building_boundaries.begin(), building_boundaries.end(), L[n]); if (it_start != building_boundaries.begin()) it_start--;
            auto it_end = std::upper_bound(building_boundaries.begin(), building_boundaries.end(), L[n] + W[n], BuildingBoundary::upper_bound_comparator); if (it_end != building_boundaries.end()) it_end++;
            for (auto it = it_start; it != it_end; it++) {
                BuildingBoundary building_boundary = *it;
                int x = building_boundary.x;
                int height = building_boundary.height;
                if (prev_height > 0) {
                    cur_p += 2 * (x - prev_x);                         // the base and top
                }
                cur_p += std::abs(prev_height - height);               // the side
                prev_x = x;
                prev_height = height;
            }
        }

        p = p + cur_p - prev_p;
        p = modulo(p);
        P.push_back(p);
    }
    long long answer = 1;
    for (long long p : P) {
        answer = modulo(answer * p);
    }
    return answer;
}


int main() {
    {
        // Test Case #4
        int N = 10;
        int K = 3;
        std::vector<int> L = {14, 5, 14};
        assert(L.size() == K);
        long long Al = 4, Bl = 7, Cl = 2, Dl = 47;
        std::vector<int> W = {6, 4, 13};
        assert(W.size() == K);
        long long Aw = 2, Bw = 3, Cw = 17, Dw = 33;
        std::vector<int> H = {11, 13, 14};
        assert(H.size() == K);
        long long Ah = 1, Bh = 0, Ch = 2, Dh = 31;
        for (int n = K; n < N; n++) {
            int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
            L.push_back(l);
            int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
            W.push_back(w);
            int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
            H.push_back(h);
        }
        assert(L.size() == N);
        assert(W.size() == N);
        assert(H.size() == N);
        int answer = solve(L, W, H);
        assert(answer == 842325442);
    }
    {
        // Test Case #8
        // int N = 200000;
        // int K = 2;
        // std::vector<int> L = {1, 3};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 100, Dl = 500000000;
        // std::vector<int> W = {100, 100};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 0, Cw = 99, Dw = 500000000;
        // std::vector<int> H = {300000000, 300000000};
        // assert(H.size() == K);
        // long long Ah = 1, Bh = 0, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 341232474);
    }
    {
        // Test Case #9
        // int N = 1000000;
        // int K = 2;
        // std::vector<int> L = {1, 2};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 0, Dl = 500000000;
        // std::vector<int> W = {2, 3};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 1, Cw = 0, Dw = 10000000;
        // std::vector<int> H = {1, 2};
        // assert(H.size() == K);
        // long long Ah = 0, Bh = 1, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 0);
    }
    {
        // Test Case #10
        // int N = 1000000;
        // int K = 2;
        // std::vector<int> L = {1, 10000000};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 10000000, Dl = 500000000;
        // std::vector<int> W = {2, 3};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 1, Cw = 0, Dw = 10000000;
        // std::vector<int> H = {1, 2};
        // assert(H.size() == K);
        // long long Ah = 0, Bh = 1, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 0);
    }

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N, K; std::cin >> N >> K;
        std::vector<int> L;
        for (int k = 0; k < K; k++) {
            int i; std::cin >> i;
            L.push_back(i);
        }
        assert(L.size() == K);
        long long Al, Bl, Cl, Dl; std::cin >> Al >> Bl >> Cl >> Dl;
        std::vector<int> W;
        for (int k = 0; k < K; k++) {
            int i; std::cin >> i;
            W.push_back(i);
        }
        assert(W.size() == K);
        long long Aw, Bw, Cw, Dw; std::cin >> Aw >> Bw >> Cw >> Dw;
        std::vector<int> H;
        for (int k = 0; k < K; k++) {
            int i; std::cin >> i;
            H.push_back(i);
        }
        assert(H.size() == K);
        long long Ah, Bh, Ch, Dh; std::cin >> Ah >> Bh >> Ch >> Dh;

        // Compute L from Al, Bl, Cl, Dl. Compute W from Aw, Bw, Cw, Dw. Compute H from Ah, Bh, Ch, Dh.
        for (int n = K; n < N; n++) {
            int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
            L.push_back(l);
            int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
            W.push_back(w);
            int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
            H.push_back(h);
        }
        assert(L.size() == N);
        assert(W.size() == N);
        assert(H.size() == N);

        int answer = solve(L, W, H);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
