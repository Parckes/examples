#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cinttypes>

using namespace std;

struct Node
{
    int value;
    Node* left;
    Node* right;
    //Node* parrent;

    Node (int x) : value(x), left(nullptr), right(nullptr) {}
};

void insert(Node*& root, int x)
{
    if (root == nullptr)
    {
        root = new Node(x);
        //root->parrent = parr;
        //printf("AA%dAA\n", root == nullptr);
    }
    if (root->value == x)
        return;
    if (x < root->value)
        insert(root->left, x);
    else
        insert(root->right, x);
}

int find_height(Node* root)
{
    if (!root)
        return 0;
    if (!root->left && !root->right)
        return 1;
    return 1 + max(find_height(root->left), find_height(root->right));
}

void print(Node* root)
{
    if(root->left)
        print(root->left);
    printf("%d\n", root->value);
    if(root->right)
        print(root->right);
}

Node* max(Node* tree)
{
    if (tree->right)
        return max(tree->right);
    return tree;
}

int second_max(Node* tree, vector<int>&  vec)
{
    while (tree->right == tree)
    {
        if (!tree->right->right)
        {
            if (tree->right->left)
                return max(tree->value, tree->right->left->value);
            return tree->value;
        }
        tree = tree->right;
    }
    sort(vec.begin(), vec.end());
    int i = vec.size();
    while(vec[i - 2] == vec[i - 1])
        --i;
    return vec[i - 2];
}

void clean(Node* root)
{
    if (root->left)
        clean(root->left);
    if (root->right)
        clean(root->right);
    delete root;
}

int main()
{
    Node* tree = nullptr;
    int height = 0;
    int a;
    vector<int> ra;
    scanf ("%d", &a);
    while (a)
    {
        ra.push_back(a);
        insert(tree, a);
        scanf ("%d", &a);
    }
    // height = find_height(tree);
    // print(tree);

    printf("%d\n", second_max(tree, ra));
    clean(tree);
    return 0;
}
