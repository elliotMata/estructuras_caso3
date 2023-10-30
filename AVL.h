#ifndef AVL_H
#define AVL_H

#include <iostream>

template <typename T>
class AVLTreeNode
{
public:
    T data;
    AVLTreeNode *left;
    AVLTreeNode *right;
    int height;
    std::vector<std::string> filenames; // Store associated filenames

    AVLTreeNode(T value, const std::string &filename) : data(value), left(nullptr), right(nullptr), height(1)
    {
        filenames.push_back(filename); // Initialize the filenames list with the first filename
    }
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

    AVLTreeNode<T> *insert(AVLTreeNode<T> *node, T value, const std::string &filename)
    {
        if (node == nullptr)
        {
            AVLTreeNode<T> *newNode = new AVLTreeNode<T>(value, filename);
            // newNode->filenames.push_back(filename);
            return newNode;
        }

        if (value < node->data)
        {
            node->left = insert(node->left, value, filename);
        }
        else if (value > node->data)
        {
            node->right = insert(node->right, value, filename);
        }
        else
        {
            // Keyword already exists, add the filename to the list
            node->filenames.push_back(filename);
            return node; // Duplicates not allowed
        }

        return balance(node);
    }

    void inOrderTraversal(AVLTreeNode<T> *node)
    {
        if (node != nullptr)
        {
            inOrderTraversal(node->left);

            std::cout << "Keyword: " << node->data << std::endl;
            std::cout << "Filenames: ";
            for (const std::string &filename : node->filenames)
            {
                std::cout << filename << " ";
            }
            std::cout << std::endl;

            inOrderTraversal(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T value, string filename)
    {
        root = insert(root, value, filename);
    }

    void inOrderTraversal()
    {
        inOrderTraversal(root);
    }
};

#endif