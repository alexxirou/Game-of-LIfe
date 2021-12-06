/**
 * @file jeu.c
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief fonctions évolution des grilles
 * @version 0.11
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "jeu.h"
#include "io.h"
/**
 * @brief comparaison des cellules voisins pou décider si une cellule est vivante
 * 
 * @param i position horizontale
 * @param j position verticale
 * @param g grille
 * @return nombre de cellules voisins vivants (calcul cyclique)
 */
int compte_voisins_vivants (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);
	return v; 
}
/**
 * @brief pointeur utiliser pour charger la bonne fonction de calcul voisinange
 * 
 */

int (*ptr_viv)(int ,  int , grille)=compte_voisins_vivants;

/**
 * 
 * @brief fonction evolution d'une grille chargée
 * 
 * @param g grille utilisée
 * @param gc copie de grille utilisée pour sauvergarder le status
 * @param age activer ou pas la vieillissement
 */
void evolue (grille *g, grille *gc, int age){
	copie_grille (*g,*gc); // copie temporaire de la grille
	int i,j,l=g->nbl, c = g->nbc,v;
	for (i=0; i<l; i++)
	{
		for (j=0; j<c; ++j)
		{
			v = ptr_viv (i, j, *gc);
			if (est_vivante(i,j,*g)) 
			{ // evolution d'une cellule vivante					
					// evolution d'une cellule vivante
					if ((v!=2 && v!= 3 ) || (age>0 && g->cellules[i][j]>8)) set_morte(i,j,*g); //regles évolution plus vieilissement
					else if (age>0 && g->cellules[i][j]<=8 && g->cellules[i][j] >0 ) g->cellules[i][j]++;  //incrementention quand l'option age est activée
			}
			else 
			{ // evolution d'une cellule morte
				if ( v==3 && g->cellules[i][j] >=0 ) set_vivante(i,j,*g); 							
			}
		}
	}
	return;
}
/**
 * @brief calcul de totale de voisins vivantes sans prennant en compte les voisins existants réels (pour le cellules des bords)
 * 
 * @param i pos horizontale
 * @param j pos verticale
 * @param g grille utilisée
 * @return int total de voisins vivants (version calcul non cyclique) 
 */
int compte_voisins_vivants_non_cyc(int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	v+= (i-1 >= 0 && j-1 >= 0) ? est_vivante(i-1, j-1, g) : 0;
	v+= (i-1 >= 0) ? est_vivante(i-1, j,g) : 0;
	v+= (i-1 >= 0 && j+1 < c) ? est_vivante(i-1, j+1, g) : 0;
	v+= (j-1 >= 0) ? est_vivante(i, j-1, g) : 0;
	v+= (j+1 < c) ? est_vivante(i, j+1, g) : 0;
	v+= (i+1 < l && j-1 >= 0) ? est_vivante(i+1, j-1, g) : 0;
	v+= (i+1 < l) ? est_vivante(i+1, j ,g) : 0;
	v+= (i+1 < l && j+1 < c) ? est_vivante(i+1, j+1, g) : 0;
	return v;
}

/**
 * @brief tester si une grille oscille
 * 
 * @param g grille à tester
 * @param gc grille copie
 * @param age mode viellissement
 * @param pas_vers_oscillation liste [2] avec delai et pas oscillation initialisé à (0,0)
 * @return  int[2]   int[1] = pas. -1 si pas d'oscillation ou >0 si oscillante. defaut 0 (non testé), 
 * int[0]=délai max 500 pas 
 */


int* test_oscillation(grille *g, grille *gc, int age, int pas_vers_oscillation[])
{	
	pas_vers_oscillation[0]=0;
	grille grille_test;
	grille grille_test2;
	grille grille_debut;
	alloue_grille(g->nbl, g->nbc, &grille_test); //grille tst oscillation
	alloue_grille(g->nbl, g->nbc, &grille_test2); //verification de l'oscillation trouvé
	alloue_grille(g->nbl, g->nbc, &grille_debut);
	assert(grille_test.cellules); 
	assert(grille_test2.cellules);//debug
	assert(grille_debut.cellules); //debug
	copie_grille(*g,grille_debut); //copie d'état initiale du test
	copie_grille(grille_debut,grille_test2);
	while (1){ 	//augmenter le metric delai 
		copie_grille(grille_debut,grille_test);
		for(int i=1; i<100; i++){ //calculer le pas d'pscillation
			evolue(&grille_test,gc,age);
			if (grille_vide(&grille_test)){ //sortir du boucle si vide
				break;
			}
			else if(grilles_egal(&grille_test,&grille_debut)){	
				pas_vers_oscillation[1]=i; //mise à jour le pas et tester évolution du grille de ce point ci
				copie_grille(grille_test,grille_test2);
				for(int j=0; j<50; j++){
					evolue(&grille_test2,gc,age);
					if (grille_vide(&grille_test2)){ //si évolution= vide alors oscillation finie donc faux.
						break;
					}
				}
				break;
			} 
		}	
		if (grille_vide(&grille_test2)||grille_vide(&grille_test)||pas_vers_oscillation[0]>500){ //si évolution = vide faux.
			pas_vers_oscillation[1]=-1;
			break;
		}
		else if(grilles_egal(&grille_test,&grille_debut)) break; //ocillation trouvé donc sortie 
		evolue(&grille_debut,&grille_test,age); //avancer l'état initile pour trouver oscillation avec delai
		pas_vers_oscillation[0]++;
	}
	// liberer les rilles

	libere_grille(&grille_test);
	libere_grille(&grille_test2);
	libere_grille(&grille_debut);
	return pas_vers_oscillation;
}
