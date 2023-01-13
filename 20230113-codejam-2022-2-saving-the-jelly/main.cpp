// Google Code Jam 2022 Round 2 : Saving the Jelly
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b158f8

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>


struct bipartite_match {
    // Ref: https://judge.yosupo.jp/submission/1548
    // if (~a) is equal to: if (a != -1)
    // if (!~a) is equal to: if (a == -1)
    int n, m;
    std::vector<int> p, q;
    std::vector<int> pre, root;
    std::vector<std::vector<int>> to;

    bipartite_match(int _n, int _m) {
        n = _n;
        m = _m;
        p = std::vector<int>(n, -1);
        q = std::vector<int>(m, -1);
        pre = std::vector<int>(n, -1);
        root = std::vector<int>(n, -1);
        to = std::vector<std::vector<int>>(n);
    }

    void add(int a, int b) {
        to[a].push_back(b);
    }

    int solve() {
        int result = 0;
        bool updated = true;
        while (updated) {
            updated = false;
            std::queue<int> s;
            for (int i = 0; i < n; i++) {
                if (p[i] == -1) {
                    root[i] = i;
                    s.push(i);
                }
            }
            while (s.size() > 0) {
                int v = s.front(); s.pop();
                if (p[root[v]] != -1) continue;
                for (int i = 0; i < to[v].size(); i++) {
                    int u = to[v][i];
                    if (q[u] == -1) {
                        while (u != -1) {
                            q[u] = v;
                            std::swap(p[v], u);
                            v = pre[v];
                        }
                        updated = true;
                        result++;
                        break;
                    }
                    u = q[u];
                    if (pre[u] != -1) continue;
                    pre[u] = v; root[u] = root[v];
                    s.push(u);
                }
            }
            if (updated) {
                std::fill(pre.begin(), pre.end(), -1);
                std::fill(root.begin(), root.end(), -1);
            }
        }
        return result;
    }
};

void test_bipartite_match() {
    // https://www.geeksforgeeks.org/maximum-bipartite-matching/
    int n = 6, m = 6;
    bipartite_match bpm(n, m);
    bpm.add(0, 1);
    bpm.add(0, 2);
    bpm.add(2, 0);
    bpm.add(2, 3);
    bpm.add(3, 2);
    bpm.add(4, 2);
    bpm.add(4, 3);
    bpm.add(5, 5);

    int max_match = bpm.solve();
    assert (max_match == 5);
    std::vector<int> expected_p = {1, -1, 0, 2, 3, 5};
    assert (bpm.p == expected_p);
    std::vector<int> expected_q = {2, 0, 3, 4, -1, 5};
    assert (bpm.q == expected_q);
}


struct Coord {
    int x, y;
};

double dist(const Coord& coord1, const Coord& coord2) {
    return std::pow(coord1.x - coord2.x, 2) + std::pow(coord1.y - coord2.y, 2);
}

struct AnswerT {
    int child;
    int sweet;
    bool operator==(const AnswerT& other) const {
        return std::tie(child, sweet) == std::tie(other.child, other.sweet);
    }
};

std::vector<AnswerT> solve(int N, const std::vector<Coord>& pos_childs, const std::vector<Coord>& pos_sweets) {
    std::vector<AnswerT> answer;
    assert (pos_childs.size() == N);
    assert (pos_sweets.size() == N + 1);
    // Bipartite match
    bipartite_match bpm(N, N + 1);
    for (int child = 0; child < N; child++) {
        double dist_to_sweet0 = dist(pos_childs[child], pos_sweets[0]);
        for (int sweet = 1; sweet < N + 1; sweet++) {
            double dist_to_sweet = dist(pos_childs[child], pos_sweets[sweet]);
            if (dist_to_sweet > dist_to_sweet0) continue;
            bpm.add(child, sweet);
        }
    }
    int max_match = bpm.solve();
    if (max_match != N) {
        // IMPOSSIBLE
        return answer;
    }
    // sweet_edge : One sweet to one child, however it's not necessarily the closest
    std::map<int, int> sweet_edge;
    assert (bpm.q.size() == N + 1);
    for (int sweet = 0; sweet < N + 1; sweet++) {
        int child = bpm.q[sweet];
        if (child == -1) continue;
        sweet_edge[sweet] = child;
    }
    // child_edges : One child to many sweets, sorted by its dist (closest sweet last)
    std::map<int, std::vector<int>> child_edges;
    for (int child = 0; child < N; child++) {
        double dist_to_sweet0 = dist(pos_childs[child], pos_sweets[0]);
        for (int sweet = 1; sweet < N + 1; sweet++) {
            double dist_to_sweet = dist(pos_childs[child], pos_sweets[sweet]);
            if (dist_to_sweet > dist_to_sweet0) continue;
            child_edges[child].push_back(sweet);
        }
        Coord pos_child = pos_childs[child];
        std::sort(child_edges[child].begin(), child_edges[child].end(), [pos_child, pos_sweets](int sweet1_no, int sweet2_no) -> bool {
            return dist(pos_child, pos_sweets[sweet1_no]) > dist(pos_child, pos_sweets[sweet2_no]);
        });
    }
    while (! child_edges.empty()) {
        // Find alternate path to make sure there will always be at least one child with their closest sweet in the graph
        {
            std::deque<int> path;
            std::set<int> path_set;
            int child = (*(child_edges.begin())).first;
            bool is_in = false;
            while (! is_in) {
                // Find a unicyclic graph
                path.push_back(child);
                path_set.insert(child);
                int closest_sweet = child_edges[child].back();
                child = sweet_edge[closest_sweet];
                is_in = path_set.find(child) != path_set.end();
            }
            while (path.front() != child) path.pop_front();
            for (const int& child : path) {
                int closest_sweet = child_edges[child].back();
                sweet_edge[closest_sweet] = child;
            }
        }
        // Remove childs and their closest sweets if they are in the graph
        std::vector<int> remaining_childs;
        for (const auto& kv : child_edges) {
            remaining_childs.push_back(kv.first);
        }
        for (const int& child : remaining_childs) {
            int closest_sweet = child_edges[child].back();
            if (sweet_edge[closest_sweet] != child) continue;
            answer.push_back({child, closest_sweet});
            // Remove the existence of the child and the sweet
            sweet_edge.erase(closest_sweet);
            child_edges.erase(child);
            for (auto& kv : child_edges) {
                std::vector<int> sweets = kv.second;
                std::vector<int> new_sweets;
                std::copy_if(sweets.begin(), sweets.end(), std::back_inserter(new_sweets), [closest_sweet](int sweet) -> bool {
                    return sweet != closest_sweet;
                });
                kv.second = new_sweets;
            }
        }
    }
    assert (answer.size() == N);
    return answer;
}


void test() {
    {
        int N = 2;
        std::vector<Coord> pos_childs = {
            {-3, 0},
            {-1, 0}
        };
        std::vector<Coord> pos_sweets = {
            {3, 0},
            {-1, -1},
            {-2, 1}
        };
        std::vector<AnswerT> answer = solve(N, pos_childs, pos_sweets);
        std::vector<AnswerT> expected = {
            {0, 2},
            {1, 1}
        };
        assert (answer == expected);
    }
}


int main() {
    // test_bipartite_match();
    // test();

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<Coord> pos_childs;
        for (int n = 0; n < N; n++) {
            int x; std::cin >> x;
            int y; std::cin >> y;
            pos_childs.push_back({x, y});
        }
        std::vector<Coord> pos_sweets;
        for (int n = 0; n < N + 1; n++) {
            int x; std::cin >> x;
            int y; std::cin >> y;
            pos_sweets.push_back({x, y});
        }
        std::vector<AnswerT> answer = solve(N, pos_childs, pos_sweets);
        if (answer.empty()) {
            std::cout << "Case #" << t + 1 << ": IMPOSSIBLE" << std::endl;
        } else {
            std::cout << "Case #" << t + 1 << ": POSSIBLE" << std::endl;
            for (const AnswerT& answer_each : answer) {
                std::cout << answer_each.child + 1 << " " << answer_each.sweet + 1 << std::endl;
            }
        }
    }
    return 0;
}
