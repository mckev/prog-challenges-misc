class Solution:
    def reverseBits(self, n: int) -> int:
        return int(f'{n:032b}'[::-1],2)


solution = Solution()
answer = solution.reverseBits(43261596)
assert answer == 964176192
answer = solution.reverseBits(4294967293)
assert answer == 3221225471
