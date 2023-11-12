#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "json.hpp"

#include "NounReader.h"
#include "FileReader.h"

using namespace std;
using json = nlohmann::json;

class PhraseParser
{
private:
    string phrase;
    vector<pair<string, int>> *keywords; // pair of keyword and count

public:
    PhraseParser();
    void mincePhrase(string phrase);
    vector<pair<string, int>> *getKeywords(); // returns the keywords and their count
    void saveToJson(const string &filename);
};

/* -------------------------------------------------------- */

PhraseParser::PhraseParser()
{
    keywords = new vector<pair<string, int>>();
}

void PhraseParser::mincePhrase(string phrase)
{
    cout << "here 1" << endl;
    NounReader *nouns = NounReader::getInstance();
    cout << "here 2" << endl;
    istringstream ss(phrase);
    cout << "here 3" << endl;
    string word;

    cout << "*** PHRASE ***\n"
         << phrase << "\n\n";

    while (ss >> word)
    {
        cout << word << endl;
        if (nouns->contains(word))
        {
            auto it = find_if(keywords->begin(), keywords->end(), [word](const pair<string, int> &p)
                              { return p.first == word; });
            if (it != keywords->end())
            {
                it->second++; // Increment count if the keyword already exists
            }
            else
            {
                keywords->push_back({word, 1}); // Add a new keyword with count 1
            }
        }
    }
}

vector<pair<string, int>> *PhraseParser::getKeywords()
{
    return this->keywords;
}

void PhraseParser::saveToJson(const string &filename)
{
    json j;
    for (const auto &pair : *keywords)
    {
        j["Keywords"].push_back({{"keyword", pair.first}, {"count", pair.second}});
    }

    ofstream outputFile(filename);
    outputFile << j.dump(2); // pretty print with indentation
}

void printKeywords(vector<pair<string, int>> *keywords)
{
    for (auto p : *keywords)
    {
        cout << p.first << " -> " << p.second << endl;
    }
    cout << "\n\n";
}

int main()
{
    PhraseParser *parser = new PhraseParser();
    FileReader reader;

    reader.processParagraphs("./libros/A-Study-in-Scarlet-by-Arthur-Conan-Doyle.txt");
    vector<pair<int, string>> *paragraphs = reader.getParagraphs();

    for (auto p : *paragraphs)
    {
        cout << p.first << ". " << p.second << endl;
        parser->mincePhrase(p.second);
    }
    vector<pair<string, int>> *keywords = parser->getKeywords();
    printKeywords(keywords);

    return 0;
}