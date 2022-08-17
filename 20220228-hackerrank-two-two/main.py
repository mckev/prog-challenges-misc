#!/usr/bin/env python3

# https://www.hackerrank.com/challenges/two-two/problem


class Node:
    def __init__(self):
        self.is_end : bool = False
        self.next : dict[int, Node] = {}
graph = Node()


def twoTwo(a):
    count = 0
    traverses = []
    for ch in reversed(str(a)):
        i = int(ch)
        new_traverses = []
        for traverse in traverses + [graph]:
            if i not in traverse.next:
                continue
            if traverse.next[i].is_end:
                count += 1
            new_traverses.append(traverse.next[i])
        traverses = new_traverses
    return count


def main():
    # Populate "graph"
    for i in range(801):
        num = 2 ** i
        traverse = graph
        for ch in reversed(str(num)):
            i = int(ch)
            if i not in traverse.next:
                traverse.next[i] = Node()
            traverse = traverse.next[i]
        traverse.is_end = True

    T = int(input())
    for _ in range(T):
        a = input()
        print(twoTwo(a))


if __name__ == '__main__':
    main()
