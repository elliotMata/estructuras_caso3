#ifndef PARAGRAPH_RANKING_H_
#define PARAGRAPH_RANKING_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;

class ParagraphRanking
{
private:
    double tfValue, idfValue;

    void calculateTF(int totalRepetitions, int totalWords)
    {
        tfValue = static_cast<double>(totalRepetitions) / totalWords;
    }

    void calculateIDF(int amountAppearances, int totalParagraphs)
    {
        idfValue = log(static_cast<double>(amountAppearances) / totalParagraphs);
    }

public:
    double getRelevance(int totalRepetitions, int totalWords, int amountAppearances, int totalParagraphs)
    {
        calculateTF(totalRepetitions, totalWords);
        calculateIDF(amountAppearances, totalParagraphs);
        return tfValue * idfValue;
    }
};

#endif