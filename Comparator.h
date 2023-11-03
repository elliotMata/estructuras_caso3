#ifndef COMPARATOR_H_
#define COMPARATOR_H_

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Comparator
{
private:
    int matches;
    double matchPercentage;
    vector<string> base, target;

    void compare()
    {
        matches = 0;
        for (const string &item : base)
        {
            if (find(target.begin(), target.end(), item) != target.end())
            {
                matches++;
            }
        }

        matchPercentage = 0.0;
        if (target.size() > 0)
        {
            matchPercentage = (static_cast<double>(matches) / target.size()) * 100.0;
        }
    }

    // hola

public:
    void compareVectors(const vector<string> &vector1, const vector<string> &vector2)
    {
        base = (vector1.size() <= vector2.size() ? vector1 : vector2);
        target = (vector2.size() >= vector1.size() ? vector2 : vector1);
        compare();
    }

    double getSimilarity()
    {
        return matchPercentage;
    }
};

#endif