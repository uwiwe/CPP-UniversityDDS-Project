#include "herencia.h"
#include <iostream>

using namespace std;

HistorialHechizos historial; // variable global que lleva el registro

// Devuelve el mago actual que tiene el hechizo
Mago* getDuenoActual(NodoLista* lista) {
    NodoLista* actual = lista;
    while (actual) {
        if (actual->mago->is_owner == 1)
            return actual->mago;
        actual = actual->next;
    }
    return nullptr;
}

// Recorre el subarbol del dueno actual buscando el reemplazo segun la primera regla
Mago* buscarReemplazoPrimerRegla(Mago* raiz) {
    if (!raiz) return nullptr;

    Queue q;
    q.enqueue(raiz); // metemos la raiz en la cola, y luego le sigue todos sus hijos, que los accedemos por left, right

    Mago* candidatoMixed = nullptr;
    Mago* candidatoMale = nullptr;

    while (!q.isEmpty()) {
        Mago* current = q.dequeue(); // vamos sacando los nodos y se verifica si cumple las condiciones

        if (current->is_dead == 0) {
            if (current->type_magic == "elemental" || current->type_magic == "unique")
                return current;
            if (!candidatoMixed && current->type_magic == "mixed")
                candidatoMixed = current;
            if (!candidatoMale && current->gender == 'H')
                candidatoMale = current;
        }

        if (current->left)
            q.enqueue(current->left); // se hace lo mismo para sus hijos
        if (current->right)
            q.enqueue(current->right);
    }

    if (candidatoMixed) return candidatoMixed;
    if (candidatoMale) return candidatoMale;
    return nullptr;
}

// Quita el hechizo al anterior dueno y se lo da al nuevo
void reasignarDueno(Mago* anterior, Mago* nuevo) {
    if (!anterior || !nuevo || anterior == nuevo) return;

    anterior->is_owner = 0;
    nuevo->is_owner = 1;

    transferirHechizos(anterior, nuevo);

    Hechizo* h = nuevo->hechizos;
    while (h) {
        historial.registrar(h->nombre, nuevo->id);
        h = h->next;
    }

    cout << "El nuevo dueno del hechizo es: " << nuevo->id << ". " << nuevo->name << " " << nuevo->last_name << endl;
}

Mago* buscarCompanero(NodoLista* lista, Mago* yo) {
    if (!yo || yo->id_father == -1) return nullptr; // -1 es el nodo raiz del proyecto, no tiene hermano

    NodoLista* actual = lista;
    while (actual) {
        Mago* m = actual->mago;
        if (m->id_father == yo->id_father && m->id != yo->id) // buscamos hermano, si es que tiene. sino terminara retornando nullptr
            return m;
        actual = actual->next;
    }
    return nullptr;
}

// Condicion 2 y demas condiciones juntas
Mago* buscarReemplazoConCompanero(NodoLista* lista, Mago* dueno) {
    if (!dueno) return nullptr;

    // caso condicion 1 Tiene hijos
    if (dueno->left || dueno->right)
        return buscarReemplazoPrimerRegla(dueno);

    // caso 2 Buscar companero
    Mago* companero = buscarCompanero(lista, dueno);

    // Si no tiene companero, aplicar directamente la tercera condicion
    if (!companero) {
        return buscarCompaneroDelMaestro(lista, dueno);
    }

    // caso 3 Companero vivo con misma magia
    if (companero->is_dead == 0 && companero->type_magic == dueno->type_magic) {
        return companero; 
    }

    // caso 4 Companero muerto y sin hijos ir a companero del maestro
    if (!tieneHijos(companero) && companero->is_dead == 1) {
        return buscarCompaneroDelMaestro(lista, dueno);
    }

    // caso 5: Buscar en subarbol del companero
    Mago* subarbol = buscarReemplazoPrimerRegla(companero); // nos adentramos y si encuentra algo lo retorna
    if (subarbol)
        return subarbol;

    // caso 6 Si todo falla, subir en el arbol y repetir logica
    return subirYNavegarHerencia(lista, dueno);
}

bool tieneHijos(Mago* m) {
    return m && (m->left || m->right);
}

// Condicion: Si el companero no esta vivo y no tiene hijos, el dueno del hechizo es el companero de su maestro independientemente de la magia que posea.
Mago* buscarCompaneroDelMaestro(NodoLista* lista, Mago* yo) {
    if (!yo || yo->id_father == -1) return nullptr;

    Mago* maestro = buscarMagoId(lista, yo->id_father); // buscamos al padre
    if (!maestro || maestro->id_father == -1) return nullptr;

    // buscar companero del padre
    NodoLista* actual = lista;
    while (actual) {
        Mago* m = actual->mago;
        if (m->id_father == maestro->id_father && m->id != maestro->id && m->is_dead == 0) // tienen el mismo padre y son diferentes
            return m; // entonces m es el companero
        actual = actual->next;
    }

    return nullptr;
}

// Condicion: Si la condicion de arriba no se puede cumplir porque el maestro esta muerto, el hechizo se pasa siguiendo las condiciones desde la primera hasta la segunda.
Mago* subirYNavegarHerencia(NodoLista* lista, Mago* origen) {
    Mago* actual = origen;

    while (actual && actual->id_father != -1) { // no son la raiz del arbol completo (no tendrian padre)
        Mago* padre = buscarMagoId(lista, actual->id_father);
        if (!padre) break;

        // Usar misma logica de reemplazo desde ese punto hacia abajo
        Mago* intento = buscarReemplazoConCompanero(lista, padre);
        if (intento) return intento;

        // Si no encuentra heredero, sigue subiendo
        actual = padre;
    }

    return nullptr;
}

Mago* buscarMujerMasJovenConCondiciones(Mago* raiz, NodoLista* lista, const std::string& nombre_hechizo) {
    if (!raiz) return nullptr;

    Queue q;
    q.enqueue(raiz);

    Mago* candidataConCondicion = nullptr;
    Mago* candidataGeneral = nullptr;

    while (!q.isEmpty()) {
        Mago* actual = q.dequeue();
        // std::cout << "Evaluando nodo id" << actual->id << " - " << actual->name 
        //   << ", genero: " << actual->gender << ", edad: " << actual->age 
        //   << ", viva: " << (actual->is_dead == 0 ? "si" : "no") << endl;

        if (actual->left) q.enqueue(actual->left);
        if (actual->right) q.enqueue(actual->right);

        if (actual->gender == 'M' && actual->is_dead == 0) {
            // opcion de mujer mas joven
            if (!candidataGeneral || actual->age < candidataGeneral->age)
                candidataGeneral = actual;

            // si tiene discipulos y cumple las demas condiciones, es la elegida
            if (actual->left || actual->right) {
                Mago* maestro = buscarMagoId(lista, actual->id_father);
                if (maestro && maestro->type_magic == "mixed" &&
                    historial.fueDueno(nombre_hechizo, maestro->id)) {

                    if (!candidataConCondicion || actual->age < candidataConCondicion->age)
                        candidataConCondicion = actual;
                }
            }
        }
    }

    if (candidataConCondicion != nullptr) {
        return candidataConCondicion;
    } else {
        // cout << "No se encontro ningun reemplazo con las condiciones";
        return candidataGeneral;
    }
}


Mago* encontrarReemplazoFinal(NodoLista* lista, Mago* dueno, Mago* raiz, const std::string& hechizo) {
    Mago* candidato = buscarReemplazoConCompanero(lista, dueno);
    if (candidato) return candidato;

    return buscarMujerMasJovenConCondiciones(raiz, lista, hechizo);
}

void verificarEdad(Mago* actual, NodoLista* lista) {
    if (!actual || actual->is_dead || actual->age <= 70) return;

    Mago* igualMagia = nullptr;
    Mago* masViejo = nullptr;

    if (actual->left && actual->left->is_dead == 0) {
        if (actual->left->type_magic == actual->type_magic)
            igualMagia = actual->left;
        if (!masViejo || actual->left->age > masViejo->age)
            masViejo = actual->left;
    }

    if (actual->right && actual->right->is_dead == 0) {
        if (!igualMagia && actual->right->type_magic == actual->type_magic)
            igualMagia = actual->right;
        if (!masViejo || actual->right->age > masViejo->age)
            masViejo = actual->right;
    }

    Mago* nuevo = igualMagia ? igualMagia : masViejo;
    if (nuevo) {
        reasignarDueno(actual, nuevo);
        cout << "El dueno paso de los 70 anos. El hechizo se paso al discipulo mas apto." << endl;
    }
}
