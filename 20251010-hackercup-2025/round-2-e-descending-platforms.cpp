// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-2/problems/E

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <vector>


class SolutionBruteforce {
private:
    std::vector<long long> best_configurations;
    long long best_count;

    void dfs(std::vector<long long>& configurations, int pos, const long long M, const std::vector<long long>& A) {
        if (pos >= A.size()) return;
        long long max_height = std::numeric_limits<long long>::max();
        if (pos > 0) max_height = configurations.at(pos - 1);
        for (int height = 0; height <= max_height; height++) {
            configurations.at(pos) = height;
            long long dot = std::inner_product(configurations.begin(), configurations.end(), A.begin(), 0LL);
            if (dot >= M) {
                long long count = std::accumulate(configurations.begin(), configurations.end(), 0LL);
                if (count < best_count) {
                    best_configurations = configurations;
                    best_count = count;
                }
                break;
            }
            dfs(configurations, pos + 1, M, A);
        }
        configurations.at(pos) = 0;
    }

public:
    struct Answer {
        std::vector<long long> best_configurations;
        long long best_count;
    };
    Answer solve(const long long M, const std::vector<long long>& A) {
        best_configurations = {};
        best_count = std::numeric_limits<long long>::max();
        std::vector<long long> configurations(A.size());
        dfs(configurations, 0, M, A);
        Answer answer = { best_configurations, best_count };
        return answer;
    }
};


class SolutionEfficient {
private:
    struct Spec {
        long long a;
        int pos;                        // [1, ...
    };

    long long ceil_div(long long a, long long b) {
        long long q = a / b;
        long long r = a % b;
        // If there is a remainder, the quotient should round upward
        if ((r != 0) && ((a > 0) == (b > 0)))
            q++;
        return q;
    }

public:
    struct Answer {
        std::vector<long long> best_configurations;
        long long best_count;
    };
    Answer solve(const long long M, const std::vector<long long>& A) {
        std::vector<Spec> specs;
        {
            long long a = 0;
            for (int i = 0; i < A.size(); i++) {
                a += A.at(i);
                Spec spec = { a, i + 1 };
                specs.push_back(spec);
            }
        }
        Spec best_spec = *std::max_element(specs.begin(), specs.end(), [](const Spec& x, const Spec& y) {
            // Get the largest a/pos descending as primary, and pos ascending as secondary
            long double rx = (long double) x.a / x.pos;
            long double ry = (long double) y.a / y.pos;
            return std::tuple(rx, -x.pos) < std::tuple(ry, -y.pos);
        });

        // For every bricks from 1 to A.size() * A.size(), find its maximum amazingness.
        // Theory: We only need to find maximum amazingness from 1 to N * N using Dynamic Programming. Then we can project to the goal M using best spec.
        std::vector<long long> max_amazingness(A.size() * A.size() + 1);
        // Also record configurations for each bricks from 1 to A.size() * A.size(). Here brick's index starts at 0 so we need to always reduce spec.pos by 1.
        std::vector<std::vector<long long>> configurations(A.size() * A.size() + 1, std::vector<long long>(A.size()));
        for (int i = 1; i <= A.size() * A.size(); i++) {
            for (const Spec& spec : specs) {
                // max_amazingness.at(i) = std::max(max_amazingness.at(i), max_amazingness.at(i - spec.pos) + spec.a);
                if (i - spec.pos >= 0 && max_amazingness.at(i - spec.pos) + spec.a > max_amazingness.at(i)) {
                    max_amazingness.at(i) = max_amazingness.at(i - spec.pos) + spec.a;
                    configurations.at(i) = configurations.at(i - spec.pos); configurations.at(i).at(spec.pos - 1)++;
                }
            }
        }

        // Now use best_spec to project required bricks to M
        long long best_count = std::numeric_limits<long long>::max();
        std::vector<long long> best_configurations;
        for (int i = 1; i <= A.size() * A.size(); i++) {
            long long remaining_amazingness = M - max_amazingness.at(i);
            long long count = i + (remaining_amazingness <= 0 ? 0 : ceil_div(remaining_amazingness, best_spec.a) * best_spec.pos);
            if (count < best_count) {
                best_count = count;
                best_configurations = configurations.at(i);
                best_configurations.at(best_spec.pos - 1) += remaining_amazingness <= 0 ? 0 : ceil_div(remaining_amazingness, best_spec.a);     // project to M
            }
        }

        // We just record the count of brick index. Now count the actual bricks.
        for (int i = A.size() - 2; i >= 0; i--) {
            best_configurations.at(i) += best_configurations.at(i + 1);
        }

        // Verify
        long long amazingness = 0;
        for (int i = 0; i < A.size(); i++) {
            amazingness += A.at(i) * best_configurations.at(i);
        }
        assert(amazingness >= M);

        Answer answer = { best_configurations, best_count };
        return answer;
    }
};


class SolutionUmnik {
// https://youtu.be/w3qx7fTyzjw?si=OlJxIutFROgiY7yI&t=4837
private:
    const long long INF = (long long) 1e16;

    long long ceil_div(long long x, long long y) {
        assert(y != 0);
        if (y < 0) {
            y = -y;
            x = -x;
        }
        if (x <= 0) return x / y;
        return (x - 1) / y + 1;
    }

public:
    struct Answer {
        std::vector<long long> best_configurations;
        long long best_count;
    };
    Answer solve(const long long M, const std::vector<long long>& _A) {
        std::vector<long long> A = _A;
        for (int i = 1; i < A.size(); i++) {
            A[i] += A[i - 1];
        }
        int best = 0;
        for (int i = 0; i < A.size(); i++) {
            if ((long double) A[i] / (i + 1) > (long double) A[best] / (best + 1)) {
                best = i;
            }
        }

        std::vector<long long> dp(A.size() * A.size() * A.size() + 1, -INF);    // dp[w] = max total value achievable using exactly w bricks
        std::vector<int> par(A.size() * A.size() * A.size() + 1);
        dp[0] = 0;
        for (int w = 0; w < A.size() * A.size(); w++) {
            if (dp[w] < 0) continue;
            for (int i = 0; i < A.size(); i++) {
                if (dp[w] + A[i] <= dp.at(w + i + 1)) continue;
                dp.at(w + i + 1) = dp[w] + A[i];
                par.at(w + i + 1) = i;
            }
        }

        Answer answer;

        // Calculate minimum number of bricks
        long long best_val = INF;
        int W = -1;
        for (int w = 0; w <= A.size() * A.size(); w++) {
            if (dp[w] < 0) continue;
            long long cur = w;
            long long lft = M - dp.at(w);
            if (lft >= 0) {
                cur += ceil_div(lft, A[best]) * (best + 1);
            }
            if (cur < best_val) {
                best_val = cur;
                W = w;
            }
        }
        answer.best_count = best_val;

        // Calculate bricks configuration
        std::vector<long long> answers(A.size());
        long long lft = M - dp.at(W);
        if (lft >= 0) {
            lft = ceil_div(lft, A[best]);
        } else {
            lft = 0;
        }
        answers[best] += lft;
        while (W > 0) {
            int p = par.at(W);
            W -= p + 1;
            answers.at(p)++;
        }
        for (int i = A.size() - 2; i >= 0; i--) {
            answers[i] += answers[i + 1];
        }
        answer.best_configurations = answers;

        // Verify
        long long amazingness = 0;
        for (int i = 0; i < A.size(); i++) {
            amazingness += A.at(i) * answer.best_configurations.at(i);
        }
        assert(amazingness >= M);

        return answer;
    }
};


void test() {
    {
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long M = 27;
        std::vector<long long> A = { 3, 5, 1 };
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
        assert(answer_bruteforce.best_count == 7);
        assert(answer_bruteforce.best_configurations == (std::vector<long long>{ 4, 3, 0 }));
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        assert(answer_efficient.best_count == answer_bruteforce.best_count);
        assert(answer_efficient.best_configurations == answer_bruteforce.best_configurations);
        SolutionUmnik solution_umnik = SolutionUmnik();
        SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
        assert(answer_umnik.best_count == answer_bruteforce.best_count);
        assert(answer_umnik.best_configurations == answer_bruteforce.best_configurations);
    }
    {
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long M = 18;
        std::vector<long long> A = { 1, 9, 6 };
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
        assert(answer_bruteforce.best_count == 4);
        assert(answer_bruteforce.best_configurations == (std::vector<long long>{ 2, 2, 0 }));
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        assert(answer_efficient.best_count == answer_bruteforce.best_count);
        assert(answer_efficient.best_configurations == answer_bruteforce.best_configurations);
        SolutionUmnik solution_umnik = SolutionUmnik();
        SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
        assert(answer_umnik.best_count == answer_bruteforce.best_count);
        assert(answer_umnik.best_configurations == answer_bruteforce.best_configurations);
    }
    {
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long M = 28;
        std::vector<long long> A = { 3, 5, 1 };
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
        assert(answer_bruteforce.best_count == 8);
        assert(answer_bruteforce.best_configurations == (std::vector<long long>{ 4, 3, 1 }));
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        assert(answer_efficient.best_count == answer_bruteforce.best_count);
        assert(answer_efficient.best_configurations == (std::vector<long long>{ 4, 4, 0 }));
        SolutionUmnik solution_umnik = SolutionUmnik();
        SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
        assert(answer_umnik.best_count == answer_bruteforce.best_count);
        assert(answer_umnik.best_configurations == (std::vector<long long>{ 4, 4, 0 }));
    }
    {
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long M = 100;
        std::vector<long long> A = { 7, 6, 16, 9, 3 };
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
        assert(answer_bruteforce.best_count == 11);
        assert(answer_bruteforce.best_configurations == (std::vector<long long>{ 3, 3, 3, 2, 0 }));
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        assert(answer_efficient.best_count == answer_bruteforce.best_count);
        assert(answer_efficient.best_configurations == (std::vector<long long>{ 5, 3, 3, 0, 0 }));
        SolutionUmnik solution_umnik = SolutionUmnik();
        SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
        assert(answer_umnik.best_count == answer_bruteforce.best_count);
        assert(answer_umnik.best_configurations == (std::vector<long long>{ 5, 3, 3, 0, 0 }));
    }
    {
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long M = 45;
        std::vector<long long> A = { 1, 2, 4, 8, 1, 2, 60, 3, 5, 7 };
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
        assert(answer_bruteforce.best_count == 7);
        assert(answer_bruteforce.best_configurations == (std::vector<long long>{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 }));
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        assert(answer_efficient.best_count == answer_bruteforce.best_count);
        assert(answer_efficient.best_configurations == answer_bruteforce.best_configurations);
        SolutionUmnik solution_umnik = SolutionUmnik();
        SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
        assert(answer_umnik.best_count == answer_bruteforce.best_count);
        assert(answer_umnik.best_configurations == answer_bruteforce.best_configurations);
    }
    {
        SolutionBruteforce solution_bruteforce = SolutionBruteforce();
        long long M = 230;
        std::vector<long long> A = { 5, 11, 12, 15, 13, 7, 23, 7 };
        SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
        assert(answer_bruteforce.best_count == 20);
        assert(answer_bruteforce.best_configurations == (std::vector<long long>{ 3, 3, 3, 3, 3, 2, 2, 1 }));
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        assert(answer_efficient.best_count == answer_bruteforce.best_count);
        assert(answer_efficient.best_configurations == (std::vector<long long>{ 3, 3, 3, 3, 3, 3, 2, 0 }));
        SolutionUmnik solution_umnik = SolutionUmnik();
        SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
        assert(answer_umnik.best_count == answer_bruteforce.best_count);
        assert(answer_umnik.best_configurations == (std::vector<long long>{ 3, 3, 3, 3, 3, 3, 2, 0 }));
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);             // [0, 1)
        for (int t = 0; t < 100; t++) {
            int N = int(rnd(mt) * 10) + 1;                      // [1, 10]
            std::vector<long long> A;
            for (int n = 0; n < N; n++) {
                int a = int(rnd(mt) * 20) + 1;
                A.push_back(a);
            }
            int M = int(rnd(mt) * 500) + 1;
            SolutionBruteforce solution_bruteforce = SolutionBruteforce();
            SolutionBruteforce::Answer answer_bruteforce = solution_bruteforce.solve(M, A);
            SolutionEfficient solution_efficient = SolutionEfficient();
            SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
            assert(answer_efficient.best_count == answer_bruteforce.best_count);
            SolutionUmnik solution_umnik = SolutionUmnik();
            SolutionUmnik::Answer answer_umnik = solution_umnik.solve(M, A);
            assert(answer_umnik.best_count == answer_bruteforce.best_count);
        }
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        long long M; std::cin >> M;
        std::vector<long long> A;
        for (int n = 0; n < N; n++) {
            long long a; std::cin >> a;
            A.push_back(a);
        }
        assert(N == A.size());
        SolutionEfficient solution_efficient = SolutionEfficient();
        SolutionEfficient::Answer answer_efficient = solution_efficient.solve(M, A);
        std::cout << "Case #" << t + 1 << ": " << answer_efficient.best_count << std::endl;
        for (long long configuration : answer_efficient.best_configurations) {
            std::cout << configuration << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
