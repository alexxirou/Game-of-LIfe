/**
 * @file main.c
 *
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief programme pour l'automate du jeu de la vie.
 * @version 0.51 (beta)
 * @date 2021-10-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/*! \mainpage 
 * \section  Projet
 * \subsection Info
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief Programme pour l'automate du "jeu de la vie"
 * @copyright Copyright (c) 2021\n
 * Jeu de la vie is free software: you can redistribute it and/or modify\n
 * it under the terms of the GNU General Public License as published by\n
 * the Free Software Foundation, either version 3 of the License, or\n
 * (at your option) any later version.\n
 * <br>
 * The project is distributed in the hope that it will be useful,\n 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 * GNU General Public License for more details.\n
 * 
 * \subsection Version v0.51 -- TP5 (dernière mise à jour):
 * 				-# Correction de bugs 
 * 				-# Corrige d'affichage menu en mode texte
 * 				-# Test oscillation 
 * 				-# Test vacuité et égalité
 * 				-# Création librarie libjeu
 * 				-# Corrige de bugs test_oscillation 
 * 				-# Test sur le serveur CI via shell script 
 */

#include <stdio.h>
#include <string.h>
#include "io.h"
#if defined (__cplusplus)
extern "C" {
	#include "jeu.h"
	#include "grille.h"
	}
#endif


/**
 * @brief fonction main du programme
 * 
 * @param argc nombre de fichiers à utiliser
 * @param argv nom fichier
 * @return int 0 pour execution réussite
 *
*/
int main (int argc, char ** argv) {
	
	
	
	if (argc <2 || argc >3 )
	{
		printf("usage : main <dossier grilles>/<fichier grille>\n");
		return 1;
	}
	
	//if (strstr(argv[1], "/")==NULL) verifier_chemin(); //buggée
    int pas = 0;
	grille g, gc;
	init_grille_from_file(argv[1],&g);
	alloue_grille (g.nbl, g.nbc, &gc);
	verifier_chemin();
	#if GRAPHIQUE
	if (argc==3 && !strcmp(argv[2], "-T"))
	{   
		debut_jeu(&g, &gc, pas);
	}
	else 
	{	
		debut_jeu_g(&g, &gc, pas);
		cairo_close_x11_surface(sfc);
	}

	#elif !GRAPHIQUE
	efface_grille();
	debut_jeu(&g, &gc, pas);
	#endif
	libere_grille(&g);
	libere_grille(&gc);
	return 0;
}