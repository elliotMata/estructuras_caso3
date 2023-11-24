#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;

double calculateTF(const string &term, const unordered_map<string, int> &wordCounts)
{
    int totalWords = 0;
    for (const auto &entry : wordCounts)
    {
        totalWords += entry.second;
    }

    if (wordCounts.count(term))
    {
        return static_cast<double>(wordCounts.at(term)) / totalWords;
    }
    return 0.0;
}

// Function to calculate Inverse Document Frequency
double calculateIDF(const string &term, const vector<unordered_map<string, int>> &allWordCounts)
{
    int documentCount = allWordCounts.size();
    int documentsWithTerm = 0;

    for (const auto &wordCount : allWordCounts)
    {
        if (wordCount.count(term))
        {
            documentsWithTerm++;
        }
    }

    if (documentsWithTerm > 0)
    {
        return log(static_cast<double>(documentsWithTerm) / (documentCount));
    }
    return 0.0;
}

// Function to calculate TF-IDF
double calculateTFIDF(const string &term, const unordered_map<string, int> &wordCounts,
                      const vector<unordered_map<string, int>> &allWordCounts)
{
    double tf = calculateTF(term, wordCounts);
    double idf = calculateIDF(term, allWordCounts);

    return tf * idf;
}

int main()
{
    vector<string> paragraphs = {
        "love friendship romance sweet tradition marriage",
        "love feminist sister friendship romance books"};

    // Tokenize and count words for each paragraph
    vector<unordered_map<string, int>> allWordCounts;
    for (const auto &paragraph : paragraphs)
    {
        unordered_map<string, int> wordCounts;
        string word;
        istringstream iss(paragraph);
        while (iss >> word)
        {
            // Remove punctuation (for simplicity)
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            wordCounts[word]++;
        }
        allWordCounts.push_back(wordCounts);
    }

    // Calculate TF-IDF for each term in each paragraph
    for (int i = 0; i < paragraphs.size(); ++i)
    {
        cout << "TF-IDF for terms in paragraph " << i + 1 << ":" << endl;
        for (const auto &wordCount : allWordCounts[i])
        {
            const string &term = wordCount.first;
            double tfidf = calculateTFIDF(term, allWordCounts[i], allWordCounts);
            cout << "Term: " << term << ", TF-IDF: " << tfidf << endl;
        }
        cout << endl;
    }

    return 0;
}