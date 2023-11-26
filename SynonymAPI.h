#ifndef SYNONYM_API_H_
#define SYNONYM_API_H_

#include "API.h"
#include "NounReader.h"

class SynonymAPI
{
private:
    vector<string> *synonyms;
    string url = "https://api.datamuse.com/words?rel_syn=";

public:
    vector<string> *getSynonyms(string noun)
    {
        API api;
        NounReader *nounReader = NounReader::getInstance();
        synonyms = new vector<string>();

        int count = 0;
        string wordUrl = url + noun;
        json response = api.getRecords(wordUrl);
        for (auto &pair : response)
        {
            string word = pair["word"].get<string>();
            if (count >= 3)
            {
                break;
            }
            if (nounReader->contains(word))
            {
                synonyms->push_back(word);
                count++;
            }
        }
        return synonyms;
    }
};

/*int main()
{
    SynonymAPI api;
    vector<string> *nouns = new vector<string>();
    nouns->push_back("horror");
    nouns->push_back("dark");
    nouns->push_back("monster");
    nouns->push_back("mystery");
    vector<string> *synonyms = api.getSynonyms(nouns);
    for (string word : *synonyms)
    {
        cout << word << " " << endl;
    }
    return 0;
}*/

#endif