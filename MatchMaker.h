#ifndef MATCH_MAKER_H_
#define MATCH_MAKER_H_

#include <bits/stdc++.h>
#include <algorithm>
#include <unordered_map>

#include "BookIndexer.h"
#include "PhraseParser.h"
#include "Comparator.h"
#include "FileReader.h"
#include "JsonCreator.cpp"
#include "BTree.h"

class MatchMaker
{
private:
    multimap<double, string, greater<int>> ranking;
    unordered_map<string, multimap<double, int, greater<double>> *> paragraphRanking;
    unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *wordRelevance;
    BookIndexer<string> indexer;
    Comparator *comparator;
    PhraseParser *parser;
    JsonParser *jsonParser;
    vector<string> *nouns;
    FileReader fileReader;
    JsonCreator *jsonCreator;
    unordered_map<string, BTree *> *books;

public:
    MatchMaker(string phrase, unordered_map<string, BTree *> *pBooks, BookIndexer<string> pIndexer, unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *pWordRelevance)
    {
        // indexer.buildTree();
        comparator = new Comparator;
        parser = new PhraseParser;
        nouns = parser->getKeywords(phrase);
        jsonParser = JsonParser::getInstance();
        books = pBooks;
        indexer = pIndexer;
        wordRelevance = pWordRelevance;
        /* jsonCreator = new JsonCreator("./libros");
        vector<string> *filenames = jsonCreator->getFilenames();
        unordered_map<string, BTree *> *books = new unordered_map<string, BTree *>();
        for (const auto &file : *filenames)
        {
            if (books->find(file) == books->end())
                (*books)[file] = new BTree();
            fileReader.processParagraphs("./libros/" + file);
            unordered_map<string, vector<int> *> *keywordParagraphs = fileReader.getKeywordParagraphs();
            for (const auto &pair : *keywordParagraphs)
            {
                (*books)[file]->insert(new Key(pair.first, pair.second));
            }
        } */
    }

    void findSimilarities()
    {
        vector<string> books;

        for (string noun : *nouns)
        {
            books = indexer.getBooks(noun);

            vector<string> bookNouns;
            for (string book : books)
            {
                comparator->compareVectors(*nouns, *jsonParser->getNouns(book));
                ranking.insert(make_pair(comparator->getSimilarity(), book));
                /* TreeNode *node = bTree->search(book);
                comparator->compareVectors(*nouns, *node->getKeywords());
                paragraphRanking[book].insert(make_pair(comparator->getSimilarity(), fileReader.readParagraph(*node->getPosition(), book))); */
                // makeParagraphRanking(book);
            }
        }
    }

    void printMap()
    {
        multimap<double, string>::iterator iterator;
        for (iterator = ranking.begin(); iterator != ranking.end(); iterator++)
        {
            cout << "(" << iterator->first << ", " << iterator->second << ")" << endl;
        }
    }

    void printMap(multimap<double, int, greater<double>> *map)
    {
        multimap<double, int>::iterator iterator;
        for (iterator = map->begin(); iterator != map->end(); iterator++)
        {
            cout << "(" << iterator->first << ", " << iterator->second << ")" << endl;
        }
        cout << "---------------------------------------------------------" << endl;
    }

    vector<string> getTop()
    {
        multimap<double, string>::iterator iterator;
        vector<string> top;
        for (iterator = ranking.begin(); iterator != ranking.end(); iterator++)
        {
            top.push_back(iterator->second);
            if (top.size() == 10)
            {
                return top;
            }
        }
        return top;
    }

    multimap<double, int, greater<double>> *makeParagraphRanking(string book)
    {
        multimap<double, int, greater<double>> *ranking = new multimap<double, int, greater<double>>();
        unordered_map<string, vector<int> *> wordParagraphs;
        unordered_map<string, vector<int> *>::iterator iterator;
        double relevance = 0;

        for (string word : *nouns)
        {
            wordParagraphs.insert({word, books->at(book)->search(word)});
        }

        for (auto &pair : wordParagraphs)
        {
            for (int paragraph : *pair.second)
            {
                relevance = (*(*(*wordRelevance)[book])[pair.first])[paragraph];
                ranking->insert({relevance, paragraph});
            }
        }
        printMap(ranking);
        return ranking;
    }
};

#endif