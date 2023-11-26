/* Driver class
 */

#include <string>

#include "BookIndexer.h"
#include "PhraseParser.h"
#include "MatchMaker.h"
#include "RelevanceCalculator.h"

unordered_map<string, BTree *> *books;
BookIndexer<string> indexer;
unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *wordRelevance;

/*string replace(string s, char current, char replace)
{
    int length = s.length();
    for (int i = 0; i < length; i++)
    {
        if (s[i] == current)
        {
            s[i] = replace;
        }
    }
    return s;
}*/

unordered_map<int, double> *createRelevanceMap(vector<pair<int, int>> *amountWords, vector<int> *keywordParagraphs, int totalParagraphs)
{
    RelevanceCalculator ranking;
    unordered_map<int, double> *relevanceMap = new unordered_map<int, double>();
    for (int position = 0; position < keywordParagraphs->size(); position++)
    {
        int totalRepetitions = amountWords->at(position).first;
        int totalParagraphWords = amountWords->at(position).second;
        int totalAppearances = keywordParagraphs->size();
        double relevance = ranking.getRelevance(totalRepetitions, totalParagraphWords, totalAppearances, totalParagraphs);
        if (relevance <= 0)
        {
            relevanceMap->insert({keywordParagraphs->at(position), relevance});
        }
    }
    return relevanceMap;
}

int main()
{
    cout << "Iniciando" << endl;
    indexer.buildTree();
    FileReader fileReader;
    JsonCreator *jsonCreator = new JsonCreator("./libros");
    vector<string> *filenames = jsonCreator->getFilenames();
    books = new unordered_map<string, BTree *>();
    wordRelevance = new unordered_map<string, unordered_map<string, unordered_map<int, double> *> *>();

    for (const string &file : *filenames)
    {
        if (books->find(file) == books->end())
            (*books)[file] = new BTree();

        fileReader.processParagraphs("./libros/" + file);

        unordered_map<string, vector<int> *> *keywordParagraphs = fileReader.getKeywordParagraphs();
        unordered_map<string, vector<pair<int, int>> *> *amountWords = fileReader.getAmountWords();
        unordered_map<string, unordered_map<int, double> *> *words = new unordered_map<string, unordered_map<int, double> *>();

        for (const auto &pair : *keywordParagraphs)
        {
            (*books)[file]->insert(new Key(pair.first, pair.second));
        }

        for (auto &pair : *keywordParagraphs)
        {
            if (keywordParagraphs->at(pair.first)->size() < fileReader.getTotalParagraphsToCheck())
            {
                unordered_map<int, double> *relevanceMap = createRelevanceMap(amountWords->at(pair.first), keywordParagraphs->at(pair.first), fileReader.getTotalParagraphsToCheck());
                words->insert({pair.first, relevanceMap});
            }
        }
        wordRelevance->insert({file, words});
    }

    cout << "\"Servidor\"" << endl;
    MatchMaker *matchMaker = new MatchMaker("horror monster dark mistery", books, indexer, wordRelevance);
    vector<string> *top;

    matchMaker->findSimilarities();
    matchMaker->createParagraphRaking();

    return 0;
}