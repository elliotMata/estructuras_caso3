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
    NounReader *nouns = NounReader::getInstance();
    istringstream ss(phrase);
    string word;

    while (ss >> word)
    {
        for (auto &letter : word)
        {
            letter = tolower(letter); // put the whole word in lowercase letters
        }
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
    string filename = "libros/A-Study-in-Scarlet-by-Arthur-Conan-Doyle.txt";

    reader.processParagraphs(filename);
    vector<pair<int, string>> *paragraphs = reader.getParagraphs();

    for (int i = 0; i < 5; i++)
    {
        cout << paragraphs->at(i).second << endl;
        parser->mincePhrase(paragraphs->at(i).second);
    }
    vector<pair<string, int>> *keywords = parser->getKeywords();
    printKeywords(keywords);

    return 0;
}
