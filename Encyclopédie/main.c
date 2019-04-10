#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define TABLEAU 10000

// -------------------------------------------LISTES-------------------------------------------------------


struct Maillon_liste { // Structure de la liste
    int id;
    char* titre;
    char* contenu;
    struct Maillon_liste * suivant;
};
typedef struct Maillon_liste * l_Encyclopedie;

l_Encyclopedie liste_creer_encyclopedie() { // La fonction crée une encyclopédie vide
    return NULL;
}

void est_deja_pris (l_Encyclopedie e, int id) { // Fonction qui vérifie si l'id qu'on veut utiliser est déjà pris
    while (e != NULL) {
        if (id == e->id) {
            printf("La clef est deja utilise\n");
            break;
        }
        e = e->suivant;
    }
}

l_Encyclopedie liste_inserer(l_Encyclopedie e, int id, char* titre, char* contenu) { // Fonction qui insère un nouvel élement dans la liste

    //est_deja_pris(e, id);

    l_Encyclopedie e_copie = (l_Encyclopedie)malloc(sizeof(struct Maillon_liste)); // On crée une copie dans laquelle on copie l'id, le titre et le contenu
    // On insère l'élement en tête de liste
    e_copie->id = id;

    e_copie->titre = malloc((strlen(titre) + 1) * sizeof(char)); // On alloue la taille d'un caractère fois le nombre de caractère
    strcpy(e_copie->titre, titre);

    e_copie->contenu = malloc((strlen(contenu) + 1) * sizeof(char));
    strcpy(e_copie->contenu, contenu);

    e_copie->suivant = e; // On place le nouvel article en tête de liste

    return e_copie;
}

l_Encyclopedie liste_supprimer(l_Encyclopedie e, int id) { // Fonction qui supprime un élement dans la liste avec son ID

    l_Encyclopedie e_copie = e; // On crée deux copies de e
    l_Encyclopedie e_temp = e;

    if (e == NULL) { // Test si la liste est vide
        printf("La liste est vide\n");
        return e;
    } else if (e->id == id) { // Si l'élement à supprimer est en tête de liste
        e_copie = e->suivant; // On défini le premier élement de la copie de e comme le deuxieme de e
        free(e);
        return e_copie; // On retourne la nouvelle liste sans le premier élement (élement à supprimer)
    } else {
        while (e_copie->id != id) { // On parcourt toute la liste
            e_temp = e_copie; // On se place juste avant le maillon qu'on examine
            e_copie = e_copie->suivant;
            if (e_copie == NULL) { // Si on arrive à la fin de la liste
                printf("La clef de l'article a supprimer n'est pas dans la liste\n");
                return e;
            }
        }
        e_temp->suivant = e_copie->suivant; // On lie les deux maillons à côté de celui à supprimer
        free(e_copie);
        return e;
    }
}

char* liste_rechercher_article (l_Encyclopedie e, int id) {
    if (e == NULL) {
        printf("La liste est vide\n");
        return NULL;
    }
    while (e != NULL) {
        if(e->id == id) {
            return e->contenu;
        }
        e = e->suivant;
    }
    return NULL;
}

l_Encyclopedie liste_rechercher_article_plein_texte(l_Encyclopedie e, char* mot) {
    l_Encyclopedie e_mot = NULL;
    if (e == NULL) {
        printf("La liste est vide");
        return e_mot;
        //exit(0);
    } else {
        while (e != NULL) { // On parcourt la liste
            if (strstr(e->contenu, mot) != NULL) {
                e_mot = liste_inserer(e_mot, e->id, e->titre, e->contenu); // Si le mot est dans l'article on l'insère dans la nouvelle encyclopédie
            }
            // Si on arrive en bout de liste on arrête la boucle
            e = e->suivant;
        }
        return e_mot;
    }
}

void liste_detruire_bibliotheque(l_Encyclopedie e) {
    l_Encyclopedie e_copie = e;
        while (e != NULL) {
            e_copie = e->suivant;
            free(e); // On libère l'espace
            e = e_copie;
        }
}

l_Encyclopedie liste_separer_article() { // Fonction qui ouvre le fichier wikipedia et sépare les ID, titres et contenus
    FILE *file_liste;
    file_liste = fopen("B46_wikipedia_500k_random.dat","r");
    l_Encyclopedie e = NULL;
    if (file_liste == NULL) { // Si le fichier est vide on retourne une liste vide
        return NULL;
    }
    char article[1000000];
    char *id_char, *titre, *contenu;
    int id;
    const char separation[2] = "|";
    while (fgets(article, 1000000, file_liste) != NULL) { // On récupère toutes les lignes une par une
        id_char = strtok(article, separation); // On sépare le tableau article à chaque "|"
        id = atoi(id_char); // On converti une chaine de caractère en entier
        titre = strtok(NULL, separation);
        contenu = strtok(NULL, "\n");
        e = liste_inserer(e, id, titre, contenu); // On insère ce qu'on a récupérer dans une liste
    }
    fclose(file_liste);
    return e;
}

void afficher_liste (l_Encyclopedie e) {
    if (e == NULL) {
        printf("La liste est vide\n");
    } else {
        while (e != NULL) {
            printf("Article : %d / %s /%s\n", e->id, e->titre, e->contenu);
            e = e->suivant;
        }
    }
}

// --------------------------------------TABLEAU DE HACHAGE----------------------------------------------


typedef l_Encyclopedie * tab_Encyclopedie;

tab_Encyclopedie tab_creer_encyclopedie () {
    tab_Encyclopedie e = (tab_Encyclopedie)malloc(TABLEAU * sizeof(l_Encyclopedie));
    int i;
    for (i = 0; i < TABLEAU; i++) {
        e[i] = NULL;
    }
    return e;
}

int valeur_hachage (int x) {
    return x % TABLEAU;
}
// On utilise les fonctions sur les listes pour modifier la case que l'on souhaite
tab_Encyclopedie tab_inserer (tab_Encyclopedie e, int id, char* titre, char* contenu) {
    int clef = valeur_hachage(id);
    e[clef] = liste_inserer(e[clef], id, titre, contenu);
    return e;
}

tab_Encyclopedie tab_supprimer(tab_Encyclopedie e, int id) {
    int clef = valeur_hachage(id);
    e[clef] = liste_supprimer(e[clef], id);
    return e;
}

char* tab_rechercher_article(tab_Encyclopedie e, int id) {
    int clef = valeur_hachage(id);
    return liste_rechercher_article(e[clef], id);
}

tab_Encyclopedie tab_rechercher_article_plein_texte (tab_Encyclopedie e, char* mot) {
    int i;
    tab_Encyclopedie e_mot = tab_creer_encyclopedie();
    for (i = 0; i < TABLEAU; i++) {
        e_mot[i] = liste_rechercher_article_plein_texte(e[i], mot);
    }
    return e_mot;
}

void tab_detruire_bibliotheque(tab_Encyclopedie e) {
    int i;
    for (i = 0; i < TABLEAU; i++) {
        liste_detruire_bibliotheque(e[i]);
    }
}

tab_Encyclopedie tab_separer_article() { // Fonction qui ouvre le fichier wikipedia et sépare les ID, titres et contenus
    FILE *file_liste;
    file_liste = fopen("B46_wikipedia_500K_random.dat","r");
    tab_Encyclopedie e = tab_creer_encyclopedie();
    if (file_liste == NULL) { // Si le fichier est vide on retourne une liste vide
        return NULL;
    }
    char article[1000000];
    char *id_char, *titre, *contenu;
    int id;
    const char separation[2] = "|";
    while (fgets(article, 1000000, file_liste) != NULL) {
        id_char = strtok(article, separation);
        id = atoi(id_char);
        titre = strtok(NULL, separation);
        contenu = strtok(NULL, "\n");
        e = tab_inserer(e, id, titre, contenu);
    }
    fclose(file_liste);
    return e;
}

void afficher_tab (tab_Encyclopedie e) {
    int i;
    for (i = 0; i < TABLEAU; i++) {
        printf("--e[%d]\n", i);
        afficher_liste(e[i]);
        printf("\n");
    }
}

// ------------------------------------------ARBRES-------------------------------------------------------


struct Maillon_arbre{ //Structure d'un noeud de l'arbre
    int id;
    char* titre;
    char* contenu;
    struct Maillon_arbre * clef_inf;
    struct Maillon_arbre * clef_sup;
};
typedef struct Maillon_arbre * a_Encyclopedie;

a_Encyclopedie arbre_creer_encyclopedie() { //crée une encyclopedie vide
    return NULL;
}

a_Encyclopedie creer_arbre(a_Encyclopedie e, int id, char* titre, char* contenu) { //Crée un noeud (pour la f insérer)

    e = (a_Encyclopedie)malloc(sizeof(struct Maillon_arbre));

    //Insertion de tout les éléments dans le noeud
    e->id = id;

    e->titre = malloc((strlen(titre) + 1) * sizeof(char));
    strcpy(e->titre, titre);

    e->contenu = malloc((strlen(contenu) + 1) * sizeof(char));
    strcpy(e->contenu, contenu);

    e->clef_inf = NULL;
    e->clef_sup = NULL;
    return e;
}

a_Encyclopedie arbre_inserer(a_Encyclopedie e, int id, char* titre, char* contenu) { //Insère les infos d'un article dans le noeud e

    a_Encyclopedie e_copie;

    if (e == NULL) { //On insère dans un noeud vide
        e_copie = creer_arbre(e, id, titre, contenu);
        return e_copie;
    }
    if (id == e->id) {
        printf("La clef est déjà utilise\n");
    } else if (id < e->id) { //recherche du noeud
        e->clef_inf = arbre_inserer(e->clef_inf, id, titre, contenu);
    } else if (id > e->id) {
        e->clef_sup = arbre_inserer(e->clef_sup, id, titre, contenu);
    }
    return e;
}

a_Encyclopedie arbre_supprimer(a_Encyclopedie e, int id) { //Supprime le noeud e
    //Si l'encyclopédie est vide
    if (e == NULL) {
        printf("L'arbre est vide\n");
        return NULL;
    }
    if (e->id == id) {
        //Si le noeud à suppr est une feuille
        if (e->clef_inf == NULL && e->clef_sup == NULL) {
            free(e);
            e = arbre_creer_encyclopedie();
        } else if (e->clef_inf == NULL && e->clef_sup != NULL) { //si le noeud a un de ses sous arbres nuls
            a_Encyclopedie i = e->clef_sup;
            free(e);
            e = i;
        } else if (e->clef_inf != NULL && e->clef_sup == NULL) {
            a_Encyclopedie j = e->clef_inf;
            free(e);
            e = j;
        } else if (e->clef_inf != NULL && e->clef_sup != NULL) { //si le noeud a deux sous arbres non nuls
            a_Encyclopedie a;

            a = e->clef_inf; //On remonte le fils le + à droite du noeud gauche dans le noeud courant

            while (a->clef_sup != NULL) {
                //	printf("%p\n", a);
                a = a->clef_sup;
            }

            e->id = a->id;  //On copie les infos
            strcpy(e->titre, a->titre);
            strcpy(e->contenu, a->contenu);

            //Suppression du noeud copié
            e->clef_inf = arbre_supprimer(e->clef_inf, a->id);
        }
    } else { //Recherche du noeud à supprimer
        if (e->id > id) {
            e->clef_inf = arbre_supprimer(e->clef_inf, id);
        }
        if (e->id < id) {
            e->clef_sup = arbre_supprimer(e->clef_sup, id);
        }
    }
    return e;
}

char* arbre_rechercher_article(a_Encyclopedie e, int id) { //Recherche l'article d'identifiant id dans e
    char* r=NULL;
    //Cas pour une encyclopedie vide
    if (e == NULL) {
        printf("L'arbre est vide\n");
        return r;
    }
    if (id == e->id) { //compare l'id rechercher avec celui de l'encyclopedie e
        r = e->contenu;
    } else if (e->id < id) {
        r = arbre_rechercher_article(e->clef_sup, id);
    } else {
        r = arbre_rechercher_article(e->clef_inf, id);
    }
    return r;
}

a_Encyclopedie lier(a_Encyclopedie a, a_Encyclopedie b) { //Sous-fonction pour la fonction arbre_rechercher_article_plein_texte
    if (a == NULL) {
        return b;
    } else {
        b = arbre_inserer(b, a->id, a->titre, a->contenu); //insère les infos de l'arbre a dans b

        //lie les deux arbres
        if (a->clef_inf != NULL) {
            b = lier(a->clef_inf, b);
        } else if (a->clef_sup != NULL) {
            b = lier(a->clef_sup, b);
        }
        return b;
    }
}

a_Encyclopedie arbre_rechercher_article_plein_texte(a_Encyclopedie e, char* mot) { //Recherche tout les artcles contenant le mot mot
    a_Encyclopedie e1=NULL;
    a_Encyclopedie e2 = NULL;

    //Cas où l'encyclopedie est vide
    if (e == NULL) {
        printf("L'arbre est vide\n");
        return NULL;
    } else if (strstr(e->contenu, mot) !=NULL) { //recherche le mot dans le contenu de l'article
        e1 = arbre_inserer(e1, e->id, e->titre, e->contenu);
    }
    e2 = lier(arbre_rechercher_article_plein_texte(e->clef_inf, mot), arbre_rechercher_article_plein_texte(e->clef_sup, mot)); //recherche le mot dans tout les articles
    e1 = lier(e1, e2);
    return e1;
}

void arbre_detruire_bibliotheque(a_Encyclopedie e) { //vide et détruit e
    if (e->clef_inf !=NULL) {
        arbre_detruire_bibliotheque(e->clef_inf);
    }
    if (e->clef_sup != NULL) {
        arbre_detruire_bibliotheque(e->clef_sup);
    }
    free(e); //vide et détruit la bibliotheque
    e = NULL;
}

a_Encyclopedie arbre_separer_article() { // Fonction qui ouvre le fichier wikipedia et sépare les ID, titres et contenus
    FILE *file_liste;
    file_liste = fopen("B46_wikipedia_500K_random.dat","r");
    a_Encyclopedie e = arbre_creer_encyclopedie();
    if (file_liste == NULL) { // Si le fichier est vide on retourne une liste vide
        return NULL;
    }
    char article[1000000];
    char *id_char, *titre, *contenu;
    int id;
    const char separation[2] = "|";
    while (fgets(article, 1000000, file_liste) != NULL) {
        id_char = strtok(article, separation);
        id = atoi(id_char);
        titre = strtok(NULL, separation);
        contenu = strtok(NULL, "\n");
        e = arbre_inserer(e, id, titre, contenu);
    }
    fclose(file_liste);
    return e;
}

void afficher_arbre(a_Encyclopedie e) { //Affiche une encyclopedie (pour les tests)
    if (e == NULL) {
        return;
    } else {
        printf("id : %d | titre : %s | contenu : %s\n", e->id, e->titre, e->contenu);
    }
    afficher_arbre(e->clef_inf);
    afficher_arbre(e->clef_sup);
}

int main() {

    // TEST SUR LES LISTES
    char l_t1[10] = "Titre1";
    char l_c1[10] = "Contenu1";
    char l_t2[10] = "Titre2";
    char l_c2[10] = "Contenu2";
    char l_t3[10] = "Titre3";
    char l_c3[10] = "Contenu3";
    char l_t_t[10] = "Test_t";
    char l_c_t[10] = "Contenu_t";
    //test sur le fichier wikipedia
/*
    l_Encyclopedie l_ew = liste_creer_encyclopedie();
    l_ew = liste_separer_article();
    //afficher_liste(l_ew);
    //test de inserer
    l_ew = liste_inserer(l_ew, 41, l_t_t, l_c_t);
    printf("id : %d, titre : %s, contenu : %s\n", l_ew->id, l_ew->titre, l_ew->contenu); // affiche id : 41, titre : Test_t, contenu : Contenu_t
    //test de supprimer
    l_ew = liste_supprimer(l_ew, 11218729);
    //afficher_liste(l_ew); // affiche toute l'encyclopedie sauf le dernier article sur le vermont
    //test de rechercher_article
    char* l_testw1 = liste_rechercher_article(l_ew, 5812967);
    printf("%s", l_testw1); // affiche l'article sur Bristol
    //test de rechercher_article_plein_texte
    l_Encyclopedie l_testw2 = liste_creer_encyclopedie();
    l_testw2 = liste_rechercher_article_plein_texte(l_ew, "France");
    afficher_liste(l_testw2);
    liste_detruire_bibliotheque(l_ew);
*/
    //test sur des petites listes
/*
    l_Encyclopedie l_e1 = liste_creer_encyclopedie();
    l_e1 = liste_inserer(l_e1, 4, l_t3, l_c3);
    l_e1 = liste_inserer(l_e1, 5, l_t2, l_c2);
    l_e1 = liste_inserer(l_e1, 12, l_t1, l_c1);
    l_Encyclopedie l_e2 = liste_creer_encyclopedie();


    //test de liste_inserer

    l_e1 = liste_inserer(l_e1, 41, l_t_t, l_c_t);
    l_e2 = liste_inserer(l_e2, 41, l_t_t, l_c_t);
    afficher_liste(l_e1);
    afficher_liste(l_e2);

    //test de liste_supprimer

    l_e1 = liste_supprimer(l_e1,41);
    afficher_liste(l_e1);
    l_e2 = liste_supprimer(l_e2, 12);


    //test de rechercher article

    char* l_test1 = liste_rechercher_article(l_e1, 4);
    printf("%s\n", l_test1);


    //test de rechercher plein texte

    l_Encyclopedie l_test2 = liste_creer_encyclopedie();
    l_test2 = liste_rechercher_article_plein_texte(l_e1, l_c1);
    afficher_liste(l_e1);
    l_test2 = liste_rechercher_article_plein_texte(l_e2, l_c1);

    //test de detruire bibliotheque

    liste_detruire_bibliotheque(l_e1);
    liste_detruire_bibliotheque(l_e2);
*/


    // TEST SUR LES TABLEAU DE HACHAGE
/*
    //test sur le fichier wikipedia
    tab_Encyclopedie t_ew = tab_creer_encyclopedie();
    t_ew = tab_separer_article();
    //afficher_tab(t_ew);

    //test de inserer
    t_ew = tab_inserer(t_ew, 41, l_t_t, l_c_t);
    printf("id : %d, titre : %s, contenu : %s\n", t_ew[valeur_hachage(41)]->id, t_ew[valeur_hachage(41)]->titre, t_ew[valeur_hachage(41)]->contenu); // affiche id : 41, titre : Test_t, contenu : Contenu_t
    //test de supprimer
    t_ew = tab_supprimer(t_ew, 11218729);
    //afficher_tab(t_ew); // affiche toute l'encyclopedie sauf le dernier article sur le vermont
    //test de rechercher_article
    char* t_testw1 = tab_rechercher_article(t_ew, 5812967);
    printf("%s", t_testw1); // affiche l'article sur Bristol
    //test de rechercher_article_plein_texte
    tab_Encyclopedie t_testw2 = tab_creer_encyclopedie();
    t_testw2 = tab_rechercher_article_plein_texte(t_ew, "France");
    afficher_tab(t_testw2);
    tab_detruire_bibliotheque(t_ew);
*/
/*
    char t_titre[10] = "Titre_t";
    char t_contenu[10] = "Contenu_t";

    tab_Encyclopedie t_e1 = tab_creer_encyclopedie();

    //test sur tab_inserer

    t_e1 = tab_inserer(t_e1, 12, t_titre,t_contenu);
    t_e1 = tab_inserer(t_e1, 5, t_titre, t_contenu);
    afficher_tab(t_e1);

    //test sur tab_supprimer

    t_e1 = tab_supprimer(t_e1, 12);
    afficher_tab(t_e1);

    //test sur rechercher_article

    char* t_test1 = tab_rechercher_article(t_e1, 5);
    printf("%s", t_test1);

    //test sur rechercher_article_plein_texte

    tab_Encyclopedie t_e2 = tab_creer_encyclopedie();
    t_e2 = tab_rechercher_article_plein_texte(t_e1, t_test1);
    afficher_tab(t_e2);

    //test sur detruire_bibliotheque

    tab_detruire_bibliotheque(t_e1);
    tab_detruire_bibliotheque(t_e2);
*/


    // TEST SUR LES ARBRES
/*
    a_Encyclopedie a_ew = arbre_creer_encyclopedie();
    a_ew = arbre_separer_article();
    //afficher_arbre(a_ew);
    //test de inserer
    a_ew = arbre_inserer(a_ew, 41, l_t_t, l_c_t);
    printf("id : %d, titre : %s, contenu : %s\n", a_ew->id, a_ew->titre, a_ew->contenu); // affiche id : 41, titre : Test_t, contenu : Contenu_t
    //test de supprimer
    a_ew = arbre_supprimer(a_ew, 11218729);
    printf("id : %d, titre : %s, contenu : %s\n", a_ew->id, a_ew->titre, a_ew->contenu); // affiche id : 41, titre : Test_t, contenu : Contenu_t
    //afficher_arbre(a_ew); // affiche toute l'encyclopedie sauf le dernier article sur le vermont
    //test de rechercher_article
    char* a_testw1 = arbre_rechercher_article(a_ew, 5812967);
    printf("%s", a_testw1); // affiche l'article sur Bristol
    //test de rechercher_article_plein_texte
    a_Encyclopedie a_testw2 = arbre_creer_encyclopedie();
    a_testw2 = arbre_rechercher_article_plein_texte(a_ew, "France");
    afficher_arbre(a_testw2);
    arbre_detruire_bibliotheque(a_ew);
*/
/*
    a_Encyclopedie e = arbre_creer_encyclopedie();

    //liste des noeuds (articles)
    int i1 = 4;
    char t1[20] = "Titre1";
    char c1[20] = "Contenu1";

    int i2 = 5;
    char t2[20] = "Titre2";
    char c2[20] = "Contenu2";

    int i3 = 12;
    char t3[20] = "Titre3";
    char c3[20] = "Contenu3";

    int i4 = 20;
    char t4[20] = "Titre4";
    char c4[20] = "Contenu4";

    int i5 = 30;
    char t5[20] = "Titre5";
    char c5[20] = "Contenu5";

    int i6 = 40;
    char t6[20] = "Titre6";
    char c6[20] = "Contenu6";

    char t_t[20] = "Test_t";
    char c_t[20] = "Contenu_t";


    //test de liste_inserer
    a_Encyclopedie e1 = arbre_creer_encyclopedie();
    a_Encyclopedie e7 = arbre_creer_encyclopedie();
    e1 = arbre_inserer(e1, 4, t1, c1);
    e1 = arbre_inserer(e1, 5, t2, c2);
    e1 = arbre_inserer(e1, 12, t3, c3);
    e1 = arbre_inserer(e1, 41, t_t, c_t); //test sur une encyclopedie pleine
    e7 = arbre_inserer(e7, 41, t_t, c_t); //test sur une encyclopedie vide
    printf("Affiche 4 articles :\n");
    arbre_afficher(e1);
    printf("Affiche l'article d'id 41 :\n");
    arbre_afficher(e7);

    //test de arbre_supprimer

    e = arbre_inserer(e, i1, t1, c1);
    e = arbre_inserer(e, i2, t2, c2);
    e = arbre_inserer(e, i3, t3, c3);
    e = arbre_inserer(e, i4, t4, c4);
    e = arbre_inserer(e, i5, t5, c5);
    e = arbre_inserer(e, i6, t6, c6);

    printf("Affiche tout les articles sauf le 2 : \n"); //Supprime un noeud quelconque
    e = arbre_supprimer(e, i2);
    arbre_afficher(e);

    printf("Affiche tout les articles sauf le 6 : \n"); //Supprime une feuille
    e = arbre_supprimer(e, i6);
    arbre_afficher(e);

    printf("Affiche tout les articles : \n"); //Supprime un article inexistant
    e = arbre_supprimer(e, NULL);
    arbre_afficher(e);

    //test de arbre_rechercher_article

    e = arbre_inserer(e, i1, t1, c1);
    e = arbre_inserer(e, i2, t2, c2);
    e = arbre_inserer(e, i3, t3, c3);
    e = arbre_inserer(e, i4, t4, c4);
    e = arbre_inserer(e, i5, t5, c5);
    e = arbre_inserer(e, i6, t6, c6);

    //arbre_afficher(e);
    char * b ;
    char * n;
    char * v;
    b = arbre_rechercher_article(e, 50);//Test pour un id inexistant
    n = arbre_rechercher_article(NULL, 5);//Test pour un arbre vide
    v = arbre_rechercher_article(e, 20);//Test
    printf("Affiche NULL :\n");
    printf("%s\n", b);
    printf("Affiche NULL :\n");
    printf("%s\n", n);
    printf("Affiche Contenu4 :\n");
    printf("%s\n", v); é

    //test de arbre_rechercher_article_plein_texte

    e = arbre_inserer(e, i1, t1, c1);
    e = arbre_inserer(e, i2, t2, c2);
    e = arbre_inserer(e, i3, t3, c3);
    e = arbre_inserer(e, i4, t4, c4);
    e = arbre_inserer(e, i5, t5, c5);
    e = arbre_inserer(e, i6, t6, c6);

    printf("Test de la fonction arbre_rechercher_article_plein_texte : \n");

    arbre_afficher(e);
    char cont2[10] = "test"; //On teste la recherche avec un mot qui n'apparait dans aucun article
    char cont1[10] = "Contenu"; //On teste la recherche avec un mot qui apparait dans tout les articles

    a_Encyclopedie f = arbre_rechercher_article_plein_texte(e, cont1);
    a_Encyclopedie g = arbre_rechercher_article_plein_texte(e, cont2);
    printf("6 articles devraient apparaitre ici :\n");
    arbre_afficher(f);
    printf("Aucun article ne devrait apparaitre ici :\n");
    arbre_afficher(g);
*/
    return 0;
}
