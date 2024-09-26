#include <stdio.h>
#include <stdlib.h>
#define BLOSUM62 "../data/BLOSUM62.txt" // La matrice de substitution
#define GAP -10 // La penalité de gap


int sequence_length(char* filename){
    // On va regarder la taille de la séquence
    FILE* fichier = NULL;
    int caractereActuel = 0, cpt=0, flag = -1;
    /* flag permet de sauter la première ligne, 
    cpt va compter les caractères de la séquence
    */
    fichier = fopen(filename, "r");
    if (fichier != NULL){
        do { // On regarde les caractères un à un
            caractereActuel = fgetc(fichier); // On lit le caractère
            if (caractereActuel == '\n') { // On saute la première ligne
            	flag++;
            }
            if ((flag != -1) && (caractereActuel != '\n')){ // On saute les sauts à la ligne
            	cpt++;
            }
        } while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
        fclose(fichier);
    }
    return cpt - 1; // On ne prend pas en compte le dernier caractère
}

char* read_sequence(char* filename){
    int taille = sequence_length(filename);
    char* sequence = malloc((taille+1)*sizeof(char)); // On compte \0
    FILE* fichier = NULL;
    int caractereActuel = 0, cpt=0, flag = -1;
    /* flag permet de sauter la première ligne, 
    cpt va compter les caractères de la séquence, comme ça on pourra
    terminer la chaine avec \0
    */
    fichier = fopen(filename, "r");
    if (fichier != NULL){
        do { // On regarde les caractères un à un
            caractereActuel = fgetc(fichier); // On lit le caractère
            if (caractereActuel == '\n') { // On saute la première ligne
            	flag++;
            }
            if ((flag != -1) && (cpt < taille) && (caractereActuel != '\n')) { // On prend bien que la chaine
            	sequence[cpt] = caractereActuel;
                cpt++;
            }
        } while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
        fclose(fichier);
    }
    sequence[cpt+1] = '\0'; // On rajoute le dernier caractère \0
    return sequence;
}

int** read_substitution_matrix(char* filename){
    int** matrice = malloc(24 * sizeof(int*)); // On donne d'abord la mémoire de chaque ligne
    for (int i = 0; i < 24; i++){ // On donne ensuite la mémoire pour chaque colonne
        matrice[i] = malloc(24 * sizeof(int**));
    }
    FILE* fichier = NULL;
    int caractereActuel = 0, flag = 0, flag2 = 0, row = -1, col = 0; 
    /* flag permet de se placer directement à la première ligne de la matrice
    flag2 permet de sauter le premier caractère, l'acide aminé
    row permet de compter les lignes, initié à -1 pour prendre en compte le premier retour à la ligne
    col permet de compter les colonnes
    */
    fichier = fopen(filename, "r");
    
    if (fichier != NULL){ // On vérifie que le fichier n'est pas vide
        do { // On regarde les caractères un à un
            caractereActuel = fgetc(fichier); // On lit le caractère
            if (caractereActuel == '\n'){ // On saute les premières lignes
            	flag++;
            }
            if (flag > 6) { // On commence à lire au niveau de la matrice
                flag2++;
                if (flag2 > 2){ // On enlève la première colonne
                    if ((caractereActuel == '-') || ((caractereActuel != ' ') && (caractereActuel != '\n'))){ // Si valeur négative
                        // On regarde les nombres négatifs ou les nombres à deux chiffres
                        char nombre_2c[3]; // Création d'une chaine 
                        nombre_2c[0] = caractereActuel; // Ajout du - ou du premier chiffre
                        caractereActuel = fgetc(fichier); // On prends le caractère suivante (la valeur)
                        nombre_2c[1] = caractereActuel; // Ajout de la valeur
                        nombre_2c[2] = '\0'; // Fin de chaine
                        int nombre_2 = atoi(nombre_2c); // Conversion de la chaine en son entier
                        matrice[row][col] = nombre_2; // Ajout à notre matrice
                        col++;
                    } else if ((caractereActuel != ' ') && (caractereActuel != '\n')) { // On ne compte que les 'vrais' caractères
                        int nombre = caractereActuel - '0'; // Conversion de la chaine en son entier
                        matrice[row][col] = nombre; // Ajout à notre matrice
                        col++;
                    }
                    
                }
                if (caractereActuel == '\n'){ // Retour à la ligne
                    flag2 = 0; // On repart de 0 pour enlever la première colonne
                    row++; // On incrémente la ligne
                    col = 0; // On remet la colonne à 0
                }
            }
        } while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
        fclose(fichier);
    }
    return matrice;
}

int** initialize_matrix(int n, int m, int value){
    int** matrice = malloc(n * sizeof(int*)); // On donne d'abord la mémoire de chaque ligne
    for (int i = 0; i < n; i++){ // On donne ensuite la mémoire pour chaque colonne
        matrice[i] = malloc(m * sizeof(int**));
    }
    for (int r; r < n; r++){ // On initialise chaque case
        for (int c; c < m; c++){
            matrice[r][c] = value;
        }
    }
    return matrice;
}

int convert_aa(char c){
    // Fonction qui associe chaque acide aminé à l'index qu'il a dans la matrice de substitution
    switch(c){
        case 'A' : return 0; break;
        case 'R' : return 1; break;
        case 'N' : return 2; break;
        case 'D' : return 3; break;
        case 'C' : return 4; break;
        case 'Q' : return 5; break;
        case 'E' : return 6; break;
        case 'G' : return 7; break;
        case 'H' : return 8; break;
        case 'I' : return 9; break;
        case 'L' : return 10; break;
        case 'K' : return 11; break;
        case 'M' : return 12; break;
        case 'F' : return 13; break;
        case 'P' : return 14; break;
        case 'S' : return 15; break;
        case 'T' : return 16; break;
        case 'W' : return 17; break;
        case 'Y' : return 18; break;
        case 'V' : return 19; break;
        case 'B' : return 20; break;
        case 'Z' : return 21; break;
        case 'X' : return 22; break;
        case '*' : return 23; break;
        // Si le caractère a une autre valeur que celles possible, on quitte
        default: printf("Il y a une erreur dans la conversion d'acide aminé"); exit(EXIT_FAILURE);
    }    
}

int compute_score(char *a, char *b, int** substitution_matrix){
    int index_row = convert_aa(a[0]); // On récupère l'indice de ligne
    int index_col = convert_aa(b[0]); // On récupère l'indice de colonne
    int score = substitution_matrix[index_row][index_col]; // On récupère le score associé
    return score;
}

int MAX(int x,int y,int z){
    // Fonction pour calculer le maximum de nos trois cases
    int max = x;
    if(y > max){
        max = y;
    }
    if(z > max){
        max = z;
    }
    return max;
}

int calculate_alignement(char* seq1, char* seq2, int** substitution_matrix, int gap_penalty, char** aligned_seq1, char** aligned_seq2){
    // On crée la matrice de score
    int longueur1 = 0, longueur2 = 0, initialise = 0; // Notre valeur d'initialisation de la matrice
    for (int i = 0; seq1[i] != '\0'; i++){longueur1 ++;}; // Taille de la séquence 1
    for (int j = 0; seq2[j] != '\0'; j++){longueur2 ++;}; // Taille de la séquence 2
    int** matrice_score = initialize_matrix(longueur2+1, longueur1+1, initialise); // On rajoute la première case vide
    printf("On a initialisé notre matrice de score de %d lignes, %d colonnes à %d.\n", longueur2+1, longueur1+1, initialise);
  
    // On remplit la matrice de score
   for (int c = 1; c < longueur1+1; c++){ // On saute la première case
        matrice_score[0][c] = matrice_score[0][c-1] + gap_penalty; // On remplit la première ligne
    }
    for (int r = 1; r < longueur2+1; r++){ // On saute la première case
        matrice_score[r][0] = matrice_score[r-1][0] + gap_penalty; // On remplit la première colonne
    }


    // On peut donc commencer à calculer le score à partir de la case [1][1]
    for (int row = 1; row < longueur2+1; row++){
        for (int col = 1; col < longueur1+1; col++){
            int dessus = matrice_score[row-1][col] + gap_penalty;
            int gauche = matrice_score[row][col-1] + gap_penalty;
            int diag = matrice_score[row-1][col-1] + compute_score(&seq1[col-1], &seq2[row-1], substitution_matrix); // On compense le fait qu'on a une case en plus dans la matrice de score
            int score_max = MAX(dessus, gauche, diag);
            matrice_score[row][col] = score_max;
        }
    }
// Affichage de la matrice de score
/*
    int row, columns;
    for (row=0; row<longueur2+1; row++)
    {
        for(columns=0; columns<longueur1+1; columns++)
        {
             printf(" %d ", matrice_score[row][columns]);
        }
        printf("\n");
    }
*/
    printf("On a rempli notre matrice de score, on va la parcourir pour déterminer l'alignement optimal.\n");
 
    // On peut créer de nos séquences alignées (temporaires donc pas de malloc)
    int taille_max = longueur1 + longueur2; // Au pire on aligne rien
    char seq1_align[taille_max+1]; // +1 pour prendre en compte \0
    seq1_align[taille_max] = '\0';
    char seq2_align[taille_max+1];
    seq2_align[taille_max] = '\0';

    // On initialise nos nouvelles séquences avec un espace
    for (int i = 0; i < taille_max; i++) {
            seq1_align[i] = ' ';
            seq2_align[i] = ' ';
    }
    
    // On peut remonter le chemin
    int r = longueur2, c = longueur1;
    printf("On commence à la dernière case, soit la case [%d][%d].\n", r, c);
    
    // On parcourt jusqu'à atteindre les bords
    taille_max--; // On commence après le \0 et on va remplir en commençant par la fin
    while ((r != 0) && (c != 0)){
        int score = matrice_score[r][c];
        // On va prendre le premier chemin qui marche
        
        if (score == matrice_score[r-1][c] + gap_penalty){
            // On va en haut
            seq1_align[taille_max] = '-'; // Gap dans la séquence 1
            seq2_align[taille_max] = seq2[r-1];
            r--;
        } else if (score == matrice_score[r][c-1] + gap_penalty){
            // On va à gauche
            seq1_align[taille_max] = seq1[c-1];
            seq2_align[taille_max] = '-'; // Gap dans la séquence 2
            c--;
        } else if (score == matrice_score[r-1][c-1] + compute_score(&seq1[c-1], &seq2[r-1], substitution_matrix)) {
            // On va en diagonale
            seq1_align[taille_max] = seq1[c-1]; // Alignement entre les deux acides aminés
            seq2_align[taille_max] = seq2[r-1];
            r--;
            c--;
        }
        taille_max--; // Permet d'avoir les séquences bien alignées, on commence par la fin
    }
    
    // On s'occupe des bords
    if ((r == 0) && (c != 0)){ // On est sur la première ligne
        while (c != 0){
            // On va à gauche
            seq1_align[taille_max] = seq1[c-1];
            seq2_align[taille_max] = '-'; // Gap dans la séquence 2
            c--;
            taille_max--;
        }
    } else if ((c == 0) && (r != 0)){ // On est sur la première colonne
        while (r != 0){
            // On va en haut
            seq1_align[taille_max] = '-'; // Gap dans la séquence 1
            seq2_align[taille_max] = seq2[r-1];
            r--;
            taille_max--;
        }
    }
    if ((r == 0) && (c == 0)){
        printf("On termine à la première case, soit la case [%d][%d].\n", r, c); 
    } else {
        printf("On n'arrive pas à la première case de la matrice de score, il y a une erreur pour retracer le chemin de l'alignement optimal.\n");
        exit(EXIT_FAILURE);
    }
    printf("On a terminé le calcul de l'alignement optimal.\n"); 
    
    // On récupère la taille de l'alignement (pareil pour les deux séquences)
    int taille_align = 0;
    for (int i = 0; seq1_align[i] != '\0'; i++){
        if (seq1_align[i] != ' '){ // On saute les espaces
            taille_align += 1;
        }
    }
    printf("On a un alignement sur %d acides aminés. ", taille_align);

    // On va enlever les espaces de nos deux chaines, on crée de nouvelles chaines avec la taille de l'alignement    
    char *seq1_align_nospace = malloc(sizeof(char) * (taille_align+1)); // +1 pour prendre en compte \0
    seq1_align_nospace[taille_align] = '\0';
    char *seq2_align_nospace = malloc(sizeof(char) * (taille_align+1));
    seq2_align_nospace[taille_align] = '\0';

    // On remplit la nouvelle séquence 1 alignée sans espace
    int cpt1 = 0, cpt2 = 0;
    for (int i = 0; seq1_align[i] != '\0'; i++){
        if (seq1_align[i] != ' '){ // On saute les espaces
            seq1_align_nospace[cpt1] = seq1_align[i];
                cpt1++;
        }
    }

    // On remplit la nouvelle séquence 2 alignée sans espace
    for (int j = 0; seq2_align[j] != '\0'; j++){
        if (seq2_align[j] != ' '){ // On saute les espaces
            seq2_align_nospace[cpt2] = seq2_align[j];
                cpt2++;
        }
    }
    
    // On modifie nos séquences pour obtenir les séquences alignées
    *aligned_seq1 = seq1_align_nospace;
    *aligned_seq2 = seq2_align_nospace;
    printf("Les séquences alignées ont bien été récupérées.\n");

    // Score de similarité, la dernière case de notre matrice pour un alignement global
    int similarite = matrice_score[longueur2][longueur1];
    return similarite;
}

double print_alignment(char* aligned_seq1, char* aligned_seq2){    
    // On récupère la taille de l'alignement (pareil pour les deux séquences)
    int taille_align = 0;
    for (int i = 0; aligned_seq1[i] != '\0'; i++){
            taille_align += 1;
    }
    
    // On affiche nos deux alignements 100 caractères par 100 caractères
    printf("On obtient l'alignement suivant :\n");
    for (int i = 0; i < taille_align; i += 100) { // On incrémente 100 par 100
        printf("%.100s\n", aligned_seq1 + i); 
        printf("%.100s\n", aligned_seq2 + i);
        if (i+100 < taille_align){ // Pour séparer les alignements si besoin 
            for (int sep = 0; sep<100;sep++){ // On affiche 100 fois un = pour séparer l'alignement sur plusieurs lignes
                printf("%c", '=');
            }
            printf("\n");
        }
    }
    
    // On calcule le score d'identité
    int id = 0;
    for (int i = 0; i < taille_align; i ++) { // On incrémente 100 par 100
        if (aligned_seq1[i] == aligned_seq2[i]){
            id++;
        }
    }
    double score_id = ((double) id) / taille_align;
    return score_id;
}


int main(int argc, char *argv[]){
    // Vérification qu'on a les deux fichiers en arguments
    if (argc != 3){
        printf("Veillez à mettre deux fichiers fasta en arguments.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("\n    -Récupération des données-\n"); // \n au début et à la fin pour aérer l'affichage
    // Récupération de la première séquence
    printf("On récupère la première séquence. "); 
    char *sequence1 = read_sequence(argv[1]);
    
    // Vérification qu'on a la bonne taille de séquence
    int taille1 = sequence_length(argv[1]);
 	int longueur1 = 0;
    for (int i = 0; sequence1[i] != '\0'; i++){ longueur1 ++;};
    if (taille1 == longueur1){printf("On a la bonne taille pour la première séquence, soit %d acides aminés.\n", taille1);}
    else {
        printf("On n'a pas la bonne taille pour la première séquence. On trouve %d en lecture du fichier et on obtient %d en lecture de chaine.\n", taille1, longueur1);
        exit(EXIT_FAILURE);
    }
    
    // Affichage de la première séquence
    if (longueur1 < 11){ // Pour les petites séquences
        printf("Première séquence : ");
        printf("%s\n", sequence1);
    } else { // Pour les grandes séquences
        printf("Les 10 premiers acides aminés de la première séquence sont : %.10s\n", sequence1);
    }
    
    // Récupération de la seconde séquence
    printf("On récupère la seconde séquence. ");
    char *sequence2 = read_sequence(argv[2]);
    
    // Vérification qu'on a la bonne taille de séquence
    int taille2 = sequence_length(argv[2]);
 	int longueur2 = 0;
    for (int i = 0; sequence2[i] != '\0'; i++){ longueur2 ++;};
    if (taille2 == longueur2){printf("On a la bonne taille pour la seconde séquence, soit %d acides aminés.\n", taille2);}
    else {
        printf("On n'a pas la bonne taille pour la seconde séquence. On trouve %d en lecture du fichier et on obtient %d en lecture de chaine.\n", taille2, longueur2);
        exit(EXIT_FAILURE);
    }
    
    // Affichage de la seconde séquence
    if (longueur2 < 11){ // Pour les petites séquences
        printf("Seconde séquence : ");
        printf("%s\n", sequence2);
    } else { // Pour les grandes séquences
        printf("Les 10 premiers acides aminés de la seconde séquence sont : %.10s\n", sequence2);
    }
    
    // Récupération de la matrice de substitution
    int** matrice_sub = read_substitution_matrix(BLOSUM62);

    // Vérifiaction du remplissage de la matrice de substitution
    // La case[7][9] a un score de -4 dans la BLOSUM62, on va le vérifier
    if (matrice_sub[7][9] == -4){
        printf("La matrice de substitution a été récupérée avec succès.\n");
    } else {
        printf("La matrice de substitution n'a pas pu être récupérée.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n    -Construction de la matrice de score-\n"); // \n au début et à la fin pour aérer l'affichage
    // Vérification de la lecture de la matrice de substitution
    // WW a un score de 11 dans la BLOSUM62, on va le lire
    char sequence_test1[2];
    char sequence_test2[2];
    sequence_test1[0] = 'W';
    sequence_test2[0] = 'W';
    int test = compute_score(&sequence_test1[0], &sequence_test2[0], matrice_sub);
    if (test == 11){
        printf("La lecture de la matrice de substitution se fait sans erreur.\n");
    } else {
        printf("Il y a une erreur dans la lecture de la matrice de substitution.\n");
        exit(EXIT_FAILURE);
    }

    // Déclaration des séquences alignées
    char *seq1_align="";
    char *seq2_align="";

    // Calcul du score de similarité
    int similarite = calculate_alignement(sequence1, sequence2, matrice_sub, GAP, &seq1_align, &seq2_align);
    printf("\n    -Résultats de l'alignement-\n"); // \n au début et à la fin pour aérer l'affichage
    printf("Le score de similarité des deux séquences est de %d.\n", similarite);

    // Affichage des séquences alignées
    double identite = print_alignment(seq1_align, seq2_align);
    
    // Affichage du score d'identité
    printf("Le score d'identité des deux séquences est de %.2f, on a %.1f%% d'identité des deux séquences.\n\n", identite, identite*100); // deux \n à la fin pour aérer l'affichage
    
    // Libération nos variables
    free(sequence1);
    free(sequence2);
    free(matrice_sub);
    free(seq1_align);
    free(seq2_align);
    return 0;
}