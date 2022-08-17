// https://www.hackerrank.com/challenges/two-two/problem

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>


struct Node {
    bool is_end;
    std::map<int, Node*> next;
};
Node* graph = new Node;


int twoTwo(std::string a) {
    int count = 0;
    std::vector<Node*> traverses;
    std::reverse(a.begin(), a.end());
    for (char ch : a) {
        int i = ch - '0';
        std::vector<Node*> new_traverses;
        traverses.push_back(graph);
        for (Node* traverse : traverses) {
            bool is_exist = traverse->next.find(i) != traverse->next.end();
            if (! is_exist) {
                continue;
            }
            if (traverse->next[i]->is_end) {
                count++;
            }
            new_traverses.push_back(traverse->next[i]);
        }
        traverses = new_traverses;
    }
    return count;
}


void multiply(int x, std::vector<int>& num) {
    // Multiply num * x and store the result on num[]. Note num[] stores the numbers in reversed order.
    // Ref: https://www.geeksforgeeks.org/factorial-large-number/
    int carry = 0;
    for (int& i : num) {
        int prod = i * x + carry;
        i = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        num.push_back(carry % 10);
        carry /= 10;
    }
}


int main() {
    // Populate "graph"
    std::vector<int> num = {1};
    for (int x = 0; x <= 800; x++) {
        Node* traverse = graph;
        // num[] is already in reversed order
        for (int i : num) {
            bool is_exist = traverse->next.find(i) != traverse->next.end();
            if (! is_exist) {
                traverse->next[i] = new Node;
            }
            traverse = traverse->next[i];
        }
        traverse->is_end = true;
        multiply(2, num);
    }

    int T;
    std::cin >> T;
    while (T--) {
        std::string a;
        std::cin >> a;
        std::cout << twoTwo(a) << std::endl;
    }
    return 0;
}
