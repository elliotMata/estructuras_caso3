#include "FileReader.h"
#include "PhraseParser.h"
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
    paragraphKeywords = new vector<pair<int, vector<string> *>>();
    keywordParagraphs = new unordered_map<string, vector<int> *>();
    amountWords = new unordered_map<string, vector<pair<int, int>> *>();

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
        addKeywords(paragraphPositions->at(i), paragraph);
        paragraphs->push_back({paragraphPositions->at(i), paragraph});
    }
    file.close();
}

void FileReader::addKeywords(int position, string paragraph)
{
    PhraseParser parser;
    position = position * PARAGRAPH_SIZE;
    vector<string> *keywords = parser.getKeywords(paragraph);
    paragraphKeywords->push_back({position, keywords});
    for (const string &keyword : *keywords)
    {
        int repetitions = countRepetitions(keyword, paragraph);
        int totalWordsParagraph = parser.getTotalWords();
        if (amountWords->find(keyword) == amountWords->end())
        {
            (*amountWords)[keyword] = new vector<pair<int, int>>();
        }
        (*amountWords)[keyword]->push_back({repetitions, totalWordsParagraph});

        if (keywordParagraphs->find(keyword) == keywordParagraphs->end())
            (*keywordParagraphs)[keyword] = new vector<int>();
        (*keywordParagraphs)[keyword]->push_back(position);
    }
}

int FileReader::countRepetitions(string keyword, string paragraph)
{
    int totalRepetitions = 0;
    string word;
    stringstream stream(paragraph);
    while (stream >> word)
    {
        if (word.compare(keyword) == 0)
        {
            totalRepetitions += 1;
        }
    }
    return totalRepetitions;
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

vector<pair<int, vector<string> *>> *FileReader::getParagraphKeywords()
{
    return this->paragraphKeywords;
}

unordered_map<string, vector<int> *> *FileReader::getKeywordParagraphs()
{
    return this->keywordParagraphs;
}

unordered_map<string, vector<pair<int, int>> *> *FileReader::getAmountWords()
{
    return this->amountWords;
}

int FileReader::getTotalParagraphsToCheck()
{
    return this->amountToCheck;
}

/*int main()
{
    FileReader reader;
    reader.processParagraphs("./libros/A-Study-in-Scarlet-by-Arthur-Conan-Doyle.txt");
    vector<pair<int, string>> *paragraphs = reader.getParagraphs();
    vector<pair<int, vector<string> *>> *keywords = reader.getParagraphKeywords();

    for (auto p : *paragraphs)
    {
        cout << p.first << ". " << p.second << endl;
    }

    for (auto p : *keywords)
    {
        cout << p.first << ". ";
        for (string word : *p.second)
        {
            cout << word << ", ";
        }
        cout << endl;
    }

    return 0;
}*/