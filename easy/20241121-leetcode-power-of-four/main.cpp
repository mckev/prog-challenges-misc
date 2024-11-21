// https://leetcode.com/problems/power-of-two/description/

#include <cassert>
#include <cmath>


class SolutionBitwise {
public:
    bool isPowerOfFour(long long n) {
        return (n > 0) && !(n & 0b10101010101010101010101010101010) && !(n & (n - 1));
    }
};


class SolutionMath {
public:
    const double EPSILON = 0.0000000001;

    bool isPowerOfFour(long long n) {
        if (n <= 0) return false;
        // n = 4^p
        // log n = p log 4
        // p = log n / log 4
        double p = std::log(n) / std::log(4);
        // Check if p is an integer
        bool isInteger = std::abs(p - int(p)) < EPSILON;
        return isInteger;
    }
};


class SolutionRecursive {
public:
    bool isPowerOfFour(long long n) {
        if (n == 0) return false;
        if (n == 1) return true;
        if (n % 4 != 0) return false;
        return isPowerOfFour(n / 4);
    }
};


int main() {
    for (int n = -1000000; n < 1000000; n++) {
        SolutionBitwise solutionBitwise = SolutionBitwise();
        SolutionMath solutionMath = SolutionMath();
        SolutionRecursive solutionRecursive = SolutionRecursive();
        bool answerBitwise = solutionBitwise.isPowerOfFour(n);
        bool answerMath = solutionMath.isPowerOfFour(n);
        bool answerRecursive = solutionRecursive.isPowerOfFour(n);
        assert(answerBitwise == answerRecursive);
        assert(answerMath == answerRecursive);
    }
    return 0;
}
