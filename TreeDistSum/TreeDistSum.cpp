#include<iostream>
#include<vector>
#include<cassert>

class Solution {
    struct TreeNode {
        int subtree_size = 0;
        std::vector<int> neighbours;
    };

public:
    std::vector<int> sumOfDistancesInTree(int n, std::vector<std::vector<int>>& edges) {
        tree_size_ = n;
        nodes_.resize(n);
        distances_.resize(n);
        if (edges.empty()) {
            return distances_;
        }
        BuildTree(edges);
        TraverseTree(0, -1);
        FinalPass(0, -1);
        return distances_;
    }

private:
    std::vector<TreeNode> nodes_;
    std::vector<int> distances_;
    int tree_size_ = 0;

    void BuildTree(std::vector<std::vector<int>>& edges) {
        for (const auto& edge : edges) {
            nodes_[edge[0]].neighbours.push_back(edge[1]);
            nodes_[edge[1]].neighbours.push_back(edge[0]);
        }
    }

    int TraverseTree(int node, int parent) {
        int subtree_sum = 0;
        nodes_[node].subtree_size = parent != -1 ? nodes_[node].neighbours.size() - 1 : nodes_[node].neighbours.size();
        for (int next_node : nodes_[node].neighbours) {
            if (next_node == parent) {
                continue;
            }
            subtree_sum += TraverseTree(next_node, node) + nodes_[next_node].subtree_size + 1;
            nodes_[node].subtree_size += nodes_[next_node].subtree_size;
        }
        distances_[node] = subtree_sum;
        return subtree_sum;
    }

    void FinalPass(int node, int parent) {
        if (parent != -1) {
            int uptree_sum = (distances_[parent] - 1) - (distances_[node] + nodes_[node].subtree_size);
            distances_[node] += uptree_sum + ((tree_size_ - 1) - nodes_[node].subtree_size);
        }
        for (int next_node : nodes_[node].neighbours) {
            if (next_node == parent) {
                continue;
            }
            FinalPass(next_node, node);
        }
    }
};

int main() {
    std::vector<std::vector<int>> sample_tree{{0, 1}, {0, 2}, {2, 3}, {2, 4}, {2, 5}};
    Solution s;
    std::vector<int> result = s.sumOfDistancesInTree(6, sample_tree);
    std::vector<int> expected{17, 0, 9, 0, 0, 3, 0, 0, 0};
    //assert(result == expected);
    return 0;
}