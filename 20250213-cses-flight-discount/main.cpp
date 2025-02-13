// Flight Discount: https://cses.fi/problemset/task/1195

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

long long solve(int total_cities, const std::vector<Flight>& flights) {
    std::vector<long long> shortest_routes(total_cities + 1);
    const int ORIGIN = 1;
    const int DESTINATION = total_cities;
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
        long long max_flight_cost;
        bool operator<(const Queue& other) const {
            // Apply discounted price for the Priority Queue
            return total_cost - max_flight_cost + max_flight_cost/2 > other.total_cost - other.max_flight_cost + other.max_flight_cost/2;
        }
    };
    std::priority_queue<Queue> queues;
    for (const auto& kv : price_matrix[ORIGIN]) {
        int to = kv.first; long long price = kv.second;
        queues.push({
            to,
            price,
            price,
        });
    }

    // Process queues
    while (! queues.empty()) {
        Queue queue = queues.top(); queues.pop();
        if (shortest_routes[queue.last_city] != UNDEFINED && queue.total_cost >= shortest_routes[queue.last_city]) {
            // We have encountered this city before. Skip.
            continue;
        }
        if (queue.last_city == DESTINATION) {
            // We have reached destination. It must be the optimal path.
            long long discounted_cost = queue.total_cost - queue.max_flight_cost + queue.max_flight_cost/2;
            return discounted_cost;
        }
        // Note: The total cost to the last city might not be optimal!
        shortest_routes[queue.last_city] = queue.total_cost;
        // Add all eligible flights
        int from = queue.last_city;
        for (const auto& kv : price_matrix[from]) {
            int to = kv.first; long long price = kv.second;
            queues.push({
                to,
                queue.total_cost + price,
                std::max(queue.max_flight_cost, price),
            });
        }
    }
    return -1;
}


void test() {
    {
        int total_cities = 3;
        std::vector<Flight> flights = {
            {1, 2, 3},
            {2, 3, 1},
            {1, 3, 7},
            {2, 1, 5},
        };
        long long answer = solve(total_cities, flights);
        assert(answer == 2);
    }
    {
        int total_cities = 10;
        std::vector<Flight> flights = {
            {1, 2, 200},
            {2, 3, 199},
            {3, 4, 199},
            {4, 5, 199},
            {5, 10, 199},
            {1, 9, 10},
            {9, 8, 10},
            {8, 7, 20},
            {7, 10, 30},
            {1, 6, 100},
            {6, 10, 1},
        };
        long long answer = solve(total_cities, flights);
        assert(answer == 51);
    }
    {
        int total_cities = 4;
        std::vector<Flight> flights = {
            {1, 2, 3},
            {1, 3, 1},
            {3, 2, 1},
            {2, 4, 10},
        };
        long long answer = solve(total_cities, flights);
        assert(answer == 7);
    }
    {
        int total_cities = 5;
        std::vector<Flight> flights = {
            {1, 2, 20},
            {1, 3, 50},
            {2, 4, 20},
            {4, 3, 1},
            {3, 5, 100},
        };
        long long answer = solve(total_cities, flights);
        assert(answer == 91);
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
    long long answer = solve(N, flights);
    std::cout << answer << std::endl;
    return 0;
}
