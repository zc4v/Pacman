#include "./main.h"
#include <math.h>
#include <string.h>

int boucle_principale(Partie p)
{
	//################################ Annonce de toute les variables ou fonctions utilises
	
	// Déclaration des variables de type entier
	int i , j;												// Compteur générique
	int direction;										// Contient la valeur ASCII de la touche pressée
	int Fin_de_partie=0;							// Drapeau indiquant si la partie est active (=0) ou finie (=1)
	int Level=1;										// N° du niveau en cours
	int tour_sous_pacball=50;					// Contient le nombre de tour restant à Paman pour chasser les fantômes
	int tour=0;											// Nombre de tour depuis le début de la partie
	int pac_ball_manger=0;						// Drapreau indiquant si une pacman est en mode chasseur
	int score =0;										// Score du joueur dans la partie
	
	// Déclaration des fonctions retournant un entier - Détail de la fonction à lire dans sa déclaration
	int attendre_touche_duree()   ,Plateau_de_Jeu() , Fin_Plateau_de_jeu ,Fin_De_Tableau() ,Super_Pacball(); 	
	
	// Déclaration des variables de type Partie
	Partie save;										// Permet de sauvegarder l'état initial du plateau qui servira à réinitialiser entre deux niveaux
	
	// Déclaration des fonctions retournant le type Partie
	Partie Deplacement_Fantomes() ,deplacement_pacman() , Quelqu_un_Manger() , Sortie_Fantome() ;
	
	// Déclaration des variables de type Point contenant des coordonnées
	Point gameover;								// Position où sera affiché le message "Game OVER"
	
	/* on donne une valeur au variables qui serviront dans le programme pour afficher "you lose" ou "you win" */
	gameover.x=(p.C*30)/4;	
	gameover.y=(p.L*30)/2.5;
	
	// ################################ Début du traitement
	// Sauvegarde des valeurs initiales du plateau  (sauvegarde de la carte, de la partie et des coordonnées du point servant a afficher le score 
	char save_board[p.L][p.C] ;
	for(i=0	;i!=p.L;	i++)
        {
        for(j=0;j!=p.C;j++)
            save_board[i][j]=p.plateau[i][j];
        }
	save = p ;
	
	// lancement de la partie 
	
	while (Fin_de_partie==0)   													// Tant que la partie est active (=0) faire :    
	{
		direction= attendre_touche_duree(15); 								// la machine va donner la valeur ASCII de la touche appuyée
		
		p=deplacement_pacman(p,direction);   								// on lance le programme
				
		if (p.plateau[0][10] =='!')														// test servant à savoir si pacman a manger une ball
		{
			score +=15 ;  																// le score augmente si une balle a été mangee
		}
		
		if ( p.plateau[10][0]=='!' )													// 	test servant à savoir si pacman a manger une super pacball
		{
			tour_sous_pacball=Super_Pacball(tour_sous_pacball,1);	// Initialisation de la variable
		}
		else
		{			
			tour_sous_pacball=Super_Pacball(tour_sous_pacball,0);	// incrément de la variable
		}
		
		if (tour_sous_pacball<50)													// Test pour savoir su pacman est chasseur ou chassé
			pac_ball_manger=1;														// chasseur
		else 
			pac_ball_manger=0;														//chassé
		
		p=Quelqu_un_Manger(p,pac_ball_manger) ;    						// fonction qui regarde si quelqu'un a mangé quelque chose 
		
		if (p.plateau[0][11] =='!' )													// test si pacman a mangé un fantome alors on augmente le score de 40	
		{
			score += 40 ;
		}
		
		// les 3 prochains tests servent à regler la vitesse des fantomes dans la partie
		if (Level ==1 )																	// Niveau 1 les fantômes bougent 3 fois moins vite que pacman
			if (tour %3 == 0)		
			{
				p=Sortie_Fantome( p , tour) ; 										// fonction qui fait sortir les fantômes tous les 20 tours			
				p=Deplacement_Fantomes(p ,pac_ball_manger);  		// fonctions qui deplace les fantômes 
				p=Quelqu_un_Manger(p,pac_ball_manger) ; 				// fonction pour voir si un fantome a mangé pacman 
			}
			
		if (Level ==2 )
			if (tour %2 == 0) 															// Niveau 2 les fantômes bougent 2 fois moins vite que pacman
			{
				p=Sortie_Fantome( p , tour) ;
				p=Deplacement_Fantomes(p ,pac_ball_manger);
				p=Quelqu_un_Manger(p,pac_ball_manger) ;
			}
		
		if ( Level ==3 || Level ==4 ) 												// Niveau 3 et 4 les fantômes bougent aussi vite que pacman
		{
				p=Sortie_Fantome( p , tour) ;
				p=Deplacement_Fantomes(p ,pac_ball_manger);
				p=Quelqu_un_Manger(p,pac_ball_manger) ;
		}
	
		if (p.plateau[0][0] == '!')														// Si pacman est mangé alors la partie s'arrête
			Fin_de_partie=1 ;
		
		
		Fin_Plateau_de_jeu= Fin_De_Tableau(p); 							// Fontion qui verifie qu'il reste des balls à manger sur le plateau 
		
		
		printf("Score %d  \n", score);   											// Affiche le score du joueur dans le terminal (pas réussi à générer un texte contenant une chaîne de caractère et un entier)
		
		Plateau_de_Jeu(p,Level,pac_ball_manger); 							// fonction qui s'occupe de faire le dessin sur le tableau 
		
		if (Fin_Plateau_de_jeu==1)													// s'il ne reste plus de ball à manger alors on incrémente le niveau et on reinitialise la partie et l'affichage
		{
			Level++;
			if (Level==5)																	// le jeu n'a que 4 niveau donc si le joueur passe au niveau 5 alors pacman est vainqueur 
			{
				afficher_texte("You Win", 75, gameover, blanc);
				actualiser();
				return 0 ;
			}
			
			p=save;																		// on redonne la valeur initiale au plateau et à la partie
			for(i=0;i!=p.L;i++)
				{
				for(j=0;j!=p.C;j++)
					p.plateau[i][j]=save_board[i][j];
				}
		}
	
		attente(85) ;																		// on fait patienter le joueur pour permettre la jouabilité du jeu 
		tour += 1;																			// à la fin de la boucle on déclare le tour suivant 
	}
	afficher_texte("You Lose", 75, gameover, blanc); 					// on affiche " you lose "  si la partie prend fin (=1) sans que pacman est gagné
	actualiser(); 
	return 0;	
}



Partie deplacement_pacman(Partie p,int direction) 						// fonction qui verifie si pacman peut bouger puis le fais avancer si possible		
{
	if (direction==0)																		// si le joueur n'a appuye sur aucune touche alors le pacman va dans la direction precedente
	{
		if (p.plateau[25][0]=='h' )
			direction = SDLK_z ;
			
		if (p.plateau[25][0]=='g' )
			direction = SDLK_q ;
			
		if (p.plateau[25][0]=='b' )
			direction = SDLK_s ;
			
		if (p.plateau[25][0]=='d' )
			direction = SDLK_d ;
			
	}
	if (direction==SDLK_z  || direction==SDLK_UP) 		 				// correspond aux valeurs données par l'appuie sur "z"ou "Z" ou "haut"
	{
		if (p.plateau[p.pacman.l-1][p.pacman.c] =='.'  || p.plateau[p.pacman.l-1][p.pacman.c] ==' ' || p.plateau[p.pacman.l-1][p.pacman.c] =='B' ) 		//verifie que la case où l'on souhaite se rendre est bien une case praticable 
		{
			if (p.plateau[p.pacman.l-1][p.pacman.c]=='B')					// les prochains tests sont là pour indiquer si le joueur est passé sur une case spéciale B = Superpacball, . = ball
				{
				p.plateau[10][0]='!';													// On place un drapeau dans le tableau pour gérer l'action spéciale
				}
			else    {
				p.plateau[10][0]='*';													// Sinon on place un mur dans le tableau pour effacer
				} 
			if (p.plateau[p.pacman.l-1][p.pacman.c]=='.')
				{
				p.plateau[0][10] ='!';													// On place un drapeau dans le tableau pour gérer l'action spéciale
				}
			else 
				{
				p.plateau[0][10] ='*';													// Sinon on place un mur dans le tableau pour effacer
				}
			p.plateau[25][0]='h' ;  														// on place le mouvement precedent dans une case du tableau 
			p.plateau[p.pacman.l][p.pacman.c]=' ';
			p.pacman.l=p.pacman.l-1;    			       							//bouge pacman dans la direction voulu
		}
	}		
		
	if (direction==SDLK_q || direction==SDLK_LEFT) 		  			// correspond aux valeurs données par l'appuie sur "q" ou "Q"ou "gauche" (fonctionnement identique au tests et aux affectation précédents)
	{
		if (p.pacman.c == 0) 
			{
				p.plateau[p.pacman.l][p.pacman.c]=' ';
				p.pacman.c=p.C-1;
				
			}
		if (p.plateau[p.pacman.l][p.pacman.c-1] =='.'  || p.plateau[p.pacman.l][p.pacman.c-1] ==' '  || p.plateau[p.pacman.l][p.pacman.c-1] =='B' ) 		 //verifie que la case où l'on souhaite se rendre est bien une case praticable 
		{
			if (p.plateau[p.pacman.l][p.pacman.c-1]=='B')
				{
				p.plateau[10][0]='!';
				}
			else    {
				p.plateau[10][0]='*';
				} 
				
			if (p.plateau[p.pacman.l][p.pacman.c-1]=='.')
				{
				p.plateau[0][10] ='!';
				}
			else 
				{
				p.plateau[0][10] ='*';
				}
			p.plateau[25][0]='g' ;
			p.plateau[p.pacman.l][p.pacman.c]=' ';
			p.pacman.c=p.pacman.c-1; 											//bouge pacman dans la direction voulu

		}
		
	}
			
	if (direction==SDLK_s || direction==SDLK_DOWN)		 			// correspond aux valeurs données par l'appuie sur "s" ou "S"ou "bas" (fonctionnement identique au tests et aux affectation précédents)
	{
		if (p.plateau[p.pacman.l+1][p.pacman.c] =='.'  || p.plateau[p.pacman.l+1][p.pacman.c] ==' '  || p.plateau[p.pacman.l+1][p.pacman.c] =='B' )		 //verifie que la case où l'on souhaite se rendre est bien une case praticable 
		{
			if ((p.plateau[p.pacman.l+1][p.pacman.c]=='B'))
				{
				p.plateau[10][0]='!';
				}
			else {
				p.plateau[10][0]='*';
				} 
				
			if (p.plateau[p.pacman.l+1][p.pacman.c]=='.')
				{
				p.plateau[0][10] ='!';
				}
			else 
				{
				p.plateau[0][10] ='*';
				}
			p.plateau[25][0]='b' ;
			p.plateau[p.pacman.l][p.pacman.c]=' ';
			p.pacman.l=p.pacman.l+1; 											//bouge pacman dans la direction voulu
		}				
	}
			
	if (direction==SDLK_d || direction==SDLK_RIGHT)		  			// correspond aux valeurs données par l'appuie sur "d" ou "D" ou "droite"  (fonctionnement identique au tests et aux affectation précédents)
	{
		if (p.pacman.c==(p.C-1)) 
			{
				p.plateau[p.pacman.l][p.pacman.c]=' ';
				p.pacman.c=0;
			}
		if (p.plateau[p.pacman.l][p.pacman.c+1] =='.'  || p.plateau[p.pacman.l][p.pacman.c+1] ==' ' || p.plateau[p.pacman.l][p.pacman.c+1] =='B' ) 	 //verifie que la case où l'on souhaite se rendre est bien une case praticable 
		{
			if ((p.plateau[p.pacman.l][p.pacman.c+1]=='B'))
				{
				p.plateau[10][0]='!';
				}
			else    {
				p.plateau[10][0]='*';
				} 
				
			if (p.plateau[p.pacman.l][p.pacman.c+1]=='.')
				{
				p.plateau[0][10] ='!';
				}
			else 
				{
				p.plateau[0][10] ='*';
				}
				
			p.plateau[25][0]='d' ;
			p.plateau[p.pacman.l][p.pacman.c]=' ';
			p.pacman.c=p.pacman.c+1; 											//bouge pacman dans la direction voulu
		
		}
		
		
	}
	
	p.plateau[p.pacman.l][p.pacman.c]='P'; 									// on place le P dans le tableau au nouvelle emplacement de pacman 
	
	return p ;
}

int Plateau_de_Jeu(Partie p,int Level,int Super_Pacball)				//fonction qui dessine le plateau 
{
	// on donne les coordones des fantomes sur le tableau et de la où va on va écrire le niveau
	
	// Définition des variables de type Point permettant de stocker les positions nécessaires aux formes à afficher
	Point Creation_Plateau, cercle , Coo_Pinky,Coo_Blue,Coo_Red, Coo_Green , niveau;
	Coo_Pinky.x=  p.fantomes[0].c*30+15;
	Coo_Pinky.y=  p.fantomes[0].l*30+15;

	Coo_Blue.x=  p.fantomes[1].c*30+15;
	Coo_Blue.y=  p.fantomes[1].l*30+15;

	Coo_Green.x=  p.fantomes[2].c*30+15;
	Coo_Green.y=  p.fantomes[2].l*30+15;

	Coo_Red.x=  p.fantomes[3].c*30+15;
	Coo_Red.y=  p.fantomes[3].l*30+15;

	niveau.x=25;
	niveau.y=335;

	Creation_Plateau.x=0;
	Creation_Plateau.y=0;
	dessiner_rectangle(Creation_Plateau,p.C*30,p.L*30, noir);

	for (Creation_Plateau.x=0;Creation_Plateau.x<(p.C*30);Creation_Plateau.x=Creation_Plateau.x+30)			// on dessine le les case du tableau une par une
		{
		cercle.x=Creation_Plateau.x+15 ;
		for (Creation_Plateau.y=0;Creation_Plateau.y<(p.L*30);Creation_Plateau.y=Creation_Plateau.y+30)
			 {
			 cercle.y=Creation_Plateau.y+15 ;
			 
			 
			 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='P')
				dessiner_disque(cercle, 13, jaune);
			 
			 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='.')
				dessiner_disque(cercle, 4, orange );
				
			 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='B')
				dessiner_disque(cercle, 11, blanc);
			
			
			 if (Level ==1)																		// selon le niveau , la couleur des murs changes chaque couleur est gérée dans les tests ci-dessous
			 {
				 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='M')
					dessiner_rectangle(Creation_Plateau,30, 2, bleu);
				 
				 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='*' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='!' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='h' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='g' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='b' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='d' )
					dessiner_rectangle(Creation_Plateau,30, 30, bleu);
				
				afficher_texte("Niveau 1", 25, niveau, orange);
			 }
			 
			  if (Level ==2)
			 {
				 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='M')
					dessiner_rectangle(Creation_Plateau,30, 2, violet);
				 
				 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='*' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='!' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='h' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='g' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='b' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='d' )
					dessiner_rectangle(Creation_Plateau,30, 30, violet);
				
				afficher_texte("Niveau 2", 25, niveau, jaune);
			 }
			 
			 if (Level ==3)
			 {
				 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='M')
					dessiner_rectangle(Creation_Plateau,30, 2, rouge);
				 
				 if (p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='*' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='!' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='h' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='g' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='b' || p.plateau[(Creation_Plateau.y/30)][(Creation_Plateau.x/30)]=='d' )
					dessiner_rectangle(Creation_Plateau,30, 30, rouge);
				
				afficher_texte("Niveau 3", 25, niveau, vert);
			 }
			 if (Level ==4)
				afficher_texte("Niveau 4", 25, niveau, blanc);
			 }
		}
		
		if (Super_Pacball==0)																	// dessine les fantomes dans leur couleur habituel ou en bleu si il y a une Superpacball active
		{
			dessiner_disque(Coo_Pinky, 12, pink);
			dessiner_disque(Coo_Green, 12, green);
			dessiner_disque(Coo_Blue, 12, blue);
			dessiner_disque(Coo_Red, 12, red);
		}	
		else 
		{
			dessiner_disque(Coo_Pinky, 12, aliceblue);
			dessiner_disque(Coo_Green, 12, aliceblue);
			dessiner_disque(Coo_Blue, 12, aliceblue);
			dessiner_disque(Coo_Red, 12, aliceblue);
		}
		actualiser() ;
		return 0 ;
	}		




int Fin_De_Tableau(Partie p)																	// fonction qui verifie si la partie est terminée
{
int i ,j, Nb_Pacball=0;

for(i=0;i!=p.L;i++)																					// on calcule case par case le nombre de ball ('.') 
        {
			for(j=0;j!=p.C;j++)
			{
				if  (	p.plateau[i][j] 	==   '.'   ) 
				{
					Nb_Pacball++ ;
				}
			}
        }

	if  (Nb_Pacball==0)																			//	S'il n'en reste plus de alors on change de niveau
	{
		return 1 ;
	}
	return 0 ;																							// S'il en reste alors le niveau continu
	
}

int Super_Pacball(int tour_sous_pacball, int pac_ball_manger )					// Fonction qui verifie si pacman a mangé une pac ball
{
	if (pac_ball_manger==1)																		// Si le pacman vient de manger une superpacball alors on reinitialise la valeur tour_sous_pacball
	{
		tour_sous_pacball=0;
	}
	tour_sous_pacball=tour_sous_pacball+1;												// Dans tout les cas on incremente tour_sous_pacball de un a chaque tour
	
	return tour_sous_pacball;																	// Renvoi le nombre de tour sous pacball

}

Partie Deplacement_Fantomes(Partie p ,int Super_Pacball)						// Fonction qui s'occupe des mouvement des fantomes 
{
	int i ,Haut,Gauche,Bas,Droite;
	for ( i=0 ; i < NBFANTOMES ; i++)
		{
			
																											//Verification du fait que le fantomes puisse bouger vers le " Haut " puis calcule de la distance avec pacman
		
			if ((p.plateau[p.fantomes[i].l -1][p.fantomes[i].c] =='.' || p.plateau[p.fantomes[i].l -1][p.fantomes[i].c] ==' ' || p.plateau[p.fantomes[i].l -1][p.fantomes[i].c] =='B' || p.plateau[p.fantomes[i].l -1][p.fantomes[i].c] =='P'  || p.plateau[p.fantomes[i].l -1][p.fantomes[i].c] =='F' ) && p.plateau[26][i] != 'b')  //on verifie que le fantomes puisse aller dans la direction puis on verifie qu'il ne va pas faire le mouvement inverse du tour précédent 
				{
				Haut=	pow ( p.pacman.l - p.fantomes[i].l  + 1 , 2 ) + pow ( p.pacman.c - p.fantomes[i].c , 2 );    // calcule de la distance avec pacman a vol d'oiseau 
				}
																											// s'il on ne peut pas aller dans la direction alors la distance est infinie ou nulle selon que le fantôme chasse ou fuit pacman
			else 
				{
				if (Super_Pacball==0)
					Haut=999999;
				else 
					Haut=0;
				}
			
			
																											// Verification du fait que le fantomes puisse bouger vers la gauche puis calcule de la distance avec pacman
			
			if ((p.plateau[p.fantomes[i].l ][ p.fantomes[i].c -1 ] =='.'  || p.plateau[p.fantomes[i].l ][ p.fantomes[i].c -1 ] ==' ' ||  p.plateau[p.fantomes[i].l ][ p.fantomes[i].c -1 ] =='B' ||  p.plateau[p.fantomes[i].l ][ p.fantomes[i].c -1 ] =='P' ||  p.plateau[p.fantomes[i].l ][ p.fantomes[i].c -1 ] =='F'  ) && p.plateau[26][i]  != 'd')
				{
				Gauche=pow ( p.pacman.l - p.fantomes[i].l  , 2 ) + pow ( p.pacman.c - p.fantomes[i].c + 1 , 2 );	
				}
			else 
				{
				if (Super_Pacball==0)
					Gauche=999999;
				else 
					Gauche=0;
				}
		
		
																											// Verification du fait que le fantomes puisse bouger vers le bas puis calcule de la distance avec pacman
			
			if (( p.plateau[p.fantomes[i].l +1][ p.fantomes[i].c ] =='.' || p.plateau[p.fantomes[i].l +1][ p.fantomes[i].c ] ==' ' || p.plateau[p.fantomes[i].l +1][ p.fantomes[i].c ] =='B' || p.plateau[p.fantomes[i].l +1][ p.fantomes[i].c ] =='P'  || p.plateau[p.fantomes[i].l +1][ p.fantomes[i].c ] =='F' ) && p.plateau[26][i]  != 'h')
				{
				Bas=pow ( p.pacman.l - p.fantomes[i].l - 1  , 2 ) + pow ( p.pacman.c - p.fantomes[i].c , 2 );
				}
			else 
				{
				if (Super_Pacball==0)
					Bas=999999;
				else 
					Bas=0;
				}
				
																											// Verification du fait que le fantomes puisse bouger vers la droite puis calcule de la distance avec pacman
			
			if ( (p.plateau[p.fantomes[i].l ][ p.fantomes[i].c +1  ]  =='.' || p.plateau[p.fantomes[i].l ][ p.fantomes[i].c +1  ]  ==' ' || p.plateau[p.fantomes[i].l ][ p.fantomes[i].c +1  ]  =='B' || p.plateau[p.fantomes[i].l ][ p.fantomes[i].c +1  ]  =='P'  || p.plateau[p.fantomes[i].l ][ p.fantomes[i].c +1  ]  =='F' )&&  p.plateau[26][i]  != 'g')
				{
				Droite=pow ( p.pacman.l - p.fantomes[i].l   , 2 ) + pow ( p.pacman.c - p.fantomes[i].c - 1 , 2 );
				}
			else 
				{
				if (Super_Pacball==0)
					Droite=999999;
				else 
					Droite=0;
				}
				
				
																											// le fantome choisira le chemin le plus court pour aller vers pacman et le prendra
			if (Super_Pacball==0)
			{
				if (Haut < Droite && Haut < Gauche && Haut < Bas )
				{
					p.fantomes[i].l=p.fantomes[i].l -1;
					p.plateau[26][i] ='h';
				}
				else 
				{
					if (Gauche < Droite  && Gauche < Bas )
					{
						if (p.fantomes[i].c<0) 
							{
								p.fantomes[i].c=p.C-1;
							}
							
						p.fantomes[i].c=p.fantomes[i].c-1;
						p.plateau[26][i] ='g';
					}
				
					else 
					{
						if (Bas < Droite  )
						{
							p.fantomes[i].l=p.fantomes[i].l+1;
							p.plateau[26][i] ='b';
						}
						else
						{
							if (p.fantomes[i].c>(p.C-1)) 
								{
									p.fantomes[i].c=0;
								}
							p.fantomes[i].c=p.fantomes[i].c+1;
							p.plateau[26][i] ='d';
							
						}
					}
				}
			}
			else 																					// s'il y a une superpacball active alors le fantome va prendre le chemin le plus long
			{
				if (Haut > Droite && Haut > Gauche && Haut > Bas )
				{
					p.fantomes[i].l=p.fantomes[i].l -1;
					p.plateau[26][i] ='h';
				}
				else 
				{
					if (Gauche > Droite  && Gauche > Bas )
					{
						if (p.fantomes[i].c<0) 
							{
								p.fantomes[i].c=p.C-1;
							}
						p.fantomes[i].c=p.fantomes[i].c-1;
						p.plateau[26][i] ='g';
					}
				
					else 
					{
						if (Bas > Droite  )
						{
							p.fantomes[i].l=p.fantomes[i].l+1;
							p.plateau[26][i] ='b';
						}
						else
						{
							if (p.fantomes[i].c>(p.C-1)) 
								{
									p.fantomes[i].c=0;
								}
							p.fantomes[i].c=p.fantomes[i].c+1;
							p.plateau[26][i] ='d';
						}
					}
				}
			}
		}
	return p ;
}

Partie Quelqu_un_Manger(Partie p, int tour_sous_pacball)						// fonction qui regarde si pacman à manger un fantôme ou l'inverse
{
	int i ;
	p.plateau[0][11]='*' ;																			// initialisation de la zone du plateau qui contient l'information si un fantôme à été mangé (donne 40 points au score)

	if (tour_sous_pacball==0)																	// s'il n'y a pas de tour_sous_pacball active alors les fantomes peuvent manger pacman
	{
		for (i=0 ; i != NBFANTOMES ; i++)
			if (p.pacman.c == p.fantomes[i].c && p.pacman.l == p.fantomes[i].l)
				p.plateau[0][0]='!';
	}

	if (tour_sous_pacball==1)																	// sinon c'est l'inverse
	{
		for (i=0 ; i != NBFANTOMES ; i++)
			if (p.pacman.c == p.fantomes[i].c && p.pacman.l == p.fantomes[i].l) // si les fantômes sont mangés alors il retourne dans leur base
			{
				p.fantomes[i].c=9 ;																
				p.fantomes[i].l=13 ;
				p.plateau[0][11]='!' ;
			}

	}
	return p;
}


Partie Sortie_Fantome(Partie p , int tour )												// fonction qui gère la sortie des fantômes de leur base
{
	if (tour %25 ==0)  																				//tout les 25 tours on fait sortir un fantôme 
	{
		for (int i = 0 ; i < 4 ; i++)
		{
			if (p.fantomes[i].c <13  && p.fantomes[i].c >9   && p.fantomes[i].l <14 && p.fantomes[i].l >11)
			{
				p.fantomes[i].c = 11 ;
				p.fantomes[i].l = 10 ;
				break ;																					// Dès qu'un fantôme est sortie on arrête la sortie des fantômes pour ce tour.
			}
		}
	}
	return p ;
}

//main 
int main(int argc, char **argv)
    {
    Partie  p;      																						// la structure contenant les données d'une partie

    /** Traitement des arguments **************************************************/
    if(argc!=2) 																						// Si le nombre d'arguments est différent de 2
        {
        printf("Usage: %s fichier_plan\n", argv[0]); // mode d'emploi et sortie
        return 0;
        }
    /******************************************************************************/

																											// Chargement du plan à partir du fichier fourni en paramètre
    printf("Chargement du plan...\n");
    p = charge_plan(argv[1]);
																											// Si problème lors du chargement du plan...
    if(p.plateau == NULL)
        return 1;

#if DEBUG==1 																						//Ceci est un drapeau à activer/désactiver dans main.h
																											// Affichage du plan lu 
    int i, j;
	printf("Affichage du plan...\n");
    for(i=0;i!=p.L;i++)
        {
        for(j=0;j!=p.C;j++)
            printf("%c",p.plateau[i][j]);
        printf("\n");
        }
#endif

/******************************************************************************/
/* A PARTIR D'ICI...C'EST A VOUS DE JOUER!                                    */
/******************************************************************************/
    ouvrir_fenetre(p.C*30,p.L*30); 															//initialisation de la surface graphique où le plateau sera affiché
    int boucle_principale() ;
    boucle_principale(p); 																			// boucle où sera exécutée la gestion du jeu
    attendre_clic();																					// permet d'attendre un clic avant de faire disparaître le plateau
    return 0;
    }
    
