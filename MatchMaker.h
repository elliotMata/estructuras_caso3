#ifndef MATCH_MAKER_H_
#define MATCH_MAKER_H_

#include "BookIndexer.h"
#include "PhraseParser.h"
#include "Comparator.h"

class MatchMaker
{
private:
    BookIndexer<string> indexer;
    Comparator *comparator;
    PhraseParser *parser;
    JsonParser *jsonParser;
    vector<string> nouns;

public:
    MatchMaker(string phrase)
    {
        indexer.buildTree();
        comparator = new Comparator;
        parser = new PhraseParser;
        nouns = parser->getKeywords(phrase);
        jsonParser = JsonParser::getInstance();
    }

    void findSimilarities()
    {
        vector<string> books;
        for (string noun : nouns)
        {
            books = indexer.getBooks(noun);

            cout << "Current noun : " << noun << "\n\n";
            cout << "Checking the following books" << endl;
            for (string book : books)
            {
                cout << book << endl;
            }

            cout << endl;

            vector<string> bookNouns;
            for (string book : books)
            {
                comparator->compareVectors(nouns, jsonParser->getNouns(book));
                cout << "Similarity with " + book << " is : " << comparator->getSimilarity() << endl;
            }

            cout << "\n--------------------------------------\n\n";
        }
    }
};

#endif