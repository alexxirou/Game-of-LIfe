/**
 * @file grille.h
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief fonctions qui manipulent les grilles utilisées par le programme
 * @version 0.11
 * @date 2021-10-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __GRILLE_H
#define __GRILLE_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/**
 * @brief structure de données de tableau avec pointeur vers les champs
 * @param nbl nombre des lignes du tableau
 * @param nbc nombre des colonnes du tableau
 * @param cellules pointeur valeur du champ du tableau
 */
typedef struct {int nbl; int nbc; int** cellules;} grille;
/**
 * @brief fonction allouer espace mémoire à une grille g
 * 
 * @param l nombre de lignes du tableau
 * @param c nombr de collones du tableau
 * @param g pointeur vers le tableau
 */
void alloue_grille (int l, int c, grille* g);

/**
 * @brief liberer la memoire allouée à une grille g
 * 
 * @param g pointeur de type 'grille' vers la grille 
 */
void libere_grille (grille* g);



/**
 * @brief alloue et initalise la grille g à partir d'un fichier
 * 
 * @param filename fichier utilisé
 * @param g grille ciblée
 */
void init_grille_from_file (char * filename, grille* g);

/**
 * @brief rend vivante la cellule (i,j) de la grille g
 * 
 * @param i ligne
 * @param j colonne
 * @param g grille
 */
static inline void set_vivante(int i, int j, grille g){g.cellules[i][j] = 1;}

/**
 * @brief Met une cellule non viable 
 * 
 * @param i lignes
 * @param j colonnes
 * @param g grille
 */

static inline void set_non_viable(int i, int j, grille g){g.cellules[i][j] = -1;}


/**
 * @brief rend morte la cellule (i,j) de la grille g
 * 
 * @param i ligne
 * @param j colonne
 * @param g grille
 */
static inline void set_morte(int i, int j, grille g){g.cellules[i][j] = 0;}

/**
 * @brief teste si la cellule (i,j) de la grille g est vivante
 * 
 * @param i ligne
 * @param j colonne
 * @param g grille
 * @return int 1 si vivante 0 si morte
 */
static inline int est_vivante(int i, int j, grille g){return g.cellules[i][j] >= 1;}

/**
 * @brief recopie gs dans gd (sans allocation)
 * 
 * @param gs grille source
 * @param gd grille destination (copie)
 */
void copie_grille (grille gs, grille gd);


/**
 * @brief tester la vacuité d'une grille
 * 
 * @param g grille 
 * @return int 1 si vide sinon 0
 */
int grille_vide(grille *g);

/**
 * @brief tester l'égalité entre deux grilles
 * 
 * @param g grille source
 * @param t grille cible
 * @return int 1 si égales 0 sinon
 */
int grilles_egal(grille *g, grille *t);
#endif
