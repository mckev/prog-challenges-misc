# https://leetcode.com/problems/n-queens/

import copy


class Solution:

    @staticmethod
    def board2Ans(board: list[list[bool]]) -> list[str]:
        return [''.join(['Q' if board[y][x] else '.' for x in range(len(board))]) for y in range(len(board))]

    @staticmethod
    def canPlaceQueen(board: list[list[bool]], y: int, x: int) -> bool:
        for i in range(len(board)):
            if board[i][x]:
                return False
            if board[y][i]:
                return False
        for diag in range(-len(board), len(board)):
            if 0 <= y + diag < len(board) and 0 <= x + diag < len(board) and board[y + diag][x + diag]:
                return False
            if 0 <= y + diag < len(board) and 0 <= x - diag < len(board) and board[y + diag][x - diag]:
                return False
        return True

    def solveNQueens(self, n: int) -> list[list[str]]:
        data = {
            'board': [[False for x in range(n)] for y in range(n)],
            'col': 0
        }
        q = []
        q.append(data)
        while q:
            data = q.pop()
            if data['col'] == n:
                yield Solution.board2Ans(data['board'])
                continue
            for row in range(n):
                if Solution().canPlaceQueen(data['board'], row, data['col']):
                    new_board = copy.deepcopy(data['board'])
                    new_board[row][data['col']] = True
                    new_data = {
                        'board': new_board,
                        'col': data['col'] + 1
                    }
                    q.append(new_data)


solution = Solution()
answers = solution.solveNQueens(4)
print(list(answers))
