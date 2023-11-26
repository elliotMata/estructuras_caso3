#include "httplib.h"
#include "MatchMaker.h"
#include "RelevanceCalculator.h"
#include "JsonCreator.h"
#include "json.hpp"
#include <unordered_map>

using namespace std;

unordered_map<string, BTree *> *books;
BookIndexer<string> indexer;
unordered_map<string, unordered_map<string, unordered_map<int, double> *> *> *wordRelevance;
unordered_map<string, unordered_map<int, string> *> *bookParagraphs;

void handle_match_making(const httplib::Request &req, httplib::Response &res)
{
    string req_phrase = req.get_param_value("req_phrase");

    res.set_header("Access-Control-Allow-Origin", "*");

    if (req_phrase != "")
    {
        MatchMaker *matchMaker = new MatchMaker(req_phrase, books, indexer, wordRelevance, bookParagraphs);
        vector<string> *ranking;

        matchMaker->findSimilarities();
        matchMaker->createParagraphRaking();
        ranking = matchMaker->getResults();

        nlohmann::json response_json = nlohmann::json::array();
        for (const string &item : *ranking)
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
        }
    }
    return relevanceMap;
}

unordered_map<int, string> *createParagraphsMap(vector<pair<int, string>> *paragraphs)
{
    unordered_map<int, string> *paragraphsMap = new unordered_map<int, string>();
    for (auto pair : *paragraphs)
    {
        paragraphsMap->insert({pair.first, pair.second});
    }
    return paragraphsMap;
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
    bookParagraphs = new unordered_map<string, unordered_map<int, string> *>();

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

        unordered_map<int, string> *paragraphsMap = createParagraphsMap(fileReader.getParagraphs());
        bookParagraphs->insert({file, paragraphsMap});
    }

    server.Get("/match", handle_match_making);
    server.Options("/(.*)",
			[&](const httplib::Request &req, httplib::Response &res) {
			res.set_header("Access-Control-Allow-Methods", " POST, GET, OPTIONS");
			res.set_header("Content-Type", "application/json");
			res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept");
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Connection", "close");
		});
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