#ifndef HECHIZOS_H
#define HECHIZOS_H

#include <string>
#include "mago.h"
#include "arbol.h"

void agregarHechizo(Mago* mago, const std::string& hechizo);
void guardarHechizoCSV(const std::string& ruta, int id_mago, const std::string& hechizo);
void cargarHechizosDesdeCSV(const std::string& ruta, NodoLista* lista);
void transferirHechizos(Mago* from, Mago* to);
void mostrarHechizos(Mago* mago);
void reescribirArchivoHechizos(const std::string& ruta, NodoLista* lista);

#endif