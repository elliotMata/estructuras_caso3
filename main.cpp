/* Driver class
 */

#include <string>

#include "BookIndexer.h"
#include "PhraseParser.h"
#include "MatchMaker.h"

int main()
{
    /*BookIndexer<std::string> indexer;
    indexer.buildTree();

    PhraseParser *parser = new PhraseParser("the cat is black, the black cat is mine");
    vector<string> keywords = parser->getKeywords();

    std::cout << "In-order traversal of AVL tree based on keywords:\n";
    indexer.inOrderTraversal();

    cout << "\n\n";

    vector<string> books = indexer.getBooks("love");
    for (int i = 0; i < books.size(); i++)
    {
        cout << books[i] << endl;
    }

    cout << "\n\n";

    for (int i = 0; i < keywords.size(); i++)
    {
        cout << keywords[i] << endl;
    }*/

    MatchMaker *matchMaker = new MatchMaker("love flower relationship and morning");
    matchMaker->findSimilarities();

    return 0;
}