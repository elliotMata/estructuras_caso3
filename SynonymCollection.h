#ifndef SYNONYM_COLLECTION_H_
#define SYNONYM_COLLECTION_H_

#include "SynonymAPI.h"

class SynonymCollection
{

private:
    unordered_map<string, vector<string> *> *synonyms;
    SynonymAPI api;
    NounReader *nounReader;

    void createMap()
    {
        char character;
        for (character = 'a'; character <= 'z'; character++)
        {
            vector<string> *nouns = nounReader->getItem(character);
            for (int position = 0; position < nouns->size(); position++)
            {
                vector<string> *wordSynonyms = api.getSynonyms(nouns->at(position));
                if (wordSynonyms != nullptr)
                {
                    cout << "adding word " << nouns->at(position) << endl;
                    synonyms->insert({nouns->at(position), wordSynonyms});
                }
            }
        }
    }

public:
    SynonymCollection()
    {
        nounReader = NounReader::getInstance();
        synonyms = new unordered_map<string, vector<string> *>();
        createMap();
    }

    bool hasSynonyms(string noun)
    {
        return synonyms->at(noun) != nullptr;
    }

    vector<string> *getSynonyms(string noun)
    {
        return synonyms->at(noun);
    }
};

#endif