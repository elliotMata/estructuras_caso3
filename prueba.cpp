#include "BTree.h"

int main()
{
    BTree t(3);
    vector<string> words;
    words.push_back("hello");

    t.insert(Key(8, "Eight", words));
    t.insert(Key(9, "Nine", words));
    t.insert(Key(10, "Ten", words));
    t.insert(Key(11, "Eleven", words));
    t.insert(Key(15, "Fifteen", words));
    t.insert(Key(16, "Sixteen", words));
    t.insert(Key(17, "Seventeen", words));
    t.insert(Key(18, "Eighteen", words));
    t.insert(Key(20, "Twenty", words));
    t.insert(Key(23, "Twenty-Three", words));

    cout << "The B-tree is: ";
    t.traverse();

    Key k1(10, "Ten", words);
    (t.search(k1) != NULL) ? cout << endl
                                  << "(" << *k1.position << ", " << *k1.filename << ") is found"
                           : cout << endl
                                  << "(" << *k1.position << ", " << *k1.filename << ") is not Found";

    Key k2(2, "Two", words);
    (t.search(k2) != NULL) ? cout << endl
                                  << "(" << *k2.position << ", " << *k2.filename << ") is found"
                           : cout << endl
                                  << "(" << *k2.position << ", " << *k2.filename << ") is not Found\n";
}