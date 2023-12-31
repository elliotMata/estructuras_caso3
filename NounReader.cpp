#include "NounReader.h"

NounReader::NounReader()
{
    nouns = new unordered_map<char, vector<string> *>();
    createMap();
}

NounReader *NounReader::getInstance()
{
    if (instance == nullptr)
    {
        instance = new NounReader();
    }
    return instance;
}

void NounReader::createMap()
{
    file.open("./common_words.txt");
    if (file.is_open())
    {
        string noun;
        while (getline(file, noun))
        {
            if (nouns->find(noun[0]) != nouns->end())
            {
                nouns->at(noun[0])->push_back(noun);
            }
            else
            {
                vector<string> *nounsList = new vector<string>();
                nounsList->push_back(noun);
                nouns->insert(make_pair(noun[0], nounsList));
            }
        }
        file.close();
    }
}

bool NounReader::contains(string noun)
{
    vector<string> *nounsList = getItem(noun[0]);
    if (!nounsList->empty())
    {
        for (const string &item : *nounsList)
        {
            if (item.compare(noun) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

vector<string> *NounReader::getItem(char key)
{
    return nouns->at(key);
}

NounReader *NounReader::instance = nullptr;

/*int main()
{
    NounReader *nounReader = NounReader::getInstance();
    vector<string> *nouns = nounReader->getItem('a');
    for (int i = 0; i < nouns->size(); i++)
    {
        cout << nouns->at(i) << " " << nouns->at(i).size() << endl;
    }
    cout << nounReader->contains("abeja") << endl;
    return 0;
}*/