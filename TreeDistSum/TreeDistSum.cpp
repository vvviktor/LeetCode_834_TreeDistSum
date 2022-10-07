#include<iostream>
#include<vector>
#include<memory>
#include<cassert>

class Solution {
    struct TreeNode {
        TreeNode(int id) : id(id) {}
        int id = 0;
        int subtree_size = 0;
        std::vector<TreeNode*> neighbours;
    };

public:
    std::vector<int> sumOfDistancesInTree(int n, std::vector<std::vector<int>>& edges) {
        tree_size_ = n;
        nodes_ = std::vector<std::unique_ptr<TreeNode>>(n);
        distances_ = std::vector<int>(n);
        if (edges.empty()) {
            return distances_;
        }
        BuildTree(edges);
        TraverseTree(nodes_[0].get(), nullptr);
        FinalPass(nodes_[0].get(), nullptr);
        return distances_;
    }

private:
    std::vector<std::unique_ptr<TreeNode>> nodes_;
    std::vector<int> distances_;
    int tree_size_ = 0;

    void BuildTree(std::vector<std::vector<int>>& edges) {
        for (const auto& edge : edges) {
            if (!nodes_[edge[0]]) {
                nodes_[edge[0]] = std::unique_ptr<TreeNode>(new TreeNode(edge[0]));
            }
            if (!nodes_[edge[1]]) {
                nodes_[edge[1]] = std::unique_ptr<TreeNode>(new TreeNode(edge[1]));
            }
            nodes_[edge[0]]->neighbours.push_back(nodes_[edge[1]].get());
            nodes_[edge[1]]->neighbours.push_back(nodes_[edge[0]].get());
        }
    }

    int TraverseTree(TreeNode* node, TreeNode* parent) {
        int subtree_sum = 0;
        node->subtree_size = parent ? node->neighbours.size() - 1 : node->neighbours.size();
        for (auto& next_node : node->neighbours) {
            if (next_node == parent) {
                continue;
            }
            subtree_sum += TraverseTree(next_node, node) + next_node->subtree_size + 1;
            node->subtree_size += next_node->subtree_size;
        }
        distances_[node->id] = subtree_sum;
        return subtree_sum;
    }

    void FinalPass(TreeNode* node, TreeNode* parent) {
        if (parent) {
            int uptree_sum = (distances_[parent->id] - 1) - (distances_[node->id] + node->subtree_size);
            distances_[node->id] += uptree_sum + ((tree_size_ - 1) - node->subtree_size);
        }
        for (auto& next_node : node->neighbours) {
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