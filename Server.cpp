#include "httplib.h"
#include "MatchMaker.h"
#include "json.hpp"
#include <unordered_map>

using namespace std;

unordered_map<string, BTree *> *books;
BookIndexer<string> indexer;

void handle_match_making(const httplib::Request &req, httplib::Response &res)
{
    string req_phrase = req.get_param_value("req_phrase");

    if (req_phrase != "")
    {
        MatchMaker *matchMaker = new MatchMaker(req_phrase, books, indexer);
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

int main()
{
    httplib::Server server;

    cout << "Iniciando" << endl;
    indexer.buildTree();
    FileReader fileReader;
    JsonCreator *jsonCreator = new JsonCreator("./libros");
    vector<string> *filenames = jsonCreator->getFilenames();
    books = new unordered_map<string, BTree *>();
    for (const string &file : *filenames)
    {
        if (books->find(file) == books->end())
            (*books)[file] = new BTree();

        fileReader.processParagraphs("./libros/" + file);
        unordered_map<string, vector<int> *> *keywordParagraphs = fileReader.getKeywordParagraphs();
        for (const auto &pair : *keywordParagraphs)
        {
            (*books)[file]->insert(new Key(pair.first, pair.second));
        }
    }
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