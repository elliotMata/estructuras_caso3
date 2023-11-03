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

    createMaps();
}

void JsonParser::createMaps()
{
    for (string book : books)
    {
        authors.insert(make_pair(book, bookInfo[book]["Author"]));
        nouns.insert(make_pair(book, bookInfo[book]["Keywords"]));
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
    return authors.at(bookName);
}

vector<string> JsonParser::getNouns(string bookName)
{
    return nouns.at(bookName);
}

unordered_map<string, vector<string>> JsonParser::getNounsHash()
{
    return this->nouns;
}

JsonParser *JsonParser::instance = nullptr;

/*int main()
{
    JsonParser *jsonParser = JsonParser::getInstance();
    vector<string> nouns = jsonParser->getNouns("Emma-by-Jane-Austen");
    for (int i = 0; i < nouns.size(); i++)
    {
        cout << nouns.at(i) << endl;
    }
    return 0;
}*/