#ifndef HERENCIA_H
#define HERENCIA_H

#include "arbol.h"
#include "hechizos.h"

struct QueueNode { // propiedades de cada nodo de la cola. es un nodo de la cola
    Mago* mago;
    QueueNode* next;
};

struct Queue { // estructura de la cola: dos punteros: front, rear == head, last
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;

    void enqueue(Mago* m) {
        QueueNode* newNode = new QueueNode{m, nullptr};
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode; // cola con puntero de head y de last
            rear = newNode;
        }
    }

    Mago* dequeue() {
        if (!front) return nullptr;
        QueueNode* temp = front;
        Mago* m = temp->mago;
        front = front->next;
        if (!front) { rear = nullptr; }
        return m; // retornar el que se ha eliminado
    }

    bool isEmpty() {
        return front == nullptr; // true o false
    }
};

struct Registro {
    std::string hechizo; // nombre del hechizo
    int duenos[20]; // ids de los magos que han sido duenos
    int cantidad = 0; // cantidad de duenos registrados
};

struct HistorialHechizos { // para llevar un registro de quien ha sido dueno de los hechizos. hace falta para la condicion de maestro de la mujer
    Registro registros[100];
    int total = 0;

    void registrar(const std::string& nombre, int id) {
        for (int i = 0; i < total; ++i) {
            if (registros[i].hechizo == nombre) {
                for (int j = 0; j < registros[i].cantidad; ++j) {
                    if (registros[i].duenos[j] == id)
                        return;
                }
                registros[i].duenos[registros[i].cantidad++] = id; // lo incluimos si no esta 
                return;
            }
        }
        // si el hechizo no existe, se agrega en el registro
        registros[total].hechizo = nombre; 
        registros[total].duenos[0] = id;
        registros[total].cantidad = 1;
        ++total; // se registra un nuevo hechizo
    }

    bool fueDueno(const std::string& nombre, int id) const {
        for (int i = 0; i < total; ++i) {
            if (registros[i].hechizo == nombre) {
                for (int j = 0; j < registros[i].cantidad; ++j) {
                    if (registros[i].duenos[j] == id)
                        return true;
                }
            }
        }
        return false;
    }
};

Mago* getDuenoActual(NodoLista* lista);
Mago* buscarReemplazoPrimerRegla(Mago* raiz);
void reasignarDueno(Mago* anterior, Mago* nuevo);
Mago* buscarReemplazoConCompanero(NodoLista* lista, Mago* dueno);
Mago* buscarCompanero(NodoLista* lista, Mago* yo);
bool tieneHijos(Mago* m);
Mago* buscarCompaneroDelMaestro(NodoLista* lista, Mago* yo);
Mago* subirYNavegarHerencia(NodoLista* lista, Mago* origen);
Mago* buscarMujerMasJovenConCondiciones(Mago* raiz, NodoLista* lista, const std::string& nombre_hechizo);
Mago* encontrarReemplazoFinal(NodoLista* lista, Mago* dueno, Mago* raiz, const std::string& hechizo);
void verificarEdad(Mago* actual, NodoLista* lista);

extern HistorialHechizos historial; // variable global para poder usarse en main
#endif