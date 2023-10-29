#include <iostream>

template <typename T>
class AVLTreeNode
{
public:
    T data;
    AVLTreeNode *left;
    AVLTreeNode *right;
    int height;

    AVLTreeNode(T value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree
{
private:
    AVLTreeNode<T> *root;

    int getHeight(AVLTreeNode<T> *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalanceFactor(AVLTreeNode<T> *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(AVLTreeNode<T> *node)
    {
        if (node != nullptr)
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    AVLTreeNode<T> *rotateRight(AVLTreeNode<T> *y)
    {
        AVLTreeNode<T> *x = y->left;
        AVLTreeNode<T> *T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVLTreeNode<T> *rotateLeft(AVLTreeNode<T> *x)
    {
        AVLTreeNode<T> *y = x->right;
        AVLTreeNode<T> *T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVLTreeNode<T> *balance(AVLTreeNode<T> *node)
    {
        updateHeight(node);

        int balance = getBalanceFactor(node);

        if (balance > 1)
        {
            if (getBalanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1)
        {
            if (getBalanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLTreeNode<T> *insert(AVLTreeNode<T> *node, T value)
    {
        if (node == nullptr)
            return new AVLTreeNode<T>(value);

        if (value < node->data)
            node->left = insert(node->left, value);
        else if (value > node->data)
            node->right = insert(node->right, value);
        else
            return node; // Duplicates not allowed

        return balance(node);
    }

    void inOrderTraversal(AVLTreeNode<T> *node)
    {
        if (node != nullptr)
        {
            inOrderTraversal(node->left);
            std::cout << node->data << " ";
            inOrderTraversal(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T value)
    {
        root = insert(root, value);
    }

    void inOrderTraversal()
    {
        inOrderTraversal(root);
    }
};