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
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    amountToCheck = (fileSize * PERCENTAGE) / PARAGRAPH_SIZE;
    numberParagraphs = (fileSize / PARAGRAPH_SIZE) - 1;
}

void FileReader::calculatePositions()
{
    paragraphPositions = new vector<int>();
    calculateTotalParagraphs();

    int count = amountToCheck;
    while (count != 0)
    {
        int position = rand() % numberParagraphs;
        if (!(find(paragraphPositions->begin(), paragraphPositions->end(), position) != paragraphPositions->end()) && position * PARAGRAPH_SIZE <= fileSize)
        {
            paragraphPositions->push_back(position);
            count--;
        }
    }
}

string FileReader::paragraphCleaner(const string &input)
{
    string result;
    for (char letter : input)
    {
        if (isalpha(letter) || letter == ' ')
        {
            result += tolower(letter);
        }
    }

    return result;
}

void FileReader::processParagraphs(const string &filename)
{
    file.open(filename);
    calculatePositions();

    paragraphs = new vector<pair<int, string>>();

    for (int i = 0; i < paragraphPositions->size(); i++)
    {
        file.seekg(paragraphPositions->at(i) * PARAGRAPH_SIZE, std::ios::beg);
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
        string paragraph(buffer);
        paragraph = paragraphCleaner(paragraph);
        paragraphs->push_back({paragraphPositions->at(i), paragraph});
    }
    file.close();
}

vector<int> *FileReader::getPositions()
{
    calculatePositions();
    return this->paragraphPositions;
}

vector<pair<int, string>> *FileReader::getParagraphs()
{
    return this->paragraphs;
}

/*int main()
{
    FileReader reader;
    reader.processParagraphs("./libros/A-Study-in-Scarlet-by-Arthur-Conan-Doyle.txt");
    vector<pair<int, string>> *paragraphs = reader.getParagraphs();

    for (auto p : *paragraphs)
    {
        cout << p.first << ". " << p.second << endl;
    }

    return 0;
}*/