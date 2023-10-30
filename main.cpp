#include "BookIndexer.h"

int main()
{
    BookIndexer<std::string> indexer;
    indexer.buildTree();

    std::cout << "In-order traversal of AVL tree based on keywords:\n";
    indexer.inOrderTraversal();

    return 0;
}