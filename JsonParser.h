/* Clase para leer la informacion de los libros del json
 * Trabaja con patron singleton
 * Contiene un mapa con los autores y uno con los sustantivos
 */

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class JsonParser
{
private:
    json books;
    json bookInfo;
    static JsonParser *instance;                               // no acepta mas de una instancia de la clase
    unordered_map<string, string> *authors;                    // hash de autores <libro, autor>
    unordered_map<string, vector<pair<string, int>> *> *nouns; // hash de sustantivos <libro, sustantivos>

    JsonParser();
    void createMaps();

public:
    static JsonParser *getInstance();                                   // retorna la instancia de la clase
    string getAuthor(string bookName);                                  // retorna el autor de un libro
    vector<pair<string, int>> *getNouns(string bookName);               // retorna la lista de sustantivos de un libro
    unordered_map<string, vector<pair<string, int>> *> *getNounsHash(); // retorna el mapa de sustantivos
};

#endif