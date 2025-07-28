# Arbol Genealogico de Magos

Este proyecto permite interactuar con el arbol genealogico de magos. Fue desarrollado para la materia Estructuras Dinamicas de Datos.

## Estructura del proyecto

bin: hechizos.csv, magos.csv, programa.exe

src: arbol.h, arbol.cpp, hechizos.h, hechizos.cpp, herencia.h, herencia.cpp, mago.h, main.cpp

## Compilacion

Desde la raiz del proyecto, compilar con:

g++ src/*.cpp -o bin/programa

y luego

bin/programa

## Funcionalidades

- Leer archivo `.csv` con los magos  
- Crear el arbol genealogico  
- Ver la linea de sucesion  
- Editar datos de magos  
- Determinar heredero del hechizo