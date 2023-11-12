#ifndef FILE_READER_H_
#define FILE_READER_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

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

    void calculateTotalParagraphs();
    void calculatePositions();
    string paragraphCleaner(const string &sentence);

public:
    void processParagraphs(const string &filename);
    vector<int> *getPositions();
    vector<pair<int, string>> *getParagraphs();
};

#endif