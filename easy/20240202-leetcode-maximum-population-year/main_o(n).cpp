// https://leetcode.com/problems/maximum-population-year/description/

#include <iostream>
#include <vector>


class Solution {
private:
    const int MAX_YEAR = 2050;
    const int UNDEFINED = -1;
    std::vector<int> population_diff;

public:
    int maximumPopulation(std::vector<std::vector<int>>& logs) {
        // Count population difference of the year
        population_diff = std::vector<int>(MAX_YEAR + 1);
        for (const std::vector<int>& log : logs) {
            int birth_year = log[0];
            int death_year = log[1];
            population_diff[birth_year]++;
            population_diff[death_year]--;
        }
        // Find the maximum population and the year it happened
        int population;
        int max_population = UNDEFINED;
        int min_year = UNDEFINED;
        for (int year = 0; year <= MAX_YEAR; year++) {
            population += population_diff[year];
            if (population > max_population) {
                max_population = population;
                min_year = year;
            }
        }
        return min_year;
    }
};


int main() {
    {
        Solution solution;
        std::vector<std::vector<int>> logs = {
            {1950, 1961}, {1960, 1971}, {1970, 1981}
        };
        int min_year = solution.maximumPopulation(logs);
        std::cout << min_year << std::endl;
    }
    return 0;
}
