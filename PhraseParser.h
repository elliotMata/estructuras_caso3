/* Clase para obtener las palabras claves de una frase
 * Las palabras claves quedan almacenadas en un vector
 */

#ifndef PHRASE_PARSER_H_
#define PHRASE_PARSER_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PhraseParser
{
private:
    string phrase;
    vector<string> *keywords;
    int totalWords;

    void mincePhrase(string phrase);
    string phraseCleaner(const string &phrase);

public:
    vector<string> *getKeywords(string phrase); // retona las palabras importantes de la frase
    int getTotalWords();
};

#endif