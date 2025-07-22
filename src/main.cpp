#include <iostream>
#include "arbol.h"
#include "hechizos.h"

using namespace std;

int main() {
    const char* archivo = "bin/magos.csv";

    NodoLista* lista = leerMagosCSV(archivo);
    if (!lista) return 1;

    Mago* raiz = construirArbol(lista);

    if (raiz)
        cout << "Arbol de magos construido correctamente. raiz: " << raiz->name << " " << raiz->last_name << endl;
    else
        cout << "No se encontro raiz." << endl;

    cargarHechizosDesdeCSV("bin/hechizos.csv", lista);

    // Probar agregar uno manualmente
    int id = 5;
    Mago* mago = buscarMagoId(lista, id);
    if (mago) {
        string hechizo = "Bola de nieve";
        agregarHechizo(mago, hechizo);
        guardarHechizoCSV("bin/hechizos.csv", mago->id, hechizo);
        mostrarHechizos(mago);
    }

    return 0;
}