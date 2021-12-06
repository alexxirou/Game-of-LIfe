/**
 * @file io.h
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief fonctions affichage grille, effacement grille, debut jeu
 * @version 0.1
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __IO_H
#define __IO_H

#if GRAPHIQUE
#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#endif

#include <stdio.h>
#include "grille.h"
#include "jeu.h"



#if GRAPHIQUE
#define MARGEG 30
#define MARGEH 250
#define MARGED 500
#define SIZEX 1000
#define SIZEY 750
#define FONT "Arial"



cairo_surface_t* sfc;


/**
 * @brief création fenêtre graphique
 * 
 * @param x longitude
 * @param y latitude
 * @return cairo_surface_t* pointeur surface 
 */
cairo_surface_t *cairo_create_x11_surface0(int x, int y);

/**
 * @brief detruire struct utilisée ou la fenêtre
 * 
 * @param sfc pointeur vers le structure crée avec cairo_create_x11surface0
 */
void cairo_close_x11_surface(cairo_surface_t *sfc);

/**
 * @brief fonction qui crée les élements statiques 
 * 
 */
void draw_menu();

/**
 * @brief fonctionne qui controle les modes
 * 
 * @param pas temps évolution
 * @param age mode vieillissement
 */

void options_menu(int pas, int age, int delai[]);

/**
 * @brief création des grilles et cellules
 * 
 * @param g grille utilisée
 * @param pas temps évolution
 * @param age mode vieillissement
 * @param delai[] mode oscillation
 */
void affiche_grille_g(grille g, int pas, int age, int delai[]);

/**
 * @brief fonction changer grilles
 * 
 * @param age Mode viellissement
 * @param input String entrée de l'utilisateur
 * @param g grille utilisée
 * @param gc copie grille
 */
void changer_grille_g(char* input, grille* g, grille* gc);

/**
 * @brief fonction évolution du jeu mode graphique
 * 
 * @param g grille utilisée
 * @param gc copie grille
 * @param pas temps dévoluton
 */
void debut_jeu_g(grille *g, grille *gc, int pas);
#endif

/**
 * @brief fonction verifier et changer le chemin vers le dossier grilles
 * 
 */

void verifier_chemin();

/**
 * @brief affichage d'un trait horizontal
 * 
 * @param c celulle
 */
void affiche_trait (int c);

/**
 * @brief affichage d'une ligne de la grille
 * 
 * @param c cellule
 * @param ligne ligne
 */
void affiche_ligne (int c, int* ligne, int age);

/**
 * @brief affichage d'une grille
 * 
 * @param g grille utilisée
 * @param pas temps d'évolution
 * @param age mode vieillissement
 * @param delai[] mode oscillation
 */
void affiche_grille (grille g,int pas, int age, int delai[]);

/**
 * @brief effacement d'une grille
 * 
 * @param g grille 
 */
void efface_grille();

/**
 * @brief fonction debut du jeu d'évolution
 * 
 * @param g grille initiale
 * @param gc grille copie
 */
void debut_jeu(grille *g, grille *gc, int pas);


/**
 * @brief mode viellissement
 * 
 */
extern int age;


#endif
