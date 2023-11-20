#ifndef FILE_READER_H_
#define FILE_READER_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#define PARAGRAPH_SIZE 512
#define PERCENTAGE 0.2

using namespace std;

class FileReader
{
private:
    fstream file;
    int fileSize, numberParagraphs, amountToCheck;
    vector<pair<int, string>> *paragraphs;
    vector<int> *paragraphPositions;
    vector<pair<int, vector<string> *>> *paragraphKeywords;
    unordered_map<string, vector<int> *> *keywordParagraphs; // hash con un vector de las posiciones de los parrafos en los que se encuentra la keyword

    void calculateTotalParagraphs();
    void calculatePositions();
    string paragraphCleaner(const string &sentence);
    void addKeywords(int position, string paragraph);

public:
    void processParagraphs(const string &filename);
    vector<int> *getPositions();
    vector<pair<int, string>> *getParagraphs();
    vector<pair<int, vector<string> *>> *getParagraphKeywords();
    unordered_map<string, vector<int> *> *getKeywordParagraphs();
    string readParagraph(const int &pos, const string &filename);
};

#endif