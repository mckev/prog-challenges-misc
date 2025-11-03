// https://www.facebook.com/codingcompetitions/hacker-cup/2025/practice-round/problems/D
// How to run:
//   $ ulimit -s unlimited
//   $ code .

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <vector>


class SolutionBruteforce {
public:
    struct Answer {
        long long best_cost;
        std::vector<int> best_schedules;
    };
    Answer solve(int N, const std::vector<std::pair<int, int>>& activities) {
        struct Queue {
            int cur_activity;
            std::map<int, int> day1;        // key: coder, value: number of activities
            std::map<int, int> day2;
            long long cost;
            std::vector<int> schedules;
            bool operator<(const Queue& other) const {
                // Minimize cost
                return cost > other.cost;
            }
        };
        std::priority_queue<Queue> queues;
        queues.push({ -1 });
        while (! queues.empty()) {
            Queue queue; queue = queues.top(); queues.pop();
            if (queue.cur_activity == activities.size() - 1) {
                // This is optimal
                assert(queue.schedules.size() == activities.size());
                Answer answer = { queue.cost, queue.schedules };
                return answer;
            }
            int cur_activity = queue.cur_activity + 1;
            auto [coder1, coder2] = activities.at(cur_activity);
            assert(coder1 != coder2);
            {
                std::map<int, int> day1; day1 = queue.day1;
                day1[coder1]++;
                day1[coder2]++;
                long long cost = queue.cost
                    - queue.day1[coder1] * queue.day1[coder1] /* previous */ + day1[coder1] * day1[coder1]
                    - queue.day1[coder2] * queue.day1[coder2] /* previous */ + day1[coder2] * day1[coder2];
                // Schedules
                std::vector<int> schedules = queue.schedules; schedules.push_back(1);
                queues.push({ cur_activity, day1, queue.day2, cost, schedules });
            }
            {
                std::map<int, int> day2; day2 = queue.day2;
                day2[coder1]++;
                day2[coder2]++;
                long long cost = queue.cost
                    - queue.day2[coder1] * queue.day2[coder1] /* previous */ + day2[coder1] * day2[coder1]
                    - queue.day2[coder2] * queue.day2[coder2] /* previous */ + day2[coder2] * day2[coder2];
                // Schedules
                std::vector<int> schedules = queue.schedules; schedules.push_back(2);
                queues.push({ cur_activity, queue.day1, day2, cost, schedules });
            }
        }
        assert(false);
    }
};


class SolutionDfs {
private:
    const int UNASSIGNED = -1;
    const int DAY_1 = 1;
    const int DAY_2 = 2;
    std::vector<int> edges_color;       // Our answer: UNASSIGNED, DAY_1, DAY_2

    struct Edge {
        int edge_id;
        int node_id_to;
    };

    struct Node {
        int counter_day_1;
        int counter_day_2;
    };

    bool assign_dfs(int node_id_from, std::map<int, Node>& nodes, const std::map<int, std::vector<Edge>>& edges_of_a_node) {
        Node& node_from = nodes.at(node_id_from);

        // Process one unassigned edges. Note that when we call dfs() recursively, it can potentially process the other unassigned edges.
        Edge unassigned_edge = { UNASSIGNED, UNASSIGNED };
        for (const Edge& edge : edges_of_a_node.at(node_id_from)) {
            // Edges which does not have color assigned (i.e. unassigned).
            if (edges_color.at(edge.edge_id) == UNASSIGNED) {
                unassigned_edge = edge;
                break;
            }
        }
        if (unassigned_edge.edge_id != UNASSIGNED) {
            int edge_id = unassigned_edge.edge_id;
            int node_id_to = unassigned_edge.node_id_to;
            Node& node_to = nodes.at(node_id_to);
            if (
                (node_from.counter_day_1 == node_from.counter_day_2 && node_to.counter_day_1 < node_to.counter_day_2) ||
                (node_from.counter_day_1 < node_from.counter_day_2)
            ) {
                edges_color.at(edge_id) = DAY_1;
                node_from.counter_day_1++;
                node_to.counter_day_1++;
                assert(node_from.counter_day_1 <= edges_of_a_node.at(node_id_from).size()); assert(node_to.counter_day_1 <= edges_of_a_node.at(node_id_to).size());
                assert(node_from.counter_day_2 <= edges_of_a_node.at(node_id_from).size()); assert(node_to.counter_day_2 <= edges_of_a_node.at(node_id_to).size());
                assign_dfs(node_id_to, nodes, edges_of_a_node);
            } else {
                edges_color.at(edge_id) = DAY_2;
                node_from.counter_day_2++;
                node_to.counter_day_2++;
                assert(node_from.counter_day_1 <= edges_of_a_node.at(node_id_from).size()); assert(node_to.counter_day_1 <= edges_of_a_node.at(node_id_to).size());
                assert(node_from.counter_day_2 <= edges_of_a_node.at(node_id_from).size()); assert(node_to.counter_day_2 <= edges_of_a_node.at(node_id_to).size());
                assign_dfs(node_id_to, nodes, edges_of_a_node);
            }
            return true;
        }
        return false;
    }

    bool balance_dfs(int node_id_from, std::map<int, Node>& nodes, const std::map<int, std::vector<Edge>>& edges_of_a_node, std::set<int>& edges_history) {
        Node& node_from = nodes.at(node_id_from);

        // Case unbalanced
        if (std::abs(node_from.counter_day_1 - node_from.counter_day_2) > 1) {
            if (node_from.counter_day_1 > node_from.counter_day_2) {
                Edge unbalanced_edge = { UNASSIGNED, UNASSIGNED };
                {
                    std::vector<Edge> candidates;
                    for (const Edge& edge : edges_of_a_node.at(node_id_from)) {
                        bool is_found = edges_history.find(edge.edge_id) != edges_history.end();
                        if (edges_color.at(edge.edge_id) == DAY_1 && !is_found) {
                            candidates.push_back(edge);
                        }
                    }
                    if (candidates.size() == 0) return false;
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
                    unbalanced_edge = candidates[dist(gen)];
                }
                assert(unbalanced_edge.edge_id != UNASSIGNED);
                assert(edges_color.at(unbalanced_edge.edge_id) == DAY_1);
                int node_id_to = unbalanced_edge.node_id_to;
                Node& node_to = nodes.at(node_id_to);
                node_from.counter_day_1--;
                node_to.counter_day_1--;
                edges_color.at(unbalanced_edge.edge_id) = DAY_2;
                node_from.counter_day_2++;
                node_to.counter_day_2++;
                edges_history.insert(unbalanced_edge.edge_id);
                balance_dfs(node_id_to, nodes, edges_of_a_node, edges_history);
            } else if (node_from.counter_day_2 > node_from.counter_day_1) {
                Edge unbalanced_edge = { UNASSIGNED, UNASSIGNED };
                {
                    std::vector<Edge> candidates;
                    for (const Edge& edge : edges_of_a_node.at(node_id_from)) {
                        bool is_found = edges_history.find(edge.edge_id) != edges_history.end();
                        if (edges_color.at(edge.edge_id) == DAY_2 && !is_found) {
                            candidates.push_back(edge);
                        }
                    }
                    if (candidates.size() == 0) return false;
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
                    unbalanced_edge = candidates[dist(gen)];
                }
                assert(unbalanced_edge.edge_id != UNASSIGNED);
                assert(edges_color.at(unbalanced_edge.edge_id) == DAY_2);
                int node_id_to = unbalanced_edge.node_id_to;
                Node& node_to = nodes.at(node_id_to);
                node_from.counter_day_2--;
                node_to.counter_day_2--;
                edges_color.at(unbalanced_edge.edge_id) = DAY_1;
                node_from.counter_day_1++;
                node_to.counter_day_1++;
                edges_history.insert(unbalanced_edge.edge_id);
                balance_dfs(node_id_to, nodes, edges_of_a_node, edges_history);
            } else {
                assert(false);
            }
            return false;
        }
        return true;
    }

public:
    struct Answer {
        long long best_cost;
        std::vector<int> best_schedules;
    };
    Answer solve(int N, const std::vector<std::pair<int, int>>& activities) {
        // Edges
        std::map<int, std::vector<Edge>> edges_of_a_node;                           // const
        for (int edge_id = 0; edge_id < activities.size(); edge_id++) {
            std::pair<int, int> activity = activities.at(edge_id);
            int node1 = activity.first;
            int node2 = activity.second;
            edges_of_a_node[node1].push_back({ edge_id, node2 });
            edges_of_a_node[node2].push_back({ edge_id, node1 });
        }
        edges_color = std::vector<int>(activities.size(), UNASSIGNED);

        // Nodes
        std::map<int, Node> nodes;
        for (int edge_id = 0; edge_id < activities.size(); edge_id++) {
            std::pair<int, int> activity = activities.at(edge_id);
            int node1 = activity.first;
            int node2 = activity.second;
            nodes[node1] = { 0, 0 };
            nodes[node2] = { 0, 0 };
        }

        // Decide initial edges color
        for (const auto& [node_id, node] : nodes) {
            while (assign_dfs(node_id, nodes, edges_of_a_node)) {
                // If we can still assigning color, keep on same node
            };
        }

        // Balance
        for (int i = 0; i < 10; i++) {
            // Case triangle: we will not be able to make it balanced. So limit the effort.
            bool is_balanced = true;
            for (const auto& [node_id, node] : nodes) {
                std::set<int> edges_history;
                if (! balance_dfs(node_id, nodes, edges_of_a_node, edges_history)) {
                    is_balanced = false;
                }
            }
            if (is_balanced) break;
        }

        // Answer
        Answer answer = {};
        std::map<int, int> day1;
        std::map<int, int> day2;
        for (int edge_id = 0; edge_id < activities.size(); edge_id++) {
            std::pair<int, int> activity = activities.at(edge_id);
            int node1 = activity.first;
            int node2 = activity.second;
            int edge_color = edges_color.at(edge_id);
            if (edge_color == DAY_1) {
                day1[node1]++;
                day1[node2]++;
            } else if (edge_color == DAY_2) {
                day2[node1]++;
                day2[node2]++;
            } else {
                assert(false);
            }
        }
        for (int n = 1; n <= N; n++) {
            answer.best_cost += (long long) day1[n] * day1[n] + (long long) day2[n] * day2[n];
        }
        answer.best_schedules = edges_color;
        return answer;
    }
};


void test() {
    {
        int N = 5;
        std::vector<std::pair<int, int>> activities = {
            { 1, 2 },
            { 4, 2 },
            { 1, 5 },
            { 2, 3 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 10);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 6;
        std::vector<std::pair<int, int>> activities = {
            { 1, 5 },
            { 6, 3 },
            { 2, 3 },
            { 1, 2 },
            { 4, 3 },
            { 2, 6 },
            { 5, 2 },
            { 2, 4 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 26);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 5;
        std::vector<std::pair<int, int>> activities = {
            { 4, 5 },
            { 1, 3 },
            { 1, 5 },
            { 3, 2 },
            { 2, 1 },
            { 1, 4 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 16);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 3;
        std::vector<std::pair<int, int>> activities = {
            { 3, 1 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 2);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 8;
        std::vector<std::pair<int, int>> activities = {
            { 2, 4 },
            { 4, 5 },
            { 2, 3 },
            { 1, 6 },
            { 6, 7 },
            { 3, 5 },
            { 3, 4 },
            { 6, 8 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 22);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        int N = 11;
        std::vector<std::pair<int, int>> activities = {
            { 1, 2 },
            { 2, 3 },
            { 3, 4 },
            { 4, 5 },
            { 5, 3 },
            { 3, 6 },
            { 6, 7 },
            { 7, 3 },
            { 3, 8 },
            { 8, 9 },
            { 9, 3 },
            { 3, 10 },
            { 10, 11 },
            { 11, 3 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 60);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin
        int N = 7;
        std::vector<std::pair<int, int>> activities = {
            { 5, 4 },
            { 6, 7 },
            { 5, 2 },
            { 7, 2 },
            { 1, 5 },
            { 1, 7 },
            { 5, 7 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 22);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin
        int N = 6;
        std::vector<std::pair<int, int>> activities = {
            { 4, 2 },
            { 3, 4 },
            { 2, 5 },
            { 1, 6 },
            { 1, 3 },
            { 6, 2 },
            { 6, 5 },
            { 4, 5 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 24);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Kevin
        int N = 7;
        std::vector<std::pair<int, int>> activities = {
            { 7, 4 },
            { 7, 1 },
            { 2, 7 },
            { 7, 3 },
            { 4, 1 },
            { 6, 3 },
        };
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
        assert(answer_bruteforce.best_cost == 16);
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
    }
    {
        // Stress test
        for (int t = 0; t < 1000; t++) {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
            int N = int(rnd(mt) * (10 + 1 - 2)) + 2;                        // [2, 10]
            int M = int(rnd(mt) * 20) + 1;                                  // [1, 20]
            std::set<std::pair<int, int>> activity_collection;
            std::vector<std::pair<int, int>> activities;
            for (int m = 0; m < M; m++) {
                int coder1 = int(rnd(mt) * N) + 1;                          // [1, N]
                int coder2 = int(rnd(mt) * N) + 1;
                if (coder1 == coder2) continue;
                std::pair<int, int> activity = { coder1, coder2 };
                {
                    // No same activity (i.e. same coders pair)
                    bool is_exist1 = activity_collection.find({ coder1, coder2 }) != activity_collection.end(); if (is_exist1) continue;
                    bool is_exist2 = activity_collection.find({ coder2, coder1 }) != activity_collection.end(); if (is_exist2) continue;
                }
                activity_collection.insert(activity);
                activities.push_back(activity);
            }
            M = activities.size();
            SolutionBruteforce solution_bruteforce = SolutionBruteforce();
            SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(N, activities);
            SolutionDfs solution_efficient = SolutionDfs();
            SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
            assert(answer_efficient.best_cost == answer_bruteforce.best_cost);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int M; std::cin >> M;
        std::vector<std::pair<int, int>> activities;
        for (int m = 0; m < M; m++) {
            int coder1; std::cin >> coder1;
            int coder2; std::cin >> coder2;
            activities.push_back({ coder1, coder2 });
        }
        SolutionDfs solution_efficient = SolutionDfs();
        SolutionDfs::Answer answer_efficient = solution_efficient.solve(N, activities);
        std::cout << "Case #" << t + 1 << ": " << answer_efficient.best_cost << " ";
        for (int m = 0; m < M; m++) {
            std::cout << answer_efficient.best_schedules.at(m);
        }
        std::cout << std::endl;
    }
    return 0;
}
