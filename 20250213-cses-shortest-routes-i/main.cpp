// Shortest Routes I: https://cses.fi/problemset/task/1671

#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <vector>


struct Flight {
    int from;
    int to;
    long long price;
};

std::vector<long long> solve(int total_cities, const std::vector<Flight>& flights) {
    std::vector<long long> shortest_routes(total_cities + 1);
    const int ORIGIN = 1;
    const int UNDEFINED = 0;

    // Firstly generate matrix of lowest price between cities
    std::vector<std::map<int, long long>> price_matrix(total_cities + 1);
    for (int flight_no = 0; flight_no < (int) flights.size(); flight_no++) {
        Flight flight = flights[flight_no];
        if (price_matrix[flight.from][flight.to] == UNDEFINED) {
            price_matrix[flight.from][flight.to] = flight.price;
        } else {
            price_matrix[flight.from][flight.to] = std::min(price_matrix[flight.from][flight.to], flight.price);
        }
    }
    // Don't visit to the same city
    for (int to = 1; to <= total_cities; to++) {
        price_matrix[to].erase(to);
    }
    // Don't visit ORIGIN
    for (int from = 1; from <= total_cities; from++) {
        price_matrix[from].erase(ORIGIN);
    }

    // Initialize queues
    struct Queue {
        int last_city;
        long long total_cost;
        bool operator<(const Queue& other) const {
            return total_cost > other.total_cost;
        }
    };
    std::priority_queue<Queue> queues;
    for (const auto& kv : price_matrix[ORIGIN]) {
        int to = kv.first; long long price = kv.second;
        queues.push({
            to,
            price,
        });
    }

    // Process queues
    while (! queues.empty()) {
        Queue queue = queues.top(); queues.pop();
        if (shortest_routes[queue.last_city] != UNDEFINED) {
            // We have encountered this city before. Skip.
            continue;
        }
        // The total cost to the last city is optimal
        shortest_routes[queue.last_city] = queue.total_cost;
        // Add all eligible flights
        int from = queue.last_city;
        for (const auto& kv : price_matrix[from]) {
            int to = kv.first; long long price = kv.second;
            queues.push({
                to,
                queue.total_cost + price,
            });
        }
    }
    shortest_routes.erase(shortest_routes.begin());
    return shortest_routes;
}


void test() {
    {
        int total_cities = 3;
        std::vector<Flight> flights = {
            {1, 2, 6},
            {1, 3, 2},
            {3, 2, 3},
            {1, 3, 4},
        };
        std::vector<long long> answer_priority_queue = solve(total_cities, flights);
        std::vector<long long> expected = {0, 5, 2};
        assert(answer_priority_queue == expected);
    }
    {
        int total_cities = 10;
        std::vector<Flight> flights = {
            {8, 5, 1},
            {9, 10, 2},
            {7, 9, 8},
            {9, 8, 8},
            {10, 9, 9},
            {7, 8, 10},
            {8, 9, 2},
            {7, 10, 10},
            {4, 5, 8},
            {5, 6, 1},
            {4, 2, 1},
            {5, 3, 6},
            {10, 7, 3},
            {3, 5, 2},
            {5, 4, 7},
            {1, 2, 9},
            {2, 3, 2},
            {6, 7, 5},
            {3, 4, 10},
            {3, 2, 10},          
        };
        std::vector<long long> answer_priority_queue = solve(total_cities, flights);
        std::vector<long long> expected = {0, 9, 11, 20, 13, 14, 19, 29, 27, 29};
        assert(answer_priority_queue == expected);
    }
    {
        int total_cities = 10;
        std::vector<Flight> flights = {
            {5, 6, 4},
            {5, 1, 7},
            {7, 4, 4},
            {7, 8, 1},
            {8, 9, 3},
            {5, 7, 6},
            {2, 3, 2},
            {1, 2, 7},
            {7, 9, 5},
            {2, 5, 8},
            {8, 5, 7},
            {4, 5, 6},
            {6, 7, 6},
            {8, 7, 6},
            {6, 2, 2},
            {3, 6, 8},
            {6, 4, 7},
            {9, 10, 5},
            {5, 2, 1},
            {3, 4, 8},          
        };
        std::vector<long long> answer_priority_queue = solve(total_cities, flights);
        std::vector<long long> expected = {0, 7, 9, 17, 15, 17, 21, 22, 25, 30};
        assert(answer_priority_queue == expected);
    }
}


int main() {
    // test();
    int N; std::cin >> N;
    int M; std::cin >> M;
    std::vector<Flight> flights;
    for (int m = 0; m < M; m++) {
        int a; std::cin >> a;
        int b; std::cin >> b;
        long long c; std::cin >> c;
        flights.push_back({a, b, c});
    }
    std::vector<long long> answer_priority_queue = solve(N, flights);
    for (long long cost : answer_priority_queue) {
        std::cout << cost << " ";
    }
    std::cout << std::endl;
    return 0;
}
