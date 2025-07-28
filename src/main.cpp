#include <iostream>
#include "arbol.h"
#include "hechizos.h"
#include "herencia.h"

using namespace std;

void modificarDatosMago(NodoLista* lista) {
    int id;
    cout << "Ingrese el id del mago a modificar: ";
    cin >> id;
    Mago* m = buscarMagoId(lista, id);

    if (!m) {
        cout << "Mago no encontrado." << endl;
        return;
    }

    cout << "Datos del mago:" << endl;
    cout << "name: " << m->name << endl;
    cout << "last_name: " << m->last_name << endl;
    cout << "age: " << m->age << endl;
    cout << "gender: " << m->gender << endl;
    cout << "type_magic: " << m->type_magic << endl;
    cout << "is_dead: " << m->is_dead << endl;
    cout << "is_owner: " << m->is_owner << endl;

    string entrada;
    cout << "Desea cambiar el nombre? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nuevo nombre: "; cin >> m->name;
    }

    cout << "Desea cambiar el apellido? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nuevo apellido: "; cin >> m->last_name;
    }

    cout << "Desea cambiar la edad? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nueva edad: "; cin >> m->age;
    }

    cout << "Desea cambiar el estado vivo/muerto? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nuevo estado: "; cin >> m->is_dead;
    }

    cout << "Desea cambiar el tipo de magia (elemental, unique, mixed, no_magic)? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nuevo tipo de magia: "; cin >> m->type_magic;
    }

    cout << "Desea cambiar el genero (H o M)? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nuevo genero: "; cin >> m->gender;
    }

    cout << "Desea cambiar si es dueno del hechizo (0 o 1)? (s o n): "; cin >> entrada;
    if (entrada == "s") {
        cout << "Nuevo estado de dueno: "; cin >> m->is_owner;
    }

    cout << "Datos modificados correctamente." << endl;
}

void mostrarLineaDeSucesion(Mago* raiz, NodoLista* lista) {
    int idsMuertosTemp[100];
    int total = 0;

    Mago* actual = getDuenoActual(lista);
    if (!actual) {
        cout << "No hay un dueno actual del hechizo." << endl;
        return;
    }

    string hechizoClave = (actual->hechizos) ? actual->hechizos->nombre : "Hechizo original";

    cout << "Linea de sucesion a seguir:" << endl;

    while (actual) {
        cout << actual->id << "- " << actual->name << " " << actual->last_name << endl;

        // Guardar id antes de simular muerte
        idsMuertosTemp[total++] = actual->id;
        actual->is_dead = 1;

        Mago* siguiente = encontrarReemplazoFinal(lista, actual, raiz, hechizoClave);
        if (!siguiente) break;

        actual = siguiente;
    }

    // Restaurar estado original de is_dead
    NodoLista* nodo = lista;
    while (nodo) {
        for (int i = 0; i < total; ++i) {
            if (nodo->mago->id == idsMuertosTemp[i]) {
                nodo->mago->is_dead = 0;
                break;
            }
        }
        nodo = nodo->next;
    }
}


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
        historial.registrar("Hechizo original", dueno->id);
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
        cout << "6. Modificar datos de un mago" << endl;
        cout << "7. Mostrar linea de sucesion actual" << endl;
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
                cout << "Mago no encontrado." << endl;
            } else {
                agregarHechizo(mago, hechizo);
                reescribirArchivoHechizos(archivoHechizos, lista); // guardar estado completo
                cout << "Hechizo agregado correctamente." << endl;
            }

        } else if (opcion == 2) {
            int id;
            cout << "Ingrese ID del mago: ";
            cin >> id;

            Mago* mago = buscarMagoId(lista, id);
            if (!mago) {
                cout << "Mago no encontrado." << endl;
            } else {
                mostrarHechizos(mago);
            }
        } else if (opcion == 3) {
            cout << "Arbol impreso en preorden: " << endl;
            mostrarArbolPreorden(raiz);
        } else if (opcion == 4) {
            Mago* actual = getDuenoActual(lista);

            verificarEdad(actual, lista);
            
            actual->is_dead = 1;
            cout << actual->name << " ha muerto." << endl;

            string hechizoClave;
            if (actual->hechizos) {
                hechizoClave = actual->hechizos->nombre;
            } else {
                hechizoClave = "Hechizo original";
            }
            Mago* nuevo = encontrarReemplazoFinal(lista, actual, raiz, hechizoClave);

            if (nuevo) {
                reasignarDueno(actual, nuevo);
                reescribirArchivoHechizos(archivoHechizos, lista);
            } else {
                cout << "No se encontro un reemplazo adecuado" << endl;
            }
        } else if (opcion == 6) {
            Mago* actual = getDuenoActual(lista);
            modificarDatosMago(lista);
            verificarEdad(actual, lista);
        } else if (opcion == 7) {
            mostrarLineaDeSucesion(raiz, lista);
        }

    } while (opcion != 5);

    cout << "Programa finalizado." << endl;
    return 0;
}