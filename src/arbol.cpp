#include <iostream>
#include <fstream> // leer archivos csv
#include <sstream> // dividir lineas
#include <string>
#include "arbol.h"

using namespace std;

// Lee el archivo csv y construye la lista enlazada de magos
NodoLista* leerMagosCSV(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo csv" << endl;
        return nullptr;
    }

    NodoLista* cabeza = nullptr;
    string linea;
    getline(archivo, linea); // Saltar encabezado

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;

        Mago* nuevo = new Mago;
        nuevo->left = nullptr;
        nuevo->right = nullptr;

        getline(ss, campo, ',');
        nuevo->id = stoi(campo); // stoi para convertir a entero, default lo lee como string

        getline(ss, campo, ',');
        nuevo->name = campo;

        getline(ss, campo, ',');
        nuevo->last_name = campo;

        getline(ss, campo, ',');
        nuevo->gender = campo[0];

        getline(ss, campo, ',');
        nuevo->age = stoi(campo);

        getline(ss, campo, ',');
        nuevo->id_father = (campo == "NULL") ? -1 : stoi(campo); // -1 si no tiene padre, es la raiz

        getline(ss, campo, ',');
        nuevo->is_dead = stoi(campo);

        getline(ss, campo, ',');
        nuevo->type_magic = campo;

        getline(ss, campo, ',');
        nuevo->is_owner = stoi(campo);

        NodoLista* nodo = new NodoLista{nuevo, cabeza}; // se inserta al inicio de la lista
        cabeza = nodo;
    }

    archivo.close();
    return cabeza;
}

// Busca un mago por su id en la lista enlazada
Mago* buscarMagoId(NodoLista* lista, int id) {
    NodoLista* actual = lista;
    while (actual) {
        if (actual->mago->id == id)
            return actual->mago;
        actual = actual->next;
    }
    return nullptr;
}

// Construye el arbol binario conectando padres con hijos
Mago* construirArbol(NodoLista* lista) {
    Mago* raiz = nullptr;
    NodoLista* actual = lista;

    while (actual) {
        Mago* hijo = actual->mago;
        if (hijo->id_father == -1) { // significa que es la raiz
            raiz = hijo;
        } else {
            Mago* padre = buscarMagoId(lista, hijo->id_father);
            if (padre) {
                if (!padre->left)
                    padre->left = hijo;
                else if (!padre->right)
                    padre->right = hijo;
                // Si ya tiene dos hijos se ignora este mago
            }
        }
        actual = actual->next;
    }

    return raiz;
}