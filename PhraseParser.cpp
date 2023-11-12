#include <sstream>
#include <algorithm>

#include "PhraseParser.h"
#include "NounReader.h"

void PhraseParser::mincePhrase(string phrase)
{
    NounReader *nouns = NounReader::getInstance();
    istringstream ss(phrase);
    string word;

    while (ss >> word)
    {
        if (nouns->contains(word) && !(find(keywords->begin(), keywords->end(), word) != keywords->end()))
        {
            keywords->push_back(word);
        }
    }
}

vector<string> *PhraseParser::getKeywords(string phrase)
{
    keywords = new vector<string>();
    mincePhrase(phrase);
    return this->keywords;
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