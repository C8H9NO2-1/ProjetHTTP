#ifndef TOMBONUS_H
#define TOMBONUS_H

#include "structure.h"

bool checkLastHeader(char requete[], int *i, int longueur, Noeud *noeud);

bool checkField_Value(char requete[], int *i, int longueur, Noeud *noeud);

bool checkField_Content(char requete[], int *i, int longueur, Noeud *noeud);

bool checkObs_Fold(char requete[], int *i, int longueur, Noeud *noeud);

bool checkField_Vchar(char requete[], int *i, int longueur, Noeud *noeud);

bool checkVchar(char requete[], int *i, int longueur, Noeud *noeud);

bool checkObs_Text(char requete[], int *i, int longueur, Noeud *noeud);

#endif