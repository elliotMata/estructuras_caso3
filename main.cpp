/* Driver class
 */

#include <string>

#include "BookIndexer.h"
#include "PhraseParser.h"

int main()
{
    BookIndexer<std::string> indexer;
    indexer.buildTree();

    PhraseParser *parser = new PhraseParser("the cat is black, the black cat is mine");
    vector<string> keywords = parser->getKeywords();

    std::cout << "In-order traversal of AVL tree based on keywords:\n";
    indexer.inOrderTraversal();

    cout << "\n\n";

    for (int i = 0; i < keywords.size(); i++)
    {
        cout << keywords[i] << endl;
    }

    return 0;
}