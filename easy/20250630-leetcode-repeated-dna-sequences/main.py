# https://leetcode.com/problems/repeated-dna-sequences/

import collections


class Solution:
    def findRepeatedDnaSequences(self, s):
        return [k for k, v in collections.Counter([s[i: i + 10] for i in range(len(s) - 10 + 1)]).items() if v > 1]


solution = Solution()
assert solution.findRepeatedDnaSequences('AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT') == ['AAAAACCCCC', 'CCCCCAAAAA']
assert solution.findRepeatedDnaSequences('AAAAAAAAAAAAA') == ['AAAAAAAAAA']
assert solution.findRepeatedDnaSequences('AAAAAAAAAAA') == ['AAAAAAAAAA']
