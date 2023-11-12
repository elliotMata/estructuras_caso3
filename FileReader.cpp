#include "FileReader.h"
#include <algorithm>

void FileReader::calculateTotalParagraphs()
{
    if (!file.is_open())
    {
        numberParagraphs = -1;
        return;
    }
    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    amountToCheck = (fileSize * PERCENTAGE) / PARAGRAPH_SIZE;
    numberParagraphs = fileSize / PARAGRAPH_SIZE;
}

void FileReader::calculatePositions()
{
    paragraphPositions = new vector<int>();
    calculateTotalParagraphs();

    int count = amountToCheck;
    while (count != 0)
    {
        int position = rand() % numberParagraphs;
        if (!(find(paragraphPositions->begin(), paragraphPositions->end(), position) != paragraphPositions->end()))
        {
            paragraphPositions->push_back(position);
            count--;
        }
    }
}

void FileReader::processParagraphs()
{
    calculatePositions();

    file.seekg(paragraphPositions->at(0), std::ios::beg);
    char buffer[PARAGRAPH_SIZE + 1];
    file.read(buffer, PARAGRAPH_SIZE);
    buffer[file.gcount()] = '\0';

    for (int i = 0; i < file.gcount(); i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = ' ';
        }
    }

    cout << "Read data: " << buffer << endl;
    file.close();
}

FileReader::FileReader(const string &filename)
{
    file.open(filename);
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
    }
}

FileReader::~FileReader()
{
    if (file.is_open())
    {
        file.close();
    }
}

int FileReader::getTotalParagraphs()
{
    calculateTotalParagraphs();
    return this->numberParagraphs;
}

vector<int> *FileReader::getPositions()
{
    calculatePositions();
    return this->paragraphPositions;
}

int main()
{
    FileReader reader("./libros/A-Study-in-Scarlet-by-Arthur-Conan-Doyle.txt");
    /*vector<int> *positions = reader.getPositions();

    for (int i = 0; i < positions->size(); i++)
    {
        cout << positions->at(i) << " ";
    }
    cout << "\n\n";*/
    reader.processParagraphs();

    return 0;
}