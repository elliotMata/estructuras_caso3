#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <filesystem>

#include "json.hpp"
#include "NounReader.h"
#include "FileReader.h"

using namespace std;
using json = nlohmann::json;

class JsonCreator
{
private:
    json file;
    string paragraph;
    string folderName;
    vector<string> *fileNames;
    vector<pair<string, int>> *keywords; // pair of keyword and count

    void findFiles();
    vector<string> *parseFilename(string &filename);

public:
    JsonCreator(string folder);
    vector<string> *getFilenames();
    void minceParagraph(string paragraph);
    vector<pair<string, int>> *getKeywords(); // returns the keywords and their count
    void saveToJson(string &filename);
    void createJson(string &jsonName);
    void reset();
};

/* -------------------------------------------------------- */

JsonCreator::JsonCreator(string folder)
{
    fileNames = new vector<string>();
    keywords = new vector<pair<string, int>>();
    folderName = folder;
}

void JsonCreator::findFiles()
{
    for (const auto &entry : filesystem::directory_iterator(folderName))
    {
        fileNames->push_back(entry.path().filename());
    }
}

vector<string> *JsonCreator::getFilenames()
{
    findFiles();
    return this->fileNames;
}

vector<string> *JsonCreator::parseFilename(string &filename)
{
    std::replace(filename.begin(), filename.end(), '-', ' ');
    size_t positionBy = filename.find("by");

    string bookname = filename.substr(0, positionBy);
    string author = filename.substr(positionBy + 2);

    vector<string> *bookData = new vector<string>();
    bookData->push_back(bookname);
    bookData->push_back(author);

    return bookData;
}

void JsonCreator::minceParagraph(string paragraph)
{
    NounReader *nouns = NounReader::getInstance();
    istringstream ss(paragraph);
    string word;

    while (ss >> word)
    {
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

vector<pair<string, int>> *JsonCreator::getKeywords()
{
    return this->keywords;
}

void JsonCreator::saveToJson(string &filename)
{
    file["books"].push_back(filename);

    vector<string> *bookDate = parseFilename(filename);
    file["bookInfo"].push_back({{"Author", bookDate->at(1)}, {"Title", bookDate->at(0)}});

    for (const auto &pair : *keywords)
    {
        file["bookInfo"]["Keywords"].push_back({{"keyword", pair.first}, {"count", pair.second}});
    }
}

void JsonCreator::createJson(string &jsonName)
{
    ofstream outputFile(jsonName);
    outputFile << file.dump(2); // pretty print with indentation
}

void JsonCreator::reset()
{
    keywords->clear();
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
    string folder = "./libros";
    string outputFile = "output.json";
    JsonCreator *creator = new JsonCreator(folder);
    FileReader reader;

    vector<string> *filenames = creator->getFilenames();

    for (string file : *filenames)
    {
        string path = folder + "/" + file;
        reader.processParagraphs(path);
        vector<pair<int, string>> *paragraphs = reader.getParagraphs();

        for (auto p : *paragraphs)
        {
            cout << p.second << endl;
            creator->minceParagraph(p.second);
            // creator->saveToJson(file);
        }
        vector<pair<string, int>> *keywords = creator->getKeywords();
        printKeywords(keywords);
        creator->reset();
    }
    // creator->createJson(outputFile);
}