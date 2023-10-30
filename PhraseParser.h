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
    vector<string> keywords;

    void mincePhrase();

public:
    PhraseParser(string phrase);  // recibe la frase del usuario
    vector<string> getKeywords(); // retona las palabras importantes de la frase
};

#endif