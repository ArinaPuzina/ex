#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// Узел дерева
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Построение графа из бинарного дерева
void buildGraph(TreeNode *node, unordered_map<int, vector<int>> &graph, TreeNode *parent = nullptr)
{
    if (!node)
        return;

    if (parent)
    {
        graph[node->val].push_back(parent->val);
        graph[parent->val].push_back(node->val);
    }
    buildGraph(node->left, graph, node);
    buildGraph(node->right, graph, node);
}

// BFS для подсчета минимального времени
int burnTree(unordered_map<int, vector<int>> &graph, int target)
{
    queue<int> q;
    unordered_set<int> visited;

    q.push(target);
    visited.insert(target);

    int time = 0;

    while (!q.empty())
    {
        int size = q.size();
        bool burned = false;

        for (int i = 0; i < size; ++i)
        {
            int node = q.front();
            q.pop();

            for (int neighbor : graph[node])
            {
                if (visited.find(neighbor) == visited.end())
                {
                    q.push(neighbor);
                    visited.insert(neighbor);
                    burned = true;
                }
            }
        }

        if (burned)
            time++;
    }

    return time;
}

int minTimeToBurnTree(TreeNode *root, int target)
{
    unordered_map<int, vector<int>> graph;
    buildGraph(root, graph);
    return burnTree(graph, target);
}

// Тестирование
int main()
{
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    root->left->right->left = new TreeNode(7);
    root->left->right->right = new TreeNode(8);
    root->right->right->right = new TreeNode(9);
    root->right->right->right->right = new TreeNode(10);

    int target = 8;
    cout << "Минимальное время для сгорания дерева: " << minTimeToBurnTree(root, target) << endl;

    return 0;
}