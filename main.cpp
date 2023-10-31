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
    vector<string> topThree;

    matchMaker->findSimilarities();
    // matchMaker->printMap();
    topThree = matchMaker->getTopThree();

    for (int i = 0; i < topThree.size(); i++)
    {
        cout << (i + 1) << ". " << replace(topThree.at(i), '-', ' ') << endl;
    }
    cout << topThree.size() << endl;
    return 0;
}