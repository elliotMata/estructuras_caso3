#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Key
{
    int *position;
    string *filename;
    vector<string> *keywords;

    Key(int intValue, const string &strValue, vector<string> keysValue) : position(new int(intValue)), filename(new string(strValue)), keywords(new vector<string>(keysValue)) {}
    Key() : position(0), filename(new string()), keywords(new vector<string>()) {}
};

class TreeNode
{
    Key *keys;
    int t;
    TreeNode **C;
    int n;
    bool leaf;

public:
    TreeNode(int temp, bool bool_leaf);
    void insertNonFull(const Key &k);
    void splitChild(int i, TreeNode *y);
    void traverse();
    TreeNode *search(const Key &k);
    friend class BTree;
};

class BTree
{
    TreeNode *root;
    int t;

public:
    BTree(int temp)
    {
        root = NULL;
        t = temp;
    }

    void traverse()
    {
        if (root != NULL)
            root->traverse();
    }

    TreeNode *search(const Key &k)
    {
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(const Key &k);
};

TreeNode::TreeNode(int t1, bool leaf1)
{
    t = t1;
    leaf = leaf1;

    keys = new Key[2 * t - 1];
    C = new TreeNode *[2 * t];

    n = 0;
}

void TreeNode::traverse()
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            C[i]->traverse();
        cout << " (" << *keys[i].position << ", " << *keys[i].filename << ")" << endl;
    }

    if (leaf == false)
        C[i]->traverse();
}

TreeNode *TreeNode::search(const Key &k)
{
    int i = 0;
    while (i < n && (*k.position > *keys[i].position || (*k.position == *keys[i].position && *k.filename > *keys[i].filename)))
        i++;

    if (i < n && *keys[i].position == *k.position && *keys[i].filename == *k.filename)
        return this;

    if (leaf == true)
        return NULL;

    return C[i]->search(k);
}

void BTree::insert(const Key &k)
{
    if (root == NULL)
    {
        root = new TreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    }
    else
    {
        if (root->n == 2 * t - 1)
        {
            TreeNode *s = new TreeNode(t, false);

            s->C[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if ((s->keys[0].position < k.position) || (s->keys[0].position == k.position && s->keys[0].filename < k.filename))
                i++;
            s->C[i]->insertNonFull(k);

            root = s;
        }
        else
            root->insertNonFull(k);
    }
}

void TreeNode::insertNonFull(const Key &k)
{
    int i = n - 1;

    if (leaf == true)
    {
        while (i >= 0 && (keys[i].position > k.position || (keys[i].position == k.position && keys[i].filename > k.filename)))
        {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        n = n + 1;
    }
    else
    {
        while (i >= 0 && (keys[i].position > k.position || (keys[i].position == k.position && keys[i].filename > k.filename)))
            i--;

        if (C[i + 1]->n == 2 * t - 1)
        {
            splitChild(i + 1, C[i + 1]);

            if ((keys[i + 1].position < k.position) || (keys[i + 1].position == k.position && keys[i + 1].filename < k.filename))
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
}

void TreeNode::splitChild(int i, TreeNode *y)
{
    TreeNode *z = new TreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1;
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];
    n = n + 1;
}

#endif