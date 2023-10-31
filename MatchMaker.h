#ifndef MATCH_MAKER_H_
#define MATCH_MAKER_H_

#include <bits/stdc++.h>

#include "BookIndexer.h"
#include "PhraseParser.h"
#include "Comparator.h"

class MatchMaker
{
private:
    multimap<double, string, greater<int>> ranking;

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

            vector<string> bookNouns;
            for (string book : books)
            {
                comparator->compareVectors(nouns, jsonParser->getNouns(book));
                ranking.insert(make_pair(comparator->getSimilarity(), book));
            }
        }
    }

    vector<string> getTopThree()
    {
        multimap<double, string>::iterator iterator;
        vector<string> topThree;
        int count = 0;
        for (iterator = ranking.begin(); iterator != ranking.end() && count < 3; iterator++)
        {
            topThree.push_back(iterator->second);
            count++;
        }
        return topThree;
    }
};

#endif