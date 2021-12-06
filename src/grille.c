/**
 * @file grille.c
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief initialisation et copie des grilles
 * @version 0.11
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "grille.h"
/**
 * @brief initialisation des cellules sur une grille avec un fichier
 * 
 * @param filename fichier utilisé
 * @param g grille ciblée d'initiliasation de cellules
 */
void init_grille_from_file (char* filename, grille* g){
	FILE* pfile = NULL;
	pfile = fopen(filename, "r");
	assert (pfile != NULL);
	
	int i,j,n,l,c,vivantes = 0;
	//int non_viables = n+1;
	
	fscanf(pfile, "%d", &l);
	fscanf(pfile, "%d", &c);
	
	alloue_grille(l,c,g);
	
	fscanf(pfile, "%d", &vivantes);
	for (n=0; n< vivantes; ++n){
		fscanf(pfile, "%d", &i);
		fscanf(pfile, "%d", &j);
		set_vivante(i,j,*g);
	}
	int non_viables;
	fscanf(pfile, "%d", &non_viables);
	for (n=0; n < non_viables; ++n){
		fscanf(pfile, "%d", &i);
		fscanf(pfile, "%d", &j);
		set_non_viable(i,j,*g);
	}
	
	fclose (pfile);
	return;
}

/**
 * @brief fonction copie d'une grille vers une autre
 * 
 * @param gs grille source 
 * @param gd grille destination (copie)
 */
void copie_grille (grille gs, grille gd){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
	return;	
}

/**
 * @brief fonction allouer espace mémoire à une grille g
 * 
 * @param l nombre de lignes du tableau
 * @param c nombr de collones du tableau
 * @param g pointeur vers le tableau
 */
void alloue_grille (int l, int c, grille* g)
{  
    g->nbl=l;
    g->nbc=c;
    g->cellules=(int**) malloc(sizeof(int**)*l);
    assert(g->cellules);//debug
	for (int i=0; i<l; i++)
    {
        g->cellules[i]=(int*) malloc(sizeof(int*)*c);
        for (int j=0; j<c; j++)
        {   
            g->cellules[i][j]=0;
        }
		assert(g->cellules[i]); //debug
    }
	
    return;
}	

/**
 * @brief liberer la memoire allouée à une grille g
 * 
 * @param g pointeur de type 'grille' vers la grille 
 */
void libere_grille (grille* g)
{
    for(int i=g->nbl-1;i>=0;i--)
    {
		free(g->cellules[i]);
        g->cellules[i]=NULL;
    }
    free(g->cellules);
    return;
}
/**
 * @brief tester la vacuité d'une grille
 * 
 * @param g grille 
 * @return int 1 si vide sinon 0
 */
int grille_vide(grille *g)
{	for (int i = 0; i < g->nbl; i++) for (int j = 0; j < g->nbc; j++)
			if (g->cellules[i][j] > 0)
				return 0;
	return 1;
}	

/**
 * @brief tester l'égalité entre deux grilles
 * 
 * @param g grille source
 * @param t grille cible
 * @return int 1 si égales 0 sinon
 */
int grilles_egal(grille *g, grille *t)
{
	for (int i = 0; i < g->nbl; i++) for (int j = 0; j < g->nbc; j++) 
	if (g->cellules[i][j] != t->cellules[i][j])
				return 0;
	return 1;
}	