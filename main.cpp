/* Driver class
 */

#include <string>

#include "BookIndexer.h"
#include "PhraseParser.h"
#include "MatchMaker.h"

string replace(string s, char current, char replace)
{
    int length = s.length();
    for (int i = 0; i < length; i++)
    {
        if (s[i] == current)
        {
            s[i] = replace;
        }
    }
    return s;
}

int main()
{
    MatchMaker *matchMaker = new MatchMaker("love flower relationship dream");
    vector<string> top;

    matchMaker->findSimilarities();
    // matchMaker->printMap();
    top = matchMaker->getTop();

    for (int i = 0; i < top.size(); i++)
    {
        cout << (i + 1) << ". " << replace(top.at(i), '-', ' ') << endl;
    }
    return 0;
}