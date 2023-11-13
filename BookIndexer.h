/* Clase para indexar los libros en el avl
 * Se ayuda del JsonParser para obtener los sustantivos de cada libro
 */

#ifndef BOOKREADER_H
#define BOOKREADER_H

#include <iostream>
#include <vector>
#include <string>

#include "JsonParser.h"
#include "AVL.h"

using namespace std;

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
        vector<string> books;
        for (const auto &pair : *(jsonParser->getNounsHash()))
        {
            books.push_back(pair.first);
        }

        for (const string &book : books)
        {
            // Get the list of keywords for the book
            vector<pair<string, int>> *keywords = jsonParser->getNouns(book);

            for (auto pair : *keywords)
            {
                // Insert keyword as the key and associate the book name with it
                avlTree.insert(pair.first, book);
            }
        }
    }

    void inOrderTraversal()
    {
        avlTree.inOrderTraversal();
    }

    vector<string> getBooks(string noun)
    {
        return avlTree.search(noun);
    }
};

#endif

/*int main()
{
    BookIndexer<std::string> builder;
    builder.buildTree();

    std::cout << "In-order traversal of AVL tree based on keywords:\n";
    builder.inOrderTraversal();

    return 0;
}*/