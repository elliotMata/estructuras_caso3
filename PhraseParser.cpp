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
        if (nouns->contains(word) && !(find(keywords.begin(), keywords.end(), word) != keywords.end()))
        {
            keywords.push_back(word);
        }
    }
}

vector<string> PhraseParser::getKeywords(string phrase)
{
    mincePhrase(phrase);
    return this->keywords;
}

/*int main()
{
    PhraseParser *pp = new PhraseParser("the cat is black, the black cat is mine");
    vector<string> keywords = pp->getKeywords();
    for (int i = 0; i < keywords.size(); i++)
    {
        cout << keywords[i] << endl;
    }
    return 0;
}*/