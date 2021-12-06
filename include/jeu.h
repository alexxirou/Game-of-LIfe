/**
 * @file jeu.h
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief fonctions d'évolution des cellules selons les régles du jeu de la vie
 * @version 0.11
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __JEU_H
#define __JEU_H
#include "grille.h"

/**
 * @brief // modulo modifié pour traiter correctement les bords i=0 et j=0  dans le calcul des voisins avec bords cycliques
 * 
 * @param i valeur du cellule
 * @param m modulo
 * @return int modulo 
 */
static inline int modulo(int i, int m) {return (i+m)%m;}

/**
 * @brief / compte le nombre de voisins vivants de la cellule (i,j) les bords sont cycliques.
 * 
 * @param i ligne
 * @param j colonne
 * @param g grille 
 * @return int nombre des voisins vivantes
 */
int compte_voisins_vivants (int i, int j, grille g);

/**
 * @brief fait évoluer la grille g d'un pas de temps
 * 
 * @param g grille
 * @param gc grille copie
 */
void evolue (grille *g, grille *gc, int age);

/**
 * @brief Pointeur de fonction de calcul voisinage cyclique/non cyclique
 * 
 */
extern int (*ptr_viv)(int ,  int , grille);

/**
 * @brief calcul voisinage non cyclique
 * 
 * @param i ligne de grille
 * @param j colonne de grille
 * @param g grille utilisée
 * @return int totale voisins vivantes
 */
int compte_voisins_vivants_non_cyc(int i, int j, grille g);

/**
 * @brief tester si une grille oscille
 * 
 * @param g grille à tester
 * @param gc grille copie
 * @param age mode viellissement
 * @param  pas_vers_oscillation liste [2] avec delai et pas oscillation initialisé à (0,0)
 * @return int -2 si pas d'oscillation ou >0 si oscillante. defaut -1 (non testé) 
 */
int* test_oscillation(grille *g, grille *gc, int age , int* pas_vers_oscillation); 
#endif

