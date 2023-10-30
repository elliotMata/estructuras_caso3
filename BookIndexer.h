#ifndef BOOKREADER_H
#define BOOKREADER_H

#include <iostream>
#include <vector>
#include "JsonParser.h"
#include "AVL.h"

template <typename T>
class BookIndexer
{
private:
    AVLTree<T> avlTree;

public:
    void buildTree()
    {
        JsonParser *jsonParser = JsonParser::getInstance();

        // Get the list of books
        std::vector<std::string> books;
        for (const auto &pair : jsonParser->getNounsHash())
        {
            books.push_back(pair.first);
        }

        for (const std::string &book : books)
        {
            // Get the list of keywords for the book
            std::vector<std::string> keywords = jsonParser->getNouns(book);

            for (const std::string &keyword : keywords)
            {
                // Insert keyword as the key and associate the book name with it
                avlTree.insert(keyword, book);
            }
        }
    }

    void inOrderTraversal()
    {
        avlTree.inOrderTraversal();
    }
};

#endif

/* int main()
{
    AVLTreeBuilder<std::string> builder;
    builder.buildTree();

    std::cout << "In-order traversal of AVL tree based on keywords:\n";
    builder.inOrderTraversal();

    return 0;
} */