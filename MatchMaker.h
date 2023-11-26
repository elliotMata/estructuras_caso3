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
    multimap<double, string, greater<double>> bookRaking;
    multimap<int, pair<string, vector<string> *>> *paragraphRanking;
    unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *wordRelevance;
    unordered_map<string, unordered_map<int, string> *> *bookParagraphs;
    unordered_map<string, double> *booksRelevance;
    vector<string> *topTen;
    vector<string> *results;

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
            bookRaking.insert({iterator->second, iterator->first});
        }
    }

    multimap<double, int, greater<double>> *createParagraphRelevance(string book)
    {
        multimap<double, int, greater<double>> *ranking = new multimap<double, int, greater<double>>();
        unordered_map<string, vector<int> *> wordParagraphs;
        unordered_map<string, vector<int> *>::iterator iterator;
        double relevance = 0;
        int amountWords = 0;

        for (string word : *nouns)
        {
            vector<int> *positions = books->at(book)->search(word);
            if (positions != nullptr)
            {
                wordParagraphs.insert({word, positions});
            }
        }

        for (auto &pair : wordParagraphs)
        {
            for (int paragraph : *pair.second)
            {
                double relevance = 0;
                int amountWords = 0;
                relevance += (*(*(*wordRelevance)[book])[pair.first])[paragraph];
                amountWords++;
                for (iterator = wordParagraphs.begin(); iterator != wordParagraphs.end(); iterator++)
                {
                    for (int position = 0; position < iterator->second->size(); position++)
                    {
                        if (iterator->second->at(position) == paragraph)
                        {
                            relevance += (*(*(*wordRelevance)[book])[pair.first])[iterator->second->at(position)];
                            amountWords++;
                        }
                    }
                }
                double end = relevance / amountWords;
                ranking->insert({end, paragraph});
            }
        }
        return ranking;
    }

    void printMap()
    {
        multimap<double, string>::iterator iterator;
        for (iterator = bookRaking.begin(); iterator != bookRaking.end(); iterator++)
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

    void *createTopBooks()
    {
        multimap<double, string>::iterator iterator;
        for (iterator = bookRaking.begin(); iterator != bookRaking.end(); iterator++)
        {
            topTen->push_back(iterator->second);
            if (topTen->size() == 10)
            {
                break;
            }
        }
        return topTen;
    }

    void printRanking()
    {
        multimap<int, pair<string, vector<string> *>>::iterator iterator;
        for (iterator = paragraphRanking->begin(); iterator != paragraphRanking->end(); iterator++)
        {
            cout << iterator->first << ". " << iterator->second.first << endl;
            for (string paragraph : *iterator->second.second)
            {
                cout << "  -> " << paragraph << "\n\n";
            }
            cout << endl;
        }
    }

    void createResults()
    {
        results = new vector<string>();
        multimap<int, pair<string, vector<string> *>>::iterator iterator;
        for (iterator = paragraphRanking->begin(); iterator != paragraphRanking->end(); iterator++)
        {
            results->push_back(iterator->second.first);
            for (string paragraph : *iterator->second.second)
            {
                results->push_back(paragraph);
            }
        }
    }

public:
    MatchMaker(string phrase, unordered_map<string, BTree *> *pBooks, BookIndexer<string> pIndexer, unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *pWordRelevance, unordered_map<string, unordered_map<int, string> *> *pBookParagraphs)
    {
        comparator = new Comparator;
        parser = new PhraseParser;
        nouns = parser->getKeywords(phrase);
        jsonParser = JsonParser::getInstance();
        books = pBooks;
        indexer = pIndexer;
        wordRelevance = pWordRelevance;
        topTen = new vector<string>();
        booksRelevance = new unordered_map<string, double>();
        paragraphRanking = new multimap<int, pair<string, vector<string> *>>();
        bookParagraphs = pBookParagraphs;
    }

    void findSimilarities()
    {
        vector<string> currentBooks;
        for (string noun : *nouns)
        {
            currentBooks = indexer.getBooks(noun);

            vector<string> bookNouns;
            for (string book : currentBooks)
            {
                comparator->compareVectors(*nouns, *jsonParser->getNouns(book));
                double matchPercentage = 0;
                if (books->at(book.append(".txt"))->search(noun) != nullptr)
                {
                    matchPercentage = comparator->getSimilarity();
                }
                createBooksRelevance(book, matchPercentage);
            }
        }
        createBooksRanking();
        createTopBooks();
    }

    void createParagraphRaking()
    {
        int index = 1;
        for (string book : *topTen)
        {
            multimap<double, int, greater<double>> *ranking = createParagraphRelevance(book);
            vector<string> *topThree = new vector<string>();
            multimap<double, int>::iterator iterator;
            int amount = 0;
            for (iterator = ranking->begin(); iterator != ranking->end() && amount < 3; iterator++)
            {
                topThree->push_back(((*(*bookParagraphs)[book]))[iterator->second]);
                amount++;
            }
            paragraphRanking->insert({index, make_pair(book, topThree)});
            index++;
        }
    }

    vector<string> *getTopBooks()
    {
        return this->topTen;
    }

    multimap<int, pair<string, vector<string> *>> *getRanking()
    {
        return this->paragraphRanking;
    }

    vector<string> *getResults()
    {
        createResults();
        return this->results;
    }
};

#endif