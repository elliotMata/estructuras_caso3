#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "json.hpp"

#include "NounReader.h"

using namespace std;
using json = nlohmann::json;

class PhraseParser
{
private:
    string phrase;
    vector<pair<string, int>> keywords; // pair of keyword and count

    void mincePhrase(string phrase);

public:
    vector<pair<string, int>> getKeywords(string phrase); // returns the keywords and their count
    void saveToJson(const string &filename);
};

void PhraseParser::mincePhrase(string phrase)
{
    NounReader *nouns = NounReader::getInstance();
    istringstream ss(phrase);
    string word;

    while (ss >> word)
    {
        if (nouns->contains(word))
        {
            auto it = find_if(keywords.begin(), keywords.end(),
                              [word](const pair<string, int> &p)
                              {
                                  return p.first == word;
                              });

            if (it != keywords.end())
            {
                // Increment count if the keyword already exists
                it->second++;
            }
            else
            {
                // Add a new keyword with count 1
                keywords.push_back({word, 1});
            }
        }
    }
}

vector<pair<string, int>> PhraseParser::getKeywords(string phrase)
{
    keywords.clear(); // Clear the vector before processing a new phrase
    mincePhrase(phrase);
    return this->keywords;
}

void PhraseParser::saveToJson(const string &filename)
{
    json j;
    for (const auto &pair : keywords)
    {
        j["Keywords"].push_back({{"keyword", pair.first}, {"count", pair.second}});
    }

    ofstream outputFile(filename);
    outputFile << j.dump(2); // pretty print with indentation
}

int main()
{
    PhraseParser pp;
    ifstream inputFile("libros/A-Study-in-Scarlet-by-Arthur-Conan-Doyle.txt");
    string fileContent;
    if (inputFile.is_open())
    {
        getline(inputFile, fileContent);
        inputFile.close();
    }
    vector<pair<string, int>> keywords = pp.getKeywords("the cat is black, the black cat is mine");
    pp.saveToJson("output.json");

    return 0;
}
