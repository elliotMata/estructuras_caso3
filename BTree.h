#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <string>
using namespace std;

// Define a structure for the key
struct Key
{
    int integerValue;
    string stringValue;

    Key(int intValue, const string &strValue) : integerValue(intValue), stringValue(strValue) {}
    Key() : integerValue(0), stringValue("") {}
};

class TreeNode
{
    Key *keys; // Replace int* with Key*
    int t;
    TreeNode **C;
    int n;
    bool leaf;

public:
    TreeNode(int temp, bool bool_leaf);
    void insertNonFull(const Key &k); // Modify the parameter type
    void splitChild(int i, TreeNode *y);
    void traverse();
    TreeNode *search(const Key &k); // Modify the parameter type
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
    { // Modify the parameter type
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(const Key &k); // Modify the parameter type
};

TreeNode::TreeNode(int t1, bool leaf1)
{
    t = t1;
    leaf = leaf1;

    keys = new Key[2 * t - 1]; // Replace int* with Key*
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
        cout << " (" << keys[i].integerValue << ", " << keys[i].stringValue << ")";
    }

    if (leaf == false)
        C[i]->traverse();
}

TreeNode *TreeNode::search(const Key &k)
{
    int i = 0;
    while (i < n && (k.integerValue > keys[i].integerValue || (k.integerValue == keys[i].integerValue && k.stringValue > keys[i].stringValue)))
        i++;

    if (i < n && keys[i].integerValue == k.integerValue && keys[i].stringValue == k.stringValue)
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
            if ((s->keys[0].integerValue < k.integerValue) || (s->keys[0].integerValue == k.integerValue && s->keys[0].stringValue < k.stringValue))
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
        while (i >= 0 && (keys[i].integerValue > k.integerValue || (keys[i].integerValue == k.integerValue && keys[i].stringValue > k.stringValue)))
        {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        n = n + 1;
    }
    else
    {
        while (i >= 0 && (keys[i].integerValue > k.integerValue || (keys[i].integerValue == k.integerValue && keys[i].stringValue > k.stringValue)))
            i--;

        if (C[i + 1]->n == 2 * t - 1)
        {
            splitChild(i + 1, C[i + 1]);

            if ((keys[i + 1].integerValue < k.integerValue) || (keys[i + 1].integerValue == k.integerValue && keys[i + 1].stringValue < k.stringValue))
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

/* int main()
{
    BTree t(3);
    t.insert(Key(8, "Eight"));
    t.insert(Key(9, "Nine"));
    t.insert(Key(10, "Ten"));
    t.insert(Key(11, "Eleven"));
    t.insert(Key(15, "Fifteen"));
    t.insert(Key(16, "Sixteen"));
    t.insert(Key(17, "Seventeen"));
    t.insert(Key(18, "Eighteen"));
    t.insert(Key(20, "Twenty"));
    t.insert(Key(23, "Twenty-Three"));

    cout << "The B-tree is: ";
    t.traverse();

    Key k1(10, "Ten");
    (t.search(k1) != NULL) ? cout << endl
                                  << "(" << k1.integerValue << ", " << k1.stringValue << ") is found"
                           : cout << endl
                                  << "(" << k1.integerValue << ", " << k1.stringValue << ") is not Found";

    Key k2(2, "Two");
    (t.search(k2) != NULL) ? cout << endl
                                  << "(" << k2.integerValue << ", " << k2.stringValue << ") is found"
                           : cout << endl
                                  << "(" << k2.integerValue << ", " << k2.stringValue << ") is not Found\n";
}
 */

#endif