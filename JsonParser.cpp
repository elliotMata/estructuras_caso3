#include <fstream>

#include "JsonParser.h"

JsonParser::JsonParser()
{
    ifstream jsonFile("./books.json");
    json file;
    jsonFile >> file;
    jsonFile.close();

    this->books = file["books"];
    this->bookInfo = file["bookInfo"];
    this->authors = new unordered_map<string, string>();
    this->nouns = new unordered_map<string, vector<pair<string, int>> *>();

    createMaps();
}

void JsonParser::createMaps()
{
    for (string book : books)
    {
        authors->insert(make_pair(book, bookInfo[book]["Author"]));
        vector<pair<string, int>> *keywords = new vector<pair<string, int>>();
        for (auto pair : bookInfo[book]["Keywords"])
        {
            keywords->push_back({pair["keyword"], pair["count"]});
            //  keywords->push_back(word);
        }
        nouns->insert(make_pair(book, keywords));
    }
}

JsonParser *JsonParser::getInstance()
{
    if (instance == nullptr)
    {
        instance = new JsonParser();
    }
    return instance;
}

string JsonParser::getAuthor(string bookName)
{
    return authors->at(bookName);
}

vector<pair<string, int>> *JsonParser::getNouns(string bookName)
{
    return nouns->at(bookName);
}

unordered_map<string, vector<pair<string, int>> *> *JsonParser::getNounsHash()
{
    return this->nouns;
}

JsonParser *JsonParser::instance = nullptr;

/*int main()
{
    JsonParser *jsonParser = JsonParser::getInstance();
    vector<pair<string, int>> *nouns = jsonParser->getNouns("Emma-by-Jane-Austen");
    for (auto pair : *nouns)
    {
        cout << pair.first << " : " << pair.second << endl;
    }
    return 0;
}*/