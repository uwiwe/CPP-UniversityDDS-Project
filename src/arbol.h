#ifndef ARBOL_H
#define ARBOL_H

#include "mago.h"

struct NodoLista { // lista enlazada qwue guarda todos los magos, para poder conectar luego padre con hijos
    Mago* mago;
    NodoLista* next;
};

NodoLista* leerMagosCSV(const char* nombreArchivo);
Mago* construirArbol(NodoLista* lista);
Mago* buscarMagoId(NodoLista* lista, int id);

#endif
