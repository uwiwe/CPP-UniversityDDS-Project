#include <iostream>
#include "arbol.h"
#include "hechizos.h"
#include "herencia.h"

using namespace std;

int main() {
    const char* archivoMagos = "bin/magos.csv";
    const char* archivoHechizos = "bin/hechizos.csv";

    NodoLista* lista = leerMagosCSV(archivoMagos);
    if (!lista) return 1;

    Mago* raiz = construirArbol(lista);

    if (!raiz) {
        cout << "No se encontro una raiz valida." << endl;
        return 1;
    }

    cout << "Arbol de magos construido correctamente. raiz: " << raiz->name << " " << raiz->last_name << endl;

    cargarHechizosDesdeCSV(archivoHechizos, lista);

    Mago* dueno = getDuenoActual(lista);
    if (dueno && !dueno->hechizos) {
    agregarHechizo(dueno, "Hechizo original");
    reescribirArchivoHechizos(archivoHechizos, lista); // guardar en csv
    }

    int opcion;
    do { // do-while hasta que el usuario decida salir del programa
        cout << "Bienvenido al menu" << endl;
        cout << "1. Agregar hechizo a un mago" << endl;
        cout << "2. Mostrar hechizos de un mago" << endl;
        cout << "3. Imprimir el arbol actual en preorden" << endl;
        cout << "4. Simular muerte del dueno actual y reasignar" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int id;
            string hechizo;
            cout << "Ingrese ID del mago: ";
            cin >> id;
            cin.ignore(); // para limpiar entrada y no de error
            cout << "Ingrese nombre del hechizo: ";
            getline(cin, hechizo);

            Mago* mago = buscarMagoId(lista, id);
            if (!mago) {
                cout << "Mago no encontrado.\n";
            } else {
                agregarHechizo(mago, hechizo);
                reescribirArchivoHechizos(archivoHechizos, lista); // guardar estado completo
                cout << "Hechizo agregado correctamente.\n";
            }

        } else if (opcion == 2) {
            int id;
            cout << "Ingrese ID del mago: ";
            cin >> id;

            Mago* mago = buscarMagoId(lista, id);
            if (!mago) {
                cout << "Mago no encontrado.\n";
            } else {
                mostrarHechizos(mago);
            }
        } else if (opcion == 3) {
            cout << "Arbol impreso en preorden: " << endl;
            mostrarArbolPreorden(raiz);
        } else if (opcion == 4) {
            Mago* actual = getDuenoActual(lista);
            actual->is_dead = 1; // solo se cambia dentro de la lista enlazada que he creado
            cout << actual->name << " ha muerto.\n";

            Mago* nuevo = buscarReemplazoConCompanero(lista, actual);
            if (nuevo) {
                reasignarDueno(actual, nuevo);
                reescribirArchivoHechizos(archivoHechizos, lista);
            } else {
                cout << "No se encontro un reemplazo adecuado" << endl;
            }

        }

    } while (opcion != 5);

    cout << "Programa finalizado.\n";
    return 0;
}