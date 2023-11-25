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
    multimap<double, string, greater<double>> ranking;
    unordered_map<string, multimap<double, int, greater<double>> *> paragraphRanking;
    unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *wordRelevance;
    unordered_map<string, double> *booksRelevance;
    BookIndexer<string> indexer;
    Comparator *comparator;
    PhraseParser *parser;
    JsonParser *jsonParser;
    vector<string> *nouns;
    FileReader fileReader;
    JsonCreator *jsonCreator;
    unordered_map<string, BTree *> *books;

    void createBooksRelevance(string book, double matchPercentage)
    {
        if (booksRelevance->find(book) != nullptr)
        {
            booksRelevance->at(book) += matchPercentage;
        }
        else
        {
            booksRelevance->insert({book, matchPercentage});
        }
    }

    void createBooksRanking()
    {
        unordered_map<string, double>::iterator iterator;
        for (iterator = booksRelevance->begin(); iterator != booksRelevance->end(); iterator++)
        {
            ranking.insert({iterator->second, iterator->first});
        }
    }

public:
    MatchMaker(string phrase, unordered_map<string, BTree *> *pBooks, BookIndexer<string> pIndexer, unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *pWordRelevance)
    {
        comparator = new Comparator;
        parser = new PhraseParser;
        nouns = parser->getKeywords(phrase);
        jsonParser = JsonParser::getInstance();
        books = pBooks;
        indexer = pIndexer;
        wordRelevance = pWordRelevance;
        booksRelevance = new unordered_map<string, double>();
    }

    void findSimilarities()
    {
        vector<string> books;

        for (string noun : *nouns)
        {
            books = indexer.getBooks(noun);

            cout << "\nCOMPARING FOR " << noun;
            vector<string> bookNouns;
            for (string book : books)
            {
                cout << " WITH BOOK " << book << endl;
                comparator->compareVectors(*nouns, *jsonParser->getNouns(book));
                createBooksRelevance(book, comparator->getSimilarity());
            }
        }
        createBooksRanking();
        printMap();
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
        book = book.append(".txt");
        multimap<double, int, greater<double>> *ranking = new multimap<double, int, greater<double>>();
        unordered_map<string, vector<int> *> wordParagraphs;
        unordered_map<string, vector<int> *>::iterator iterator;
        double relevance = 0;
        int amountWords = 0;

        cout << book << endl;

        for (string word : *nouns)
        {
            cout << "word " << word << endl;
            vector<int> *positions = books->at(book)->search(word);
            if (positions != nullptr)
            {
                cout << "-> mm size " << positions->size() << endl;
                wordParagraphs.insert({word, positions});
            }
        }

        for (auto &pair : wordParagraphs)
        {
            for (int paragraph : *pair.second)
            {
                relevance = (*(*(*wordRelevance)[book])[pair.first])[paragraph];
                cout << "relevance " << relevance << endl;
                ranking->insert({relevance, paragraph});
            }
        }
        cout << ranking->size() << endl;
        printMap(ranking);
        return ranking;
    }
};

#endif