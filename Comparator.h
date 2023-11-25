#ifndef COMPARATOR_H_
#define COMPARATOR_H_

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Comparator
{
private:
    int matches, totalWords;
    double matchPercentage;
    vector<string> base, paragraphTarget;
    vector<pair<string, int>> target;

    void compare()
    {
        matches = 0;
        totalWords = 0;

        for (int i = 0; i < base.size(); i++)
        {
            for (int j = 0; j < target.size(); j++)
            {
                totalWords += target.at(j).second;
                if (target.at(j).first.compare(base.at(i)) == 0)
                {
                    matches += target.at(j).second;
                }
            }
        }

        matchPercentage = 0.0;
        if (target.size() > 0)
        {
            matchPercentage = (static_cast<double>(matches) / totalWords) * 100.0;
        }
        cout << "match percentage " << matchPercentage << endl;
    }

    void compareParagraphs()
    {
        matches = 0;
        for (const string &item : base)
        {
            if (find(paragraphTarget.begin(), paragraphTarget.end(), item) != paragraphTarget.end())
            {
                matches++;
            }
        }

        matchPercentage = 0.0;
        if (paragraphTarget.size() > 0)
        {
            matchPercentage = (static_cast<double>(matches) / base.size()) * 100.0;
        }
    }

public:
    void compareVectors(const vector<string> &vector1, const vector<pair<string, int>> &vector2)
    {
        base = vector1;
        target = vector2;
        compare();
    }

    void compareVectors(const vector<string> &vector1, const vector<string> &vector2)
    {
        base = vector1;
        paragraphTarget = vector2;
        compareParagraphs();
    }

    double getSimilarity()
    {
        return matchPercentage;
    }
};

#endif