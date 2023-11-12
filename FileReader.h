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
    int numberParagraphs, amountToCheck;
    vector<int> *paragraphPositions;

    void calculateTotalParagraphs();
    void calculatePositions();

public:
    FileReader(const string &filename);
    ~FileReader();
    void processParagraphs();

    int getTotalParagraphs();
    vector<int> *getPositions();
};

#endif