#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <math.h>

#define LONGUEUR 26
#define LARGEUR 80

typedef struct serpent {
    int posX;
    int posY;
    int longueur;
}serpent;

typedef struct fruit {
    int posX;
    int posY;
}fruit;

typedef struct obstacle {
    int posX;
    int posY;
}obstacle;

serpent snake;

int snakeposX[50]; // Tableau des positions en x du serpent
int snakeposY[50]; // Tableau des positions en y du serpent

fruit fruit1, fruit2, fruit3, fruit4, fruit5, fruit6, fruitia;
obstacle o1, o2, o3, o4, o5, oia;

char lab[LONGUEUR][LARGEUR]; // Plateau de jeu

// Affichage du plateau de jeu
void afficher(){
    int i, j;
    for (i = 0; i < LONGUEUR; i++) {
        for (j = 0; j < LARGEUR; j++) {
            printf("%c", lab[i][j]);
        }
        printf("\n");
    }
    printf("Score : %d\n\nUtiliser z,q,s,d pour vous deplacer\n", (snake.longueur - 3));
}


// Construction du labyrinthe
void construction_lab() {
    int i, j;

    // On crée une matrice vide
    for (i = 0; i < LONGUEUR; i++) {
        for (j = 0; j < LARGEUR; j++) {
            lab[i][j] = 32;
        }
    }

    i = 0;

    // Construction de mur du bas et du haut
    for (i = 0; i < LARGEUR; i++) {
        lab[0][i] = '#';
        lab[LONGUEUR - 1][i] = '#';
    }

    i = 0;

    // Construction du mur gauche et droite
    for (i = 0; i < LONGUEUR; i++) {
        lab[i][0] = '#';
        lab[i][LARGEUR - 1] = '#';
    }

    // Afficher les fruits
    fruit1.posX = 20;
    fruit1.posY = 40;
    lab[fruit1.posX][fruit1.posY] = '*';

    fruit2.posX = 23;
    fruit2.posY = 5;
    lab[fruit2.posX][fruit2.posY] = '*';

    fruit3.posX = 12;
    fruit3.posY = 47;
    lab[fruit3.posX][fruit3.posY] = '*';

    fruit4.posX = 6;
    fruit4.posY = 89;
    lab[fruit4.posX][fruit4.posY] = '*';

    fruit5.posX = 20;
    fruit5.posY = 51;
    lab[fruit5.posX][fruit5.posY] = '*';

    fruit6.posX = 4;
    fruit6.posY = 79;
    lab[fruit6.posX][fruit6.posY] = '*';


    // Afficher les obstacles
    o1.posX = 17;
    o1.posY = 74;
    lab[o1.posX][o1.posY] = 'o';

    o2.posX = 10;
    o2.posY = 94;
    lab[o2.posX][o2.posY] = 'o';

    o3.posX = 23;
    o3.posY = 10;
    lab[o3.posX][o3.posY] = 'o';

    o4.posX = 4;
    o4.posY = 7;
    lab[o4.posX][o4.posY] = 'o';

    o5.posX = 2;
    o5.posY = 15;
    lab[o5.posX][o5.posY] = 'o';

    // Afficher Snake
    snake.posX = LONGUEUR / 2;
    snake.posY = LARGEUR / 2;
    snakeposX[2] = snake.posX; // position en x de la tête
    snakeposY[2] = snake.posY; // position en y de la tête
    snakeposX[1] = snake.posX; // position en x du corps
    snakeposY[1] = snake.posY + 1; // position en y du corps
    snakeposX[0] = snake.posX; // position de la queue en x
    snakeposY[0] = snake.posY + 2; // position de la queue en y
    lab[snake.posX][snake.posY] = '@';
    lab[snake.posX][snake.posY + 1] = '+';
    lab[snake.posX][snake.posY + 2] = '+';
}

int fruit_posX;
int fruit_posY;
void creer_fruit() { // On crée un nouveau fruit à un endroit aléatoire sur le tableau
    fruit_posX = rand()%25 + 1;
    fruit_posY = rand()%98 + 1;
    lab[fruit_posX][fruit_posX] = '*';
}

int compteur;
// Fonction pour effacer la queue du Snake
void effacer() {
    lab[snakeposX[0]][snakeposY[0]] = 32; // On remplace la queue du snake par ' '
    for (compteur = 0; compteur < snake.longueur; compteur++) { // On remplace les coordonnées pour chaque morceau du corps
        snakeposX[compteur] = snakeposX[compteur + 1]; // La position de la queue en x de Snake devient celle du corps
        snakeposY[compteur] = snakeposY[compteur + 1]; // La position de la queue en y de snake devient celle du corps
    }
    snakeposX[snake.longueur - 1] = snake.posX;
    snakeposY[snake.longueur - 1] = snake.posY;
}

void perdu1() {
    system("cls");
    printf("\nScore : %d\nVous avez perdu ! :(\n R : Rejouer\n M : Menu principal\n Q : Quitter\n", snake.longueur - 3); // Le score est égale au nombre de fruits mangés soit la longueur du serpent - 3
    char c = getch();
    if (c == 'r') {
        niveau1(); // On relance le niveau 1
    }
    if (c == 'm') {
        system("cls"); // On retourne au menu
        menu();
    }
    if (c == 'q') {
        exit(0); // On arrête le programme
    } else {
        system("cls");
        perdu1(); // Si on appuye sur une autre touche on relance le menu perdu
    }
}

// Fonction pour lancer le niveau 1
void niveau1() {
	char touche;
    snake.longueur = 3;
    construction_lab();
    afficher();
    while (1) {
        touche = getch(); // On récupère la touche sur laquelle on a appuyé

                /*
                        L'AXE X SE DIRIGE VERS LE BAS ET L'AXE Y VERS LA DROITE
                                                                                        */

        lab[snakeposX[0]][snakeposY[0]] = 32;

        if (touche == 'z') {
            lab[snake.posX][snake.posY] = '+'; // La tête (@) devient le corps (+)
            snake.posX--; // On déplace la tête de snake vers le haut

            if (lab[snake.posX][snake.posY] == '*') {
                snake.longueur++;
                creer_fruit();
                lab[snake.posX][snake.posY] = 32;
            }

            if (lab[snake.posX][snake.posY] == '+') {
                perdu1();
            }

            if (lab[snake.posX][snake.posY] == 32) {
                effacer();
            }
            lab[snake.posX][snake.posY] = '@';
            system("cls"); // On efface l'écran
        }

        if (touche == 'd') {
            lab[snake.posX][snake.posY] = '+';
            snake.posY++;

            if (lab[snake.posX][snake.posY] == '*') {
                snake.longueur++;
                creer_fruit();
                lab[snake.posX][snake.posY] = 32;
            }

            if (lab[snake.posX][snake.posY] == '+') {
                perdu1();
            }

            if (lab[snake.posX][snake.posY] == 32) {
                effacer();
            }

            lab[snake.posX][snake.posY] = '@';
            system("cls");
        }

        if (touche == 's') {
            lab[snake.posX][snake.posY] = '+';
            snake.posX++;

            if (lab[snake.posX][snake.posY] == '*') {
                snake.longueur++;
                creer_fruit();
                lab[snake.posX][snake.posY] = 32;
            }

            if (lab[snake.posX][snake.posY] == '+') {
                perdu1();
            }

            if (lab[snake.posX][snake.posY] == 32) {
                effacer();
            }

            lab[snake.posX][snake.posY] = '@';
            system("cls");
        }

        if (touche == 'q') {
            lab[snake.posX][snake.posY] = '+';
            snake.posY--;

            if (lab[snake.posX][snake.posY] == '*') {
                snake.longueur++;
                creer_fruit();
                lab[snake.posX][snake.posY] = 32;
            }

            if (lab[snake.posX][snake.posY] == '+') {
                perdu1();
            }

            if (lab[snake.posX][snake.posY] == 32) {
                effacer();
            }

            lab[snake.posX][snake.posY] = '@';
            system("cls");
        }
        lab[0][0] = '#';
        afficher(); // On réaffiche le plateau de jeu

		if(snake.posX < 1 || snake.posY < 1 || snake.posX > (LONGUEUR - 2) || snake.posY > (LARGEUR - 2) || (snake.posX == o1.posX && snake.posY == o1.posY) || (snake.posX == o2.posX && snake.posY == o2.posY) || (snake.posX == o3.posX && snake.posY == o3.posY) || (snake.posX == o4.posX && snake.posY == o4.posY) || (snake.posX == o5.posX && snake.posY == o5.posY)) {
            perdu1();
		}
        Sleep(200);
    }
}

time_t start2, stop2, test2;

void perdu2() {
    system("cls");
    time(&stop2); // On prends le temps lorsqu'on perd
    int score = (stop2 - start2) * (snake.longueur - 3); // Le score est égale au temps fois le nomnbre de fruits mangés
    printf("\nTemps : %d\nFruits manges : %d\nScore : %d\nVous avez perdu ! :(\n R : Rejouer\n M : Menu principal\n Q : Quitter\n", (stop2 - start2), snake.longueur - 3, score); // On affiche la différence de temps entre le moment où on lance le niveau et on perd
    char c = getch();
    if (c == 'r') {
        niveau2();
    }
    if (c == 'm') {
        system("cls");
        menu();
    }
    if (c == 'q') {
        exit(0);
    } else {
        system("cls");
        perdu2();
    }
}

void niveau2() {
    char touche;
    snake.longueur = 3;
    construction_lab();
    afficher();
    touche = getch();
    start2 = time(&start2);
    while (1) {
        if (kbhit()) { // La fonction kbhit détecte si on appuye sur une touche
            touche = getch(); // On récupére la tocuhe sur laquelle on a appuyé
        } else { // Si on n'appuie sur aucune touche on continue de se déplacer dans la direction de la dernière touche appuyée
            if (touche == 'z') {
                lab[snake.posX][snake.posY] = '+'; // La tête (@) devient le corps (+)
                snake.posX--; // On déplace la tête de snake vers le haut

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu2;
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }
                lab[snake.posX][snake.posY] = '@';
                system("cls"); // On efface l'écran
                lab[0][0] = '#';
                afficher();
            }

            if (touche == 'd') {
                lab[snake.posX][snake.posY] = '+';
                snake.posY++;

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu2();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }

                lab[snake.posX][snake.posY] = '@';
                system("cls");
                lab[0][0] = '#';
                afficher();
            }

            if (touche == 's') {
                lab[snake.posX][snake.posY] = '+';
                snake.posX++;

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu2();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }

                lab[snake.posX][snake.posY] = '@';
                system("cls");
                lab[0][0] = '#';
                afficher();
            }

            if (touche == 'q') {
                lab[snake.posX][snake.posY] = '+';
                snake.posY--;

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu2();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }

                lab[snake.posX][snake.posY] = '@';
                system("cls");
                lab[0][0] = '#';
                afficher();
            }

            // Si la tête a les mêmes coordonnées que soit un bord ou un obstacle alors on perd
            if(snake.posX < 1 || snake.posY < 1 || snake.posX > (LONGUEUR - 2) || snake.posY > (LARGEUR - 2) || (snake.posX == o1.posX && snake.posY == o1.posY) || (snake.posX == o2.posX && snake.posY == o2.posY) || (snake.posX == o3.posX && snake.posY == o3.posY) || (snake.posX == o4.posX && snake.posY == o4.posY) || (snake.posX == o5.posX && snake.posY == o5.posY)) {
                perdu2();
            }
            test2 = time(&test2);
            printf("Temps : %d", (test2 - start2));
            Sleep(150); // On ajoute un délai dans la boucle pour rendre le jeu plus fluide
        }
    }
}

time_t start3, stop3, test3;

void perdu3() {
    system("cls");
    stop3 = time(&stop3);
    int score = (stop2 - start2) * (snake.longueur - 3);
    printf("\nTemps : %d\nFruits manges : %d\nScore : %d\nVous avez perdu ! :(\n R : Rejouer\n M : Menu principal\n Q : Quitter\n", (stop2 - start2), snake.longueur - 3, score);
    char c = getch();
    if (c == 'r') {
        niveau3();
    }
    if (c == 'm') {
        system("cls");
        menu();
    }
    if (c == 'q') {
        exit(0);
    } else {
        system("cls");
        perdu3();
    }
}

void niveau3() {
    char touche;
    snake.longueur = 3;
    construction_lab();
    afficher();
    touche = getch();
    start3 = time(&start3);
    while (1) {
        if (kbhit()) { // La fonction kbhit détecte si on appuye sur une touche
            touche = getch(); // On récupére la tocuhe sur laquelle on a appuyé
        } else { // Si on n'appuie sur aucune touche on continue de se déplacer dans la direction de la dernière touche appuyée

            // Lorsque la tête de snake arrive à un mur, on redéfini la position de sa tête de l'autre côté du tableau
            if (snake.posX == 1) {
                snake.posX = LONGUEUR - 3;
            }
            if (snake.posY == 1) {
                snake.posY = LARGEUR - 3;
            }
            if (snake.posX > (LONGUEUR - 3)) {
                snake.posX = 1;
            }
            if (snake.posY > (LARGEUR - 3)) {
                snake.posY = 1;
            }

            if (touche == 'z') {
                lab[snake.posX][snake.posY] = '+'; // La tête (@) devient le corps (+)
                snake.posX--; // On déplace la tête de snake vers le haut

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu3();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }
                lab[snake.posX][snake.posY] = '@';
                system("cls"); // On efface l'écran
                lab[0][0] = '#';
                afficher();
            }

            if (touche == 'd') {
                lab[snake.posX][snake.posY] = '+';
                snake.posY++;

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu3();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }

                lab[snake.posX][snake.posY] = '@';
                system("cls");
                lab[0][0] = '#';
                afficher();
            }

            if (touche == 's') {
                lab[snake.posX][snake.posY] = '+';
                snake.posX++;

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu3();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }

                lab[snake.posX][snake.posY] = '@';
                system("cls");
                lab[0][0] = '#';
                afficher();
            }

            if (touche == 'q') {
                lab[snake.posX][snake.posY] = '+';
                snake.posY--;

                if (lab[snake.posX][snake.posY] == '*') {
                    snake.longueur++;
                    creer_fruit();
                    lab[snake.posX][snake.posY] = 32;
                }

                if (lab[snake.posX][snake.posY] == '+') {
                    perdu3();
                }

                if (lab[snake.posX][snake.posY] == 32) {
                    effacer();
                }

                lab[snake.posX][snake.posY] = '@';
                system("cls");
                lab[0][0] = '#';
                afficher();
            }

            // Si la tête a les mêmes coordonnées qu'un obstacle alors on perd
            if((snake.posX == o1.posX && snake.posY == o1.posY) || (snake.posX == o2.posX && snake.posY == o2.posY) || (snake.posX == o3.posX && snake.posY == o3.posY) || (snake.posX == o4.posX && snake.posY == o4.posY) || (snake.posX == o5.posX && snake.posY == o5.posY)) {
                perdu3();
            }
            test3 = time(&test3);
            printf("Temps : %d", (test3 - start3));
            Sleep(150);
        }
    }
}

// On crée un tableau avec un seul fruit pour le niveau avec ia
void construction_lab_ia() {
    system("cls");
    int i, j;

    // On crée une matrice vide
    for (i = 0; i < LONGUEUR; i++) {
        for (j = 0; j < LARGEUR; j++) {
            lab[i][j] = 32;
        }
    }

    i = 0;

    // Construction de mur du bas et du haut
    for (i = 0; i < LARGEUR; i++) {
        lab[0][i] = '#';
        lab[LONGUEUR - 1][i] = '#';
    }

    i = 0;

    // Construction du mur gauche et droite
    for (i = 0; i < LONGUEUR; i++) {
        lab[i][0] = '#';
        lab[i][LARGEUR - 1] = '#';
    }

    creer_fruit();

    // Afficher Snake
    snake.posX = LONGUEUR / 2;
    snake.posY = LARGEUR / 2;
    snakeposX[2] = snake.posX; // position en x de la tête
    snakeposY[2] = snake.posY; // position en y de la tête
    snakeposX[1] = snake.posX; // position en x du corps
    snakeposY[1] = snake.posY + 1; // position en y du corps
    snakeposX[0] = snake.posX; // position de la queue en x
    snakeposY[0] = snake.posY + 2; // position de la queue en y
    lab[snake.posX][snake.posY] = '@';
    lab[snake.posX][snake.posY + 1] = '+';
    lab[snake.posX][snake.posY + 2] = '+';
}

time_t startia, testia;

void niveau4(){
    snake.longueur = 3;
    construction_lab_ia();
    afficher();
    startia = time(&startia);
    while (1) {
        if (snake.posX == 1) {
            snake.posX = LONGUEUR - 3;
        }
        if (snake.posY == 1) {
            snake.posY = LARGEUR - 3;
        }
        if (snake.posX > (LONGUEUR - 3)) {
            snake.posX = 1;
        }
        if (snake.posY > (LARGEUR - 3)) {
            snake.posY = 1;
        }
        lab[snake.posX][snake.posY] = '+';

        if (snake.posX < fruit_posX) {
            snake.posX++; // Si les coordonnées en X du fruit sont supérieurs aux coordonnées en X de la tête alors on va vers le bas
        }
        if (snake.posX > fruit_posX) {
            snake.posX--; // Si les coordonnées en X du fruit sont inférieurs aux coordonnées en X de la tête alors on va vers le haut
        }
        if (snake.posX == fruit_posX) { // Quand les coordonnées du fruit et de la tête en X sont égales alors on se déplace maintenant sur l'axe Y
            if (snake.posY < fruit_posY) {
                snake.posY--; // Si les coordonnées en Y du fruit sont supérieurs aux coordonnées en Y de la tête alors on va vers la droite
            }
            if (snake.posY > fruit_posY) {
                snake.posY++; // Si les coordonnées en Y du fruit sont inférieurs aux coordonnées en Y de la tête alors on va vers la gauche
            }
        }

        if (lab[snake.posX][snake.posY] == '*') {
            snake.longueur++;
            creer_fruit();
            lab[snake.posX][snake.posY] = 32;
        }

        if (lab[snake.posX][snake.posY] == 32) {
            effacer();
        }

        lab[snake.posX][snake.posY] = '@';
        system("cls");
        lab[0][0] = '#';
        afficher();
        testia = time(&testia);
        printf("Temps : %d", (testia - startia));
        Sleep(125);
    }
}




// Fonction pour réafficher le menu avec la flèche devant l'option sélectionée
void fleche(int pos) {
    if (pos == 1) {
        printf("______________________________________JOUER A SNAKE______________________________________\n\n -> Niveau 1\n    Niveau 2\n    Niveau 3\n    Niveau 4\n    Quitter\n\n Utilisez les fleches directionelles pour vous deplacer dans le menu et entree pour comfirmer\n");
    }

    if (pos == 2) {
        printf("______________________________________JOUER A SNAKE______________________________________\n\n    Niveau 1\n -> Niveau 2\n    Niveau 3\n    Niveau 4\n    Quitter\n\n Utilisez les fleches directionelles pour vous deplacer dans le menu et entree pour comfirmer\n");
    }

    if (pos == 3) {
        printf("______________________________________JOUER A SNAKE______________________________________\n\n    Niveau 1\n    Niveau 2\n -> Niveau 3\n    Niveau 4\n    Quitter\n\n Utilisez les fleches directionelles pour vous deplacer dans le menu et entree pour comfirmer\n");
    }
    if (pos == 4) {
        printf("______________________________________JOUER A SNAKE______________________________________\n\n    Niveau 1\n    Niveau 2\n    Niveau 3\n -> Niveau 4\n    Quitter\n\n Utilisez les fleches directionelles pour vous deplacer dans le menu et entree pour comfirmer\n");
    }
    if (pos == 5) {
        printf("______________________________________JOUER A SNAKE______________________________________\n\n    Niveau 1\n    Niveau 2\n    Niveau 3\n    Niveau 4\n -> Quitter\n\n Utilisez les fleches directionelles pour vous deplacer dans le menu et entree pour comfirmer\n");
    }
}

void menu() {
    int pos = 1;
    int c;
    printf("______________________________________JOUER A SNAKE______________________________________\n\n -> Niveau 1\n    Niveau 2\n    Niveau 3\n    Niveau 4\n    Quitter\n\n Utilisez les fleches directionelles pour vous deplacer dans le menu et entree pour comfirmer\n");
    while (1) {
        c = getch();
        /*
            On utilise les flèches directionelles pour se déplacer dans le menu
         */
        if (c == 80) {
            pos++;
            if (pos == 6) {
                pos = 1; // Quand on est à la dernière option et appuie sur bas, on retourne en haut
            }
            system("cls");
            fleche(pos);
        }

        if (c == 72) {
            pos--;
            if (pos == 0) {
                pos = 5; // Quand on est à la première position et appuie sur haut on retourne en bas
            }
            system("cls");
            fleche(pos);
        }

        if (c == 97) {
            niveau2();
        }

        // On comfirme la sélection avec entrée
        if (c == 13) {
            if (pos == 1) {
                niveau1();
            }

            if (pos == 2) {
                niveau2();
            }

            if (pos == 3) {
                niveau3();
            }

            if (pos == 4) {
                niveau4();
            }
            if (pos == 5) {
                exit(0);
            }
        }
    }
}

int main() {
    system("title Snake");
    menu(); // Dès que le programme se lance, on lance le menu
    return 0;
}