#include <cassert>
#include <iostream>
#include <random>


long long calculateK(long long S, long long D) {
    long long buns = S*2 + D*2;
    long long patties = S + D*2;
    long long K = std::min(patties, buns > 0 ? buns - 1 : 0);
    return K;
}


long long solution(long long A, long long B, long long C) {
    // From following:
    //   - K is determined by minimum of number of patties and (buns - 1).
    //   - Number of buns are equal for single and double.
    //   - Number of patties of double is 2x of single.
    // So:
    //   - If the price of double is > 2x single, then buy all single (i.e. maximize single).
    //   - If the price of double is <= single, then buy all double (i.e. maximize double).

    if (B > 2 * A) {
        // Strategy 1: Maximize Single
        long long S = C / A;
        C -= S * A;
        long long D = C / B;
        long long K = calculateK(S, D);
        return K;
    } else if (B <= A) {
        // Strategy 2: Maximize Double
        long long D = C / B;
        C -= D * B;
        long long S = C / A;
        long long K = calculateK(S, D);
        return K;
    } else {
        // Strategy 3: Maximize Double, however check Single of 0, 1, or 2
        long long OriginalC = C;
        long long K = 0;
        for (int S = 0; S <= 2; S++) {
            long long C = OriginalC;
            C -= S * A;
            if (C < 0) continue;
            long long D = C / B;
            K = std::max(K, calculateK(S, D));
        }
        return K;
    }
}


long long solution2(long long A, long long B, long long OriginalC) {
    long long BestK = 0;
    for (long long D = 0; D <= OriginalC / B; D++) {
        long long C = OriginalC;
        C -= D * B;
        long long S = C / A;
        long long K = calculateK(S, D);
        BestK = std::max(BestK, K);
    }
    return BestK;
}


int main() {
    {
        assert(solution(2, 3, 5) == 3);
        assert(solution(2, 3, 2) == 1);
        assert(solution(2, 3, 1) == 0);
        assert(solution(5, 1, 100) == 199);
        assert(solution(1, 3, 100) == 100);
        assert(solution(1, 1, 1000000000000) == 1999999999999);
        assert(solution(314, 512, 133769420) == 522536);
        assert(solution2(314, 512, 133769420) == 522536);
        assert(solution(5, 9, 9152845331581750) == 2033965629240388);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 1000000; T++) {
            long long A = int(rnd(mt) * 10000) + 1;
            long long B = int(rnd(mt) * 10000) + 1;
            long long C = int(rnd(mt) * 1000000) + 1;
            long long answer = solution(A, B, C);
            long long answer2 = solution2(A, B, C);
            assert(answer == answer2);
        }
    }

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        long long A, B, C; std::cin >> A >> B >> C;
        long long answer = solution(A, B, C);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
