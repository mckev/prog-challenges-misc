# https://leetcode.com/problems/n-queens/

class Solution:

    @staticmethod
    def board2Ans(board: list[list[bool]]) -> list[str]:
        return [''.join(['Q' if board[y][x] else '.' for x in range(len(board))]) for y in range(len(board))]

    @staticmethod
    def canPlaceQueen(board: list[list[bool]], y: int, x: int) -> bool:
        for i in range(len(board)):
            if board[i][x]: return False
            if board[y][i]: return False
        for diag in range(-len(board), len(board)):
            if 0 <= y + diag < len(board) and 0 <= x + diag < len(board) and board[y + diag][x + diag]:
                return False
            if 0 <= y + diag < len(board) and 0 <= x - diag < len(board) and board[y + diag][x - diag]:
                return False
        return True

    @staticmethod
    def placeQueen(board: list[list[bool]], x: int) -> list[list[str]]:
        if x == len(board):
            yield Solution.board2Ans(board)
            return
        for y in range(len(board)):
            if Solution.canPlaceQueen(board, y, x):
                board[y][x] = True
                yield from Solution.placeQueen(board, x + 1)
                board[y][x] = False

    def solveNQueens(self, n: int) -> list[list[str]]:
        board = [[False for x in range(n)] for y in range(n)]
        yield from Solution.placeQueen(board, 0)


solution = Solution()
answers = solution.solveNQueens(4)
print(list(answers))
