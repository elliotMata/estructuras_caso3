#include <sstream>
#include <algorithm>

#include "PhraseParser.h"
#include "NounReader.h"

void PhraseParser::mincePhrase(string phrase)
{
    NounReader *nouns = NounReader::getInstance();
    istringstream ss(phrase);
    string word;
    totalWords = 0;

    while (ss >> word)
    {
        if (nouns->contains(word) && !(find(keywords->begin(), keywords->end(), word) != keywords->end()))
        {
            keywords->push_back(word);
        }
        totalWords += 1;
    }
}

string PhraseParser::phraseCleaner(const string &phrase)
{
    string cleanedPhrase = phrase;
    transform(cleanedPhrase.begin(), cleanedPhrase.end(), cleanedPhrase.begin(), ::tolower);
    cleanedPhrase.erase(std::remove_if(cleanedPhrase.begin(), cleanedPhrase.end(), ::ispunct), cleanedPhrase.end());
    cleanedPhrase.erase(std::remove_if(cleanedPhrase.begin(), cleanedPhrase.end(), ::isdigit), cleanedPhrase.end());

    return cleanedPhrase;
}

vector<string> *PhraseParser::getKeywords(string phrase)
{
    keywords = new vector<string>();
    phrase = phraseCleaner(phrase);
    mincePhrase(phrase);
    return this->keywords;
}

int PhraseParser::getTotalWords()
{
    return this->totalWords;
}

/*int main()
{
    PhraseParser pp;
    vector<string> *keywords = pp.getKeywords("the cat is black, the black cat is mine");
    for (int i = 0; i < keywords->size(); i++)
    {
        cout << keywords->at(i) << endl;
    }
    return 0;
}*/