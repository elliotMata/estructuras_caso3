#include "NounReader.h"

NounReader::NounReader()
{
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
    file.open("/home/steph/Documents/estructuras/caso03/estructuras_caso3/lista-sustantivos.txt");
    if (file.is_open())
    {
        string noun;
        while (getline(file, noun))
        {
            if (nouns.find(noun[0]) != nouns.end())
            {
                nouns.at(noun[0]).push_back(noun);
            }
            else
            {
                vector<string> nounsList;
                nounsList.push_back(noun);
                nouns.insert(make_pair(noun[0], nounsList));
            }
        }
        file.close();
    }
}

bool NounReader::contains(string noun)
{
    vector<string> nounsList = getItem(noun[0]);
    if (!nounsList.empty())
    {
        for (int i = 0; i < nounsList.size(); i++)
        {
            if (nounsList.at(i).compare(noun))
            {
                return true;
            }
        }
    }
    return false;
}

vector<string> NounReader::getItem(char key)
{
    return nouns.at(key);
}

NounReader *NounReader::instance = nullptr;

/*int main ()
{
    NounReader* nounReader = NounReader::getInstance();
    vector<string> nouns = nounReader->getItem('a');
    for (int i = 0; i < nouns.size(); i++)
    {
        cout << nouns.at(i) << endl;
    }
    cout << nounReader->contains("love") << endl;
    return 0;
}*/