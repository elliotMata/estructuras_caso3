#include "httplib.h"
#include "MatchMaker.h"
#include "RelevanceCalculator.h"
#include "json.hpp"
#include <unordered_map>

using namespace std;

unordered_map<string, BTree *> *books;
BookIndexer<string> indexer;
unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *wordRelevance;

void handle_match_making(const httplib::Request &req, httplib::Response &res)
{
    string req_phrase = req.get_param_value("req_phrase");

    if (req_phrase != "")
    {
        MatchMaker *matchMaker = new MatchMaker(req_phrase, books, indexer, wordRelevance);
        vector<string> top;

        matchMaker->findSimilarities();
        top = matchMaker->getTop();

        nlohmann::json response_json = nlohmann::json::array();
        for (const string &item : top)
        {
            response_json.push_back(item);
        }

        res.set_content(response_json.dump(), "application/json");
    }
    else
    {
        res.status = 400;
        res.set_content("Missing req_phrase parameter", "text/plain");
    }
}

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
            // cout << "adding par " << keywordParagraphs->at(position) << " with relevance " << relevance << endl;
        }
    }
    return relevanceMap;
}

int main()
{
    httplib::Server server;

    cout << "Iniciando" << endl;
    indexer.buildTree();
    FileReader fileReader;
    JsonCreator *jsonCreator = new JsonCreator("./libros");
    vector<string> *filenames = jsonCreator->getFilenames();
    books = new unordered_map<string, BTree *>();
    wordRelevance = new unordered_map<string, unordered_map<string, unordered_map<int, double> *> *>();

    for (const string &file : *filenames)
    {
        // cout << "------------------------ " << file << " ----------------------------" << endl;
        if (books->find(file) == books->end())
            (*books)[file] = new BTree();

        fileReader.processParagraphs("./libros/" + file);

        unordered_map<string, vector<int> *> *keywordParagraphs = fileReader.getKeywordParagraphs();
        unordered_map<string, vector<pair<int, int>> *> *amountWords = fileReader.getAmountWords();
        unordered_map<string, unordered_map<int, double> *> *words = new unordered_map<string, unordered_map<int, double> *>();

        for (const auto &pair : *keywordParagraphs)
        {
            /* if (pair.first == "zone" && file == "The-Divine-Comedy-by-Dante-Alighieri.txt")
            {
                for (const int &i : *pair.second)
                {
                    cout << i << endl;
                }
            } */
            (*books)[file]->insert(new Key(pair.first, pair.second));
        }

        for (auto &pair : *keywordParagraphs)
        {
            // cout << "---------- in word " << pair.first << endl;
            if (keywordParagraphs->at(pair.first)->size() < fileReader.getTotalParagraphsToCheck())
            {
                unordered_map<int, double> *relevanceMap = createRelevanceMap(amountWords->at(pair.first), keywordParagraphs->at(pair.first), fileReader.getTotalParagraphsToCheck());
                words->insert({pair.first, relevanceMap});
            }
        }
        wordRelevance->insert({file, words});
    }
    /* vector<int> *result = (*books)["The-Divine-Comedy-by-Dante-Alighieri.txt"]->search("zone");
    for (const int &i : *result)
    {
        cout << i << endl;
    } */
    server.Get("/match", handle_match_making);
    try
    {
        cout << "Activando servidor" << endl;
        server.listen("0.0.0.0", 65000);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}