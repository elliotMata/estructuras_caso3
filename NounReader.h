/* Clase para leer los sustantivos del archivo
 * Trabaja con patron singleton
 * Contiene un mapa con los sustantivos organizados alfabeticamente
 */

#ifndef NOUN_READER_H_
#define NOUN_READER_H_

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class NounReader
{
private:
    fstream file;
    static NounReader *instance;               // no acepta mas de una instancia de la clase
    unordered_map<char, vector<string>> nouns; // hash de sustantivos <caracter, lista de sustantivos>

    NounReader();
    void createMap();

public:
    static NounReader *getInstance(); // retorna la instancia de la clase
    bool contains(string noun);       // retorna si el sustantivo se encuentra en la lista
    vector<string> getItem(char key); // retorna la lista de sustantivos correspondiente al caracter
};

#endif