#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Define the degree of the B-tree
const int DEGREE = 3;

// Structure representing the key
struct Key
{
    vector<int> *positions; // Contiene la posicion de los parrafos
    string *keyword;        // Contiene la keyword con la que se indexo el parrafo

    Key(string pKeyword, vector<int> *pPositions) : keyword(&pKeyword), positions(pPositions) {}
};

// B-tree node structure
struct BTreeNode
{
    int numKeys;                     // Number of keys currently stored in the node
    bool isLeaf;                     // Indicates if the node is a leaf node
    Key *keys[2 * DEGREE - 1];       // Array to hold keys (pointers to Key objects)
    BTreeNode *children[2 * DEGREE]; // Array to hold child pointers

    // Constructor to initialize a node
    BTreeNode()
    {
        numKeys = 0;
        isLeaf = true;
        for (int i = 0; i < 2 * DEGREE; ++i)
        {
            children[i] = nullptr;
            if (i < 2 * DEGREE - 1)
            {
                keys[i] = nullptr;
            }
        }
    }
};

class BTree
{
private:
    BTreeNode *root;

    vector<int> *searchAux(const string &keyword, BTreeNode *node)
    {
        if (!node)
            return nullptr;

        int index = node->numKeys - 1; // inicializa el indice asumiendo que la keyword buscada es "mayor" que las almacenadas
        while (index >= 0)
        {
            int result = keyword.compare(*(node->keys[index]->keyword)); // compara keyword de la key "mayor" con la buscada
            if (result >= 0)
            {
                if (result == 0)
                    return node->keys[index]->positions;                                         // si son iguales retorna el vector de posiciones de parrafos con esa keyword
                return (node->isLeaf) ? nullptr : searchAux(keyword, node->children[index + 1]); /* si es una hoja retorna nullptr porque no existe la palabra en el arbol
                                                                                                    si no es hoja retorna el resultado de la busqueda en el hijo derecho */
            }
            index--;
        }
        return (node->isLeaf) ? nullptr : searchAux(keyword, node->children[index + 1]); /* si recorrio todos los hijos retorna null si es hoja o el resultado de la busqueda
                                                                                            en el hijo "menor" si no es hoja*/
    }

public:
    BTree() : root(nullptr) {}

    void insert(Key *newKey)
    {
        if (root == nullptr)
        {
            root = new BTreeNode();
            root->keys[0] = newKey;
            root->numKeys = 1;
        }
        else
        {
            if (root->numKeys == 2 * DEGREE - 1)
            {
                BTreeNode *newRoot = new BTreeNode();
                newRoot->isLeaf = false;
                newRoot->children[0] = root;
                splitChild(newRoot, 0);
                int i = 0;
                if (newRoot->keys[0]->keyword->compare(*(newKey->keyword)) < 0)
                    i++;
                insertNonFull(newRoot->children[i], newKey);
                root = newRoot;
            }
            else
            {
                insertNonFull(root, newKey);
            }
        }
    }

    void insertNonFull(BTreeNode *node, Key *newKey)
    {
        int insertionIndex = node->numKeys - 1; // usa numKeys - 1 porque numKeys es la cantidad y la indexacion empieza en 0
        if (node->isLeaf)
        {
            while (insertionIndex >= 0 && newKey->keyword->compare(*(node->keys[insertionIndex]->keyword)) < 0) // compara lexicograficamente los keywords y se detiene en el indice 0
            {
                node->keys[insertionIndex + 1] = node->keys[insertionIndex--]; // hace corrimiento de los keys para dejar espacio para el nuevo key
            }
            if (newKey->keyword->compare(*(node->keys[insertionIndex]->keyword)) == 0)
            {
                for (const int &pos : *newKey->positions)
                {
                    node->keys[insertionIndex]->positions->push_back(pos);
                }
            }
            else
            {
                node->keys[insertionIndex + 1] = newKey; // inserta en la posicion correspondiente
                node->numKeys++;
            }
        }
        else
        {
            while (insertionIndex >= 0 && newKey->keyword->compare(*(node->keys[insertionIndex]->keyword)) < 0) // busca el indice en el que buscar el hijo
            {
                insertionIndex--;
            }

            if (node->children[++insertionIndex]->numKeys == 2 * DEGREE - 1) // verifica si la hoja esta llena
            {
                splitChild(node, insertionIndex);

                if (newKey->keyword->compare(*(node->keys[insertionIndex]->keyword)) > 0)
                {
                    insertionIndex++;
                }
            }

            insertNonFull(node->children[insertionIndex], newKey); // inserta en la hoja
        }
    }

    void splitChild(BTreeNode *parentNode, int index)
    {
        BTreeNode *child = parentNode->children[index];
        BTreeNode *newChild = new BTreeNode();
        newChild->isLeaf = child->isLeaf; // prepara el nuevo nodo
        newChild->numKeys = DEGREE - 1;   // prepara el nuevo nodo

        for (int i = 0; i < DEGREE - 1; i++)
        {
            newChild->keys[i] = child->keys[i + DEGREE]; // copia la segunda mitad de keys del nodo
        }

        if (!child->isLeaf)
        {
            for (int i = 0; i < DEGREE; i++)
            {
                newChild->children[i] = child->children[i + DEGREE]; // copia la seguna mitad de hijos del nodo
            }
        }

        child->numKeys = DEGREE - 1; // actualiza la nueva cantidad de keys del nodo

        for (int i = parentNode->numKeys; i > index; i--)
        {
            parentNode->children[i + 1] = parentNode->children[i]; // corre los hijos del nodo para la insercion del nuevo hijo
        }

        parentNode->children[index + 1] = newChild; // inserta el nuevo hijo a la derecha de la nueva key

        for (int i = parentNode->numKeys - 1; i >= index; i--)
        {
            parentNode->keys[i + 1] = parentNode->keys[i]; // corre las keys para la insercion de la key que sale del hijo
        }

        parentNode->keys[index] = child->keys[DEGREE - 1]; // inserta la key mediana del hijo (que ahora cuenta como basura porque la cantidad indica que no hay nada en ese indice)
        parentNode->numKeys++;                             // actualiza cantidad de keys
    }

    vector<int> *search(const string &keyword)
    {
        return searchAux(keyword, root);
    }
};

#endif