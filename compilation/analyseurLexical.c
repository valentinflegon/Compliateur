#include "analyseurLexical.h"
#include "analyseurSyntaxique.h"
#include "tableDesIdentificateurs.c"


/******************************
*   PROCEDURE: ERREUR(int numeroErreur)
*   La procédure prend en argument un numéro d'erreur
*   et affiche le message d'erreur correspondant à ce 
*   numéro avec la ligne ou l'erreur a été détectée.
*   L'appel de cette procédure provoque l'arrêt du programme.
*
********************************/
void ERREUR(int numeroErreur){
    switch (numeroErreur){
          case 0:
            printf("Erreur n°%d / Ligne n°%d :ERREUR syntaxique",numeroErreur, NUM_LIGNE);
            break;
        case 1:
            printf("Erreur n°%d / Ligne n°%d : Fin de fichier atteinte ",numeroErreur, NUM_LIGNE);
            break;
        case 2:
            printf("Erreur n°%d / Ligne n°%d : L'entier lu depasse la taille maximale",numeroErreur, NUM_LIGNE);
            break;
        case 3:
            printf("Erreur n°%d / Ligne n°%d : La chaine de characteres depasse la longeur maximale",numeroErreur, NUM_LIGNE);
            break;
        case 4:
            printf("Erreur n°%d / Ligne n°%d : Caractère non valide ",numeroErreur, NUM_LIGNE);
            break;
        case 5:
            printf("Erreur n°%d / Ligne n°%d : Mauvaise initialisation du tableau mot réervé ",numeroErreur, NUM_LIGNE);
            break;
        case 6:
            printf(" Ligne n°%d : %s",NUM_LIGNE,MESSAGE_ERREUR);
            break;
        case 7:
            printf(" Erreur dans l'interpreter ");
            break;
    }
    exit(0);
}

/******************************
*   PROCEDURE: LIRE_CAR()
*   La procédure, va lire un caractère du fichier source ,
*   et l'affecter à la variable globale CARLU.
*   Si le caractère lu est un fin de ligne \n alors 
*   la variable globale NUM_LIGNE est incrémenté.
*   Si le caractère lu est un fin de fichier alors 
*   cela appel la fonction ERREUR avec le numéro d'erreur 1.
*   
********************************/
void LIRE_CAR(){
    if (SOURCE != NULL){
        CARLU = fgetc(SOURCE);
        if (CARLU == '\n'){
            NUM_LIGNE++ ; //Fin de ligne
            LIRE_CAR();
        } 
        if (CARLU == EOF) ERREUR(1); //Fin de fichier
    }
}

/******************************
*   PROCEDURE: SAUTER_SEPARATEURS()
*   probleme avec cette procedure 
*   
********************************/
void SAUTER_SEPARATEURS() {
    bool flag = false;
    while (!flag) {
        LIRE_CAR();//
        while (CARLU == ' '  || CARLU == '\n') {
            LIRE_CAR();
        }
        if (CARLU == '{') {
            while (CARLU != '}') {
                LIRE_CAR();
            }
            LIRE_CAR();
        }
        flag = true;
    }
}

/******************************
*   FONCTION: RECO_ENTIER()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
T_UNILEX RECO_ENTIER(){ 
    char* CHAINE= malloc(sizeof(CHAINE));
    while (((int)CARLU >= 48) && ((int)CARLU <= 57)) { 
       CHAINE = strcat(CHAINE,&CARLU);
        LIRE_CAR();
    }
    NOMBRE = atoi(CHAINE);
    if(NOMBRE>MAX_INT)  ERREUR(2); 
    return ent;
}

/******************************
*   FONCTION: RECO_CHAINE()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
T_UNILEX RECO_CHAINE(){
     for (int j=0;j < LONG_MAX_CHAINE;j++){
          CHAINE[j] = NULL;
    }
    int i =0;
    LIRE_CAR();
    while ((int)CARLU != 39){
        if (i  >= LONG_MAX_CHAINE) ERREUR(3);
        CHAINE[i] = CARLU; 
        LIRE_CAR();
        i++;
    }
    LIRE_CAR();
    return ch;
}

/******************************
*   FONCTION: RECO_IDENT_OU_MOT_RESERVE()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
T_UNILEX RECO_IDENT_OU_MOT_RESERVE(){
     int j,i=0;
    for (j=0;j < LONG_MAX_CHAINE;j++){
          CHAINE[j] = NULL;
    }
    while(((int)CARLU >= 48 && (int)CARLU <= 57) || ((int)CARLU >= 65 && (int)CARLU <= 90) || ((int)CARLU >= 97 && (int)CARLU <= 122) || CARLU == '_'){
        if (i < LONG_MAX_CHAINE) {
            if ((int)CARLU >= 97 && (int)CARLU <= 122) CARLU -= 32;{
                toupper(CARLU);
                CHAINE[i] = CARLU;
            }
            LIRE_CAR(); 
            i++;
        }
    }
    if (EST_UN_MOT_RESERVE()){//mot clé
        LIRE_CAR(); 
        return motcle;
    } 
    else {//ident
       // INSERER(CHAINE,variable);
        LIRE_CAR();
        return ident;
    }
}

/******************************
*   FONCTION: EST_UN_MOT_RESERVE()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*   prog && PROGRAMME 
********************************/
bool EST_UN_MOT_RESERVE(){
    int j,i,w;
    int sizeChaine = strlen(CHAINE);  
    for (int k=0;k<NB_MOTS_RESERVES;k++){
        w=0;
        for (i = 0; i<sizeChaine;i++){
            if (CHAINE[i] == TABLE_MOTS_RESERVES[k][i]){
                w++;
            }    
        } 
        if (w == sizeChaine ){
            return true;
        }
    }   
    return false;       
}

/******************************
*   FONCTION: RECO_SYMB()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
T_UNILEX RECO_SYMB(){
    switch (CARLU)
    {
    case ',':
        LIRE_CAR();
        return virg;
        break;
    case ';':
        LIRE_CAR();
        return ptvirg;
        break;
    case '.':
        LIRE_CAR();
        return point;
        break;
    case '=':
        LIRE_CAR();
        return eg;
        break;
    case '+':
        LIRE_CAR();
        return plus;
        break;
    case '-':
        LIRE_CAR();
        return moins;
        break;
    case '*':
        LIRE_CAR();
        return mult;
        break;
    case '/':
        LIRE_CAR();
        return divi;
        break;
    case '(':
        LIRE_CAR();
        return parouv;
        break;
    case ')':
        LIRE_CAR();
        return parfer;
        break;
    case '>':
        LIRE_CAR();
        if(CARLU == '=' )
            LIRE_CAR();
            return supe;
            break;
        if(CARLU != '='){
            LIRE_CAR();
            return sup;
            break;
        }
    case ':':
        LIRE_CAR();
        if(CARLU == '=' )
            LIRE_CAR();
            return aff;
            break;
        if (CARLU != '='){
            return deuxpts;
            break;
        }
    case '<':
        LIRE_CAR();
        if(CARLU == '=' )
            LIRE_CAR();
            return infe;
            break;
        if( CARLU == '>'){ 
            LIRE_CAR();
            return diff;
            break;
        }
        else
        {
            return inf;
            break;
        }
    default:
        ERREUR(4); // si le caractere n'est pas compris parmi eux alors on le connais
        break;
    }
}

/******************************
*   FONCTION: ANALEX()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
T_UNILEX ANALEX(){
    if ((CARLU == ' ')|| (CARLU == '{')){
        SAUTER_SEPARATEURS();
    }
    else if (((int)CARLU >= 48) && ((int)CARLU <= 57)){ // un entier
        RECO_ENTIER();
    }
    else if ((int)CARLU == 39){ // une apostrphe
        RECO_CHAINE();
    }
    else if (((int)CARLU >= 65 && (int)CARLU <= 90) || ((int)CARLU >= 97 && (int)CARLU <= 122)){     //reco ident OU MOT CLE
        RECO_IDENT_OU_MOT_RESERVE();
    }
    else
        RECO_SYMB();
}

/******************************
*   FONCTION: INSERE_TABLE_RESERVES()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
void INSERE_TABLE_RESERVES(char nouveauMot[]){
   int x=  strlen(nouveauMot);
   if (x <= 9 && CONST <= NB_MOTS_RESERVES) {
        for (int i =0; i<x;i++){
            TABLE_MOTS_RESERVES[CONST][i] = nouveauMot[i];
        }
    }
    else ERREUR(5);
    CONST ++;
} 

/******************************
*   FONCTION: INITIALISER()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
void INITIALISER(){
    NUM_LIGNE = 1;
    NB_CONST_CHAINE = 0;
    DERNIERE_ADRESSE_VAR_GLOB = -1;
    SOURCE = fopen("SOURCE.txt", "r");
    INITIALISER_TABLE_IDENT();
    LIRE_CAR();// pour commencer 
    INSERE_TABLE_RESERVES("PROGRAMME");
    INSERE_TABLE_RESERVES("DEBUT");
    INSERE_TABLE_RESERVES("FIN");
    INSERE_TABLE_RESERVES("CONST");
    INSERE_TABLE_RESERVES("VAR");
    INSERE_TABLE_RESERVES("ECRIRE");
    INSERE_TABLE_RESERVES("LIRE");
    
}

/******************************
*   FONCTION: TERMINER()
*   Regarde que le programme est bien 
*   de la forme 
*
*   Retourne:
*
********************************/
void TERMINER(){
    TERMINER_TABLE_IDENT();
    fclose(SOURCE);
}