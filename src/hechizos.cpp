#include <iostream>
#include <fstream>
#include <sstream>
#include "hechizos.h"

using namespace std;

// Agrega un nuevo hechizo al final de la lista de hechizos del mago
void agregarHechizo(Mago* mago, const string& hechizo) {
    if (!mago) return;

    Hechizo* nuevo = new Hechizo{hechizo, nullptr};

    if (!mago->hechizos) {
        mago->hechizos = nuevo; // si no tiene ninguno este sera el primero
    } else {
        Hechizo* actual = mago->hechizos;
        while (actual->next)            // Avanza al final de la lista
            actual = actual->next;
        actual->next = nuevo;           // anadir el nuevo hechizo al final
    }
}

// Guarda un hechizo en el archivo csv en nueva linea
void guardarHechizoCSV(const string& ruta, int id_mago, const string& hechizo) {
    ofstream archivo(ruta, ios::app);   // abrir el archivo como append
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de hechizos" << endl;
        return;
    }
    archivo << id_mago << "," << hechizo << endl; // se escribe id y hechizo separados por coma
    archivo.close();
}

// Cargar todos los hechizos desde csv y se asignan al mago correspondiente
void cargarHechizosDesdeCSV(const string& ruta, NodoLista* lista) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "No se encontro el archivo de hechizos (se creara cuando se agregue uno)" << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);       // conviertir la linea en un stream para extraer campos
        string campo;
        getline(ss, campo, ',');      // lee id
        int id = stoi(campo);
        getline(ss, campo, ',');      // Lee el hechizo

        Mago* mago = buscarMagoId(lista, id);
        if (mago)
            agregarHechizo(mago, campo); // Se Asigna el hechizo al mago si lo encuentra
    }

    archivo.close();
}

// Transfiere todos los hechizos de un mago a otro y deja vacio al primero
void transferirHechizos(Mago* from, Mago* to) {
    if (!from || !to || from == to) return;

    Hechizo* actual = from->hechizos;
    while (actual) {
        agregarHechizo(to, actual->nombre); // Copia cada hechizo al nuevo mago
        actual = actual->next;
    }

    from->hechizos = nullptr; // se le borran los hechizos al mago anterior
}

// Muestra todos los hechizos que tiene un mago
void mostrarHechizos(Mago* mago) {
    if (!mago || !mago->hechizos) {
        cout << "Este mago no tiene hechizos registrados" << endl;
        return;
    }

    cout << "Hechizos de " << mago->name << " " << mago->last_name << ": " << endl;
    Hechizo* actual = mago->hechizos;
    while (actual) {
        cout << " - " << actual->nombre << "\n";
        actual = actual->next;
    }
}