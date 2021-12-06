/**
 * @file io.c
 * @author Xirouchakis Alexandros (alexandros.xirouchakis2@etu.unistra.fr)
 * @brief fonctions entrée et sortie de données   
 * @version 0.11
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "io.h"
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

/**
 * @brief fonction qui verifie le chemin vers l'executable et change le repertoire current au "Grilles/"
 * 
 */
void verifier_chemin()
{
	
	char buf[1024]="";
	char cible[1024]="";
	int long len = readlink("/proc/self/exe", buf, 1023);
 
	if (len != -1) 
	{
  		buf[len] = '\0';

		memcpy(cible,&buf[0],len-9);
		strcat(cible,"/grilles");
		chdir(cible);
	}
	
	else printf("Impossible de recuperer le chemin vers l'éxecutable. Chemin trop longue ou problem de permissions.");

	return;
}
	

void affiche_trait (int c){
	int i;
	for (i=0; i<c; ++i) printf ("|---");
	printf("|\n");
	return;
}

/**
 * @brief fonction affichage d'une ligne de la grille 
 * 
 * @param c colonnes
 * @param ligne lignes
 * @param age mode viellissement
*/
void affiche_ligne (int c, int* ligne, int age){
	int i;
	if (age>=1)
	{
		for (i=0; i<c; ++i) 
			if (ligne[i] == 0 ) printf ("|   "); else if(ligne[i]==-1) printf("[ X ");   else printf ("| %d ", ligne[i]);	
	}	
	else
	{
		for (i=0; i<c; ++i) 
			if (ligne[i] == 0 ) printf ("|   "); else if(ligne[i]==-1) printf("[ X ");   else printf ("| 0 ");
			
	}
	printf("|\n");
	return;
}

/**
 * @brief fonction affichage d'une grille sur terminal
 * 
 * @param g grille utilisé
 * @param pas temps d'évolution
 * @param age mode vieillissement
 * @param delai mode oscillation
 */
void affiche_grille(grille g, int pas, int age, int* delai){
	int i, l=g.nbl, c=g.nbc;
	printf("\n");
	if (ptr_viv == compte_voisins_vivants_non_cyc) printf("Comptage : non cyclique          ");
	else printf("Comptage : cyclique      ");
	if (age>0) printf("Vieillissement: Activée      ");
	else printf ("Viellissement: Desactivée       " );
	if (delai[1]==0) printf("Oscillation: non testé \n\n");
	else if (delai[1]>0) printf("%d Delai: %d\n\n",delai[1], delai[0]);
	else printf("Non oscillante\n\n");
	affiche_trait(c);
	for (i=0; i<l; ++i) {
		affiche_ligne(c, g.cellules[i], age);
		affiche_trait(c);
	}	
	printf("\n"); 
	
	printf("Temps d'évolution: %d\n", pas);
	return;
}

/**
 * @brief fonction effacement de la grille
 * 
 * @param g grille utilisé
 */

void efface_grille (){
	printf("\e[1;1H\e[2J");
}

/**
 * @brief fonction pour activer ou désactiver le vieillissement
 * 
 * @param age parametre qui controle le vieillissement
 * @param g grille actuelle
 * */

void active_vieil(int* age, grille *g){	
	if(*age>0) 
	{
		*age = 0;
		for (int i=0; i<g->nbl; i++)
		{
			for (int j=0; j<g->nbc; ++j)
			{
				if (g->cellules[i][j] >1) g->cellules[i][j]=1;
			}
		}	
	}		
	else
	{
		*age = 1;
		
	}
	return;
}

/**
 * @brief fonction qui change le mode de calcul de voisinage via reattribution du pointeur de la fonction
 * @param delai mode oscillation 
 */
void change_le_calcul(){	
	if(ptr_viv == compte_voisins_vivants)
	{
		ptr_viv=compte_voisins_vivants_non_cyc;	
	}
	else if (ptr_viv == compte_voisins_vivants_non_cyc)
	{
		ptr_viv=compte_voisins_vivants;	
	}
	else
	{
		printf("Erreur mode inconnu.");
	}
	return;
		
}
/**
 * @brief fonction qui charge une nouvelle grille utilisant un fichier donnée par l'utilisateur
 * 
 * @param pas pointeur vers la variable temps d'évolution
 * @param g grille à utiliser
 * @param gc copie de la grille utilisée
 */

void changer_grille(int *pas, grille* g, grille* gc)
{
	char path[1024];
    getcwd(path, 1024);
    printf("Vous êtes maintenant dans: %s\n", path);
	*pas = 0;
	libere_grille(g);
	libere_grille(gc);
	char filename[50];
	printf("Entrez le nom d'un fichier à charger: ");
	scanf("%50s",filename);
	int l = getchar();
	if ( l=='\n')
		{	
			init_grille_from_file (filename, g);
			alloue_grille (g->nbl, g->nbc, gc);
			efface_grille();
		}
	return;	
}



/**
 * @brief fonction de mutation de grille selon le touche (évolution ou nouvelle grille)
 * 
 * @param g grille générée
 * @param gc copie temp de la grille
 * @param pas temps évolution
 */
void debut_jeu(grille *g, grille *gc, int pas)
{	
	
	int delai[2];
	delai[1]=0; //valeur  compteur du test d'oscillation
	int age=0;
	efface_grille();
	affiche_grille(*g,pas,age,delai);	
	int c = getchar();
	while (c != 'q') // touche 'q' pour quitter
	{   
		switch (c) {
			case '\n' : 
			{ // touche "entree" pour évoluer
				evolue(g,gc,age);
				pas++;
				efface_grille();
				affiche_grille(*g,pas,age,delai);				
				break;
			}
			case 'c' :
			{	
				delai[1]=0;
				change_le_calcul();
				efface_grille();
				affiche_grille(*g,pas, age, delai);	
				while((getchar())!='\n'); //Flush input buffer jusqu'a EOF
				break;
				
			}
			case 'v' :
			{
				// activation / désactivation du vieillissement
				
				while((getchar())!='\n'); //Flush input buffer jusqu'a EOF
				active_vieil(&age, g);
				delai[1]=0;
				efface_grille();
				affiche_grille(*g,pas,age,delai);	
				break;
			}
			case 'n' :
			{// touche "n" pour charger une nouvelle grille
				efface_grille();
				delai[1]=0;
				changer_grille(&pas,g,gc);
				affiche_grille(*g, pas, age, delai);
				break;
			}
			case 'o' :
			{
				while((getchar())!='\n'); //Flush input buffer jusqu'a EOF
				test_oscillation(g, gc, age, delai);
				efface_grille();
				affiche_grille(*g,pas,age, delai);
	
				break;
			}	
			default : 
			{ // touche non traitée
				printf("\n\e[1A");
				break;
			}
		}	
		
		c = getchar();
	}	 
	return;	
}
#if GRAPHIQUE

/**
 * @brief création fenêtre graphique
 * 
 * @param x longitude
 * @param y latitude
 * @return cairo_surface_t* pointeur surface 
 */
cairo_surface_t *cairo_create_x11_surface0(int x, int y)
{
    Display *dsp;
    Window win;
    Window rootwin;
	int screen;
    cairo_surface_t *sfc;
	

    if ((dsp = XOpenDisplay(NULL)) == NULL) {
		printf("Impossible d'ouvir l'interface. \n");  
		exit(1);
	}
    screen = DefaultScreen(dsp);
	rootwin=RootWindow(dsp, screen);

	win=XCreateSimpleWindow(dsp, rootwin, 1, 1, SIZEX, SIZEY, 0, 
			BlackPixel(dsp, screen), BlackPixel(dsp, screen));

	XStoreName(dsp, win, "jeu de la vie");
	XSelectInput(dsp, win,  ExposureMask | ButtonPressMask | KeyPressMask);
	XMapWindow(dsp, win);
	
    sfc = cairo_xlib_surface_create(dsp, win, DefaultVisual(dsp, 0), x, y);
    cairo_xlib_surface_set_size(sfc, x, y);
    
	return sfc;
}

/**
 * @brief detruire struct utilisée ou la fenêtre
 * 
 * @param sfc pointeur vers le structure crée avec cairo_create_x11surface0
 */
void cairo_close_x11_surface(cairo_surface_t *sfc)
{
   	Display *dsp = cairo_xlib_surface_get_display(sfc);
	cairo_surface_destroy(sfc);
   	XCloseDisplay(dsp);
}

/**
 * @brief fonction qui crée les élements statiques 
 * 
 */
void draw_menu()
{	
	cairo_t* surface =cairo_create(sfc);
	cairo_set_source_rgb(surface, 0.5, 0.5, 0.5);
	cairo_paint(surface);
	cairo_set_source_rgb(surface, 1, 1, 1);
	  cairo_select_font_face(surface, FONT,
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(surface,20);
	cairo_move_to(surface, 10,MARGEG+10);
	cairo_show_text(surface, "• touche Entrée: Faire évoluer les cellules ");
	cairo_move_to(surface, 10,MARGEG*2+10);
	cairo_show_text(surface, "• touche c: Comptage cyclique");
	cairo_move_to(surface, 10,MARGEG*3 +10);
	cairo_show_text(surface, "• touche v: Activer / désactiver le vieillissement");
	cairo_move_to(surface, 10,MARGEG*4+10);
	cairo_show_text(surface, "• touche n / clique gauche : Changer de grille");
	cairo_move_to(surface, 10,MARGEG*5+10);
	cairo_show_text(surface, "• touche q / clique droite : Quitter");
	cairo_move_to(surface, MARGED+20, MARGEH+MARGEG);
	cairo_show_text(surface, "Temps de'évolution: ");
	cairo_move_to(surface, MARGED+20, MARGEH+2*MARGEG+10);
	cairo_show_text(surface, "Comptage cyclique: ");
	cairo_move_to(surface, MARGED+20, MARGEH+3*MARGEG+20);
	cairo_show_text(surface, "Viellissement: ");
	cairo_move_to(surface, MARGED+20, MARGEH+4*MARGEG+30);
	cairo_show_text(surface, "Test Oscillation: ");
	cairo_destroy(surface);
}

/**
 * @brief création des grilles et cellules
 * 
 * @param g grille utilisée
 * @param pas temps évolution
 * @param age mode vieillissement
 */
void affiche_grille_g(grille g, int pas, int age, int* delai)
{	
	draw_menu();
	cairo_t* grilles;
	cairo_t* res;
	grilles=cairo_create(sfc);
	res=cairo_create(sfc);
	double cell = (double) (SIZEX/2) / g.nbl;
	double celh = (double) (SIZEY - MARGEH)/g.nbc;
	int size =(int)((cell+celh)/4);
	char str[32];
	cairo_set_source_rgb(grilles, 0, 0, 0);
	cairo_set_font_size(res, size);
	double difh = (celh*0.3);
	double difl = (cell*0.65);
	for (int i = 0; i<g.nbl; i++)
	{
		for (int j = 0; j<g.nbc; j++)
		{
			cairo_rectangle(grilles, (j*celh) ,  MARGEH+(i*cell), celh-1, cell-1 );
			cairo_move_to(res,(j*celh)+difh,MARGEH+(i*cell)+difl);
			cairo_set_source_rgb(res, 0, 1, 0);
			if(age<=0){ 
				
				if (g.cellules[i][j]>0) cairo_show_text(res,"O");
				
				else if (g.cellules[i][j]<0) 
			    {
					cairo_set_source_rgb(res, 1, 0, 0);
					cairo_show_text(res,"X");
				}	
			}
			else if(age>0){

				if (g.cellules[i][j]>0) {

					sprintf(str, "%d", g.cellules[i][j]);
					cairo_show_text(res,str);
				}
				else if (g.cellules[i][j]<0)
				{ 
					cairo_set_source_rgb(res, 1, 0, 0);
					cairo_show_text(res,"X");
				}	
			}	
			
		}	
	}	
	options_menu(pas, age, delai);
	cairo_stroke(grilles);
	cairo_destroy(grilles);
	cairo_destroy(res);
	return;
}

/**
 * @brief fonction qui crée les élements statiques 
 * 
 */
void options_menu(int pas, int age, int* delai)
{
	cairo_t* txt;
	txt=cairo_create(sfc);
	cairo_set_font_size(txt, 20);
	cairo_set_source_rgb(txt, 0, 0, 1);
	char str[32];
	sprintf(str, "%d", pas);
	cairo_move_to(txt, MARGED+230, MARGEH+MARGEG);
	cairo_show_text(txt,str);
	cairo_move_to(txt, MARGED+230, MARGEH+2*MARGEG+10);
	if(ptr_viv == compte_voisins_vivants_non_cyc)
	{
		cairo_set_source_rgb(txt, 1, 0, 0);
		cairo_show_text(txt,"Desactivée");
	}
	else 
	{
		cairo_set_source_rgb(txt, 0, 1, 0);
		cairo_show_text(txt,"Activée");
	}
	cairo_move_to(txt, MARGED+170, MARGEH+(3*MARGEG)+20);
	if (age>0) 
	{
		cairo_set_source_rgb(txt, 0, 1, 0);
		cairo_show_text(txt,"Activée");
	}
	else 
	{
		cairo_set_source_rgb(txt, 1, 0, 0);
		cairo_show_text(txt,"Desactivée");
	}
	cairo_move_to(txt, MARGED+190, MARGEH+4*MARGEG+30);
	if (delai[1]>0)
	{
		cairo_set_source_rgb(txt, 0, 1, 0);
		sprintf(str, "%d Delai: %d",delai[1],delai[0]);
		cairo_show_text(txt,str);
	}
	else if (delai[1]<0)
	{
		cairo_set_source_rgb(txt, 0, 0, 1);
		cairo_show_text(txt,"Pas d'oscillation");
	}
	else
	{
		cairo_set_source_rgb(txt, 1, 0, 0);
		cairo_show_text(txt,"Non testée");
	}
	cairo_destroy(txt);
	return;
}

/**
 * @brief fonction affichage d'entrée utilisateur sur l'ecran pour changer le fichier grille
 * 
 * @param input String entrée de l'utilisateur
 */

void draw_input(char *input)
{
	cairo_t* cr;
	cr = cairo_create(sfc);
	cairo_set_source_rgb(cr, 1, 1, 1);
	char inputLabel[256];
	int x =MARGED+20;
	int y =MARGEH+(11*MARGEG);
	cairo_rectangle(cr, x-10 , y-20,  470,  50);
	sprintf(inputLabel, "Fichier grille à charger:");
	cairo_text_extents_t extendsInput;
	cairo_text_extents_t extendsInputLabel;
	
	cairo_fill(cr);
	cairo_select_font_face(cr, FONT,
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);

	cairo_set_font_size(cr, 18);
	cairo_set_source_rgb(cr, 0.301, 0.301, 0.301);
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, inputLabel);
	cairo_text_extents(cr, inputLabel, &extendsInputLabel);
	cairo_move_to(cr, x + extendsInputLabel.width + 2, y);
	cairo_show_text(cr, input);
	cairo_text_extents(cr, input, &extendsInput);
	int inputOffset = x + extendsInputLabel.width + 5 + extendsInput.width;
	cairo_move_to(cr, inputOffset, y - 15);
	cairo_set_line_width(cr, 1);
	cairo_line_to(cr, inputOffset, y + 10);
	cairo_stroke(cr);
	cairo_destroy(cr);
}


/**
 * @brief fonction évolution du jeu mode graphique
 * 
 * @param g grille utilisée
 * @param gc copie grille
 * @param pas temps dévoluton
 */
void debut_jeu_g(grille *g, grille *gc, int pas)
{	
	int delai[2]; //valeur  compteur du test d'oscillation
	delai[1]=0;
	int age=0;
	XEvent e;
	sfc=cairo_create_x11_surface0(SIZEX, SIZEY);
	
	while (1) {
		XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
		affiche_grille_g(*g, pas, age, delai);
		if (e.type==KeyPress && e.xkey.keycode == XKeysymToKeycode(cairo_xlib_surface_get_display(sfc), 0x0076)) //v
		{	
			delai[1]=0;
			active_vieil(&age,  g);
			affiche_grille_g(*g, pas, age, delai);
			
		}
		else if (e.type==KeyPress && e.xkey.keycode == XKeysymToKeycode(cairo_xlib_surface_get_display(sfc), 0x0063)) //c
		{
			delai[1]=0;
			change_le_calcul();
			affiche_grille_g(*g, pas, age, delai);
		}
		else if ((e.type==KeyPress && e.xkey.keycode == XKeysymToKeycode(cairo_xlib_surface_get_display(sfc),  0xff0d))||(e.type == ButtonPress && e.xbutton.button ==1)) //entrée
		{
			evolue(g,gc,age);
			pas++;
			affiche_grille_g(*g, pas, age, delai);
		}
		else if (e.type==KeyPress && e.xkey.keycode == XKeysymToKeycode(cairo_xlib_surface_get_display(sfc), 0x006e)) //n il faut entrer juste le nom fichier sans chemin
		{	
			draw_input(""); 
			KeySym key;
			char input[13]="";	
		    int exit = 0;
			char inputBuffer[100]="";
			
			while(exit==0)
			{
				
				XNextEvent(cairo_xlib_surface_get_display(sfc), &e);   //entrer pour passer le nom entrée
				if (e.type == KeyPress  &&XLookupString(&e.xkey,input,255,&key,0)==1) {
					if (e.xkey.keycode == 36){ 
						exit=1;	
						break;
					}
					else if (e.xkey.keycode ==  22 ){      //backspace
						inputBuffer[strlen(inputBuffer)-1] = '\0';
					}
					else if (e.xkey.keycode == 9 || strlen(inputBuffer)>20) {  // escape  pour quitter ou nom plus de 20 chars
						break;
					}
					else
					{
						strcat(inputBuffer, input); 	
					}
					draw_input(inputBuffer);
					

				}
			}
			if (exit==1){
				strcat(inputBuffer ,"\0");
				//printf("%ld", strlen(inputBuffer)); //debug
				//printf("%s", inputBuffer);		 //debug
				delai[1] = 0;
				libere_grille(g);
				libere_grille(gc);
				init_grille_from_file(inputBuffer, g);	
				alloue_grille (g->nbl, g->nbc, gc);
				
			}	
			pas=0;
			affiche_grille_g(*g, pas, age ,delai);
			
		}
		else if (e.type==KeyPress && e.xkey.keycode == XKeysymToKeycode(cairo_xlib_surface_get_display(sfc),  0x006f)) //test oscilation
		{
			test_oscillation(g, gc, age, delai);
			affiche_grille_g(*g, pas, age, delai);

		}
		else if ((e.type == ButtonPress && e.xbutton.button ==3 ) ||(e.type==KeyPress && e.xkey.keycode == XKeysymToKeycode(cairo_xlib_surface_get_display(sfc),  0x0071)) ) 
		{		
			return;  //q ou clicque droite pour quitter
		}
	}
}

#endif