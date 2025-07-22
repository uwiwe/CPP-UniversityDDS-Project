#ifndef MAGO_H
#define MAGO_H

#include <string>

struct Hechizo {
    std::string nombre;
    Hechizo* next;
};

struct Mago {
    int id;
    std::string name;
    std::string last_name;
    char gender;
    int age;
    int is_dead;
    std::string type_magic;
    int is_owner;

    Mago* left;  // primer discipulo
    Mago* right; // segundo discipulo
    int id_father;

    Hechizo* hechizos = nullptr; // en este campo se guardaran los hechizos extraidos del csv
};

#endif