//
// Created by matet on 30/01/2020.
//

#include "analyseurLexical.h"



//ok
void ERREUR(int numeroErreur){
    switch (numeroErreur){
        case 1:
            printf("Erreur n°%d / Ligne n°%d : Fin de fichier atteinte ",numeroErreur, NUM_LIGNE);
            break;
        case 2:
            printf("Erreur n°%d / Ligne n°%d : L'entier lu depasse la taille maximale",numeroErreur, NUM_LIGNE);
            break;
        case 3:
            printf("Erreur n°%d / Ligne n°%d : La chaine de characteres depasse la longeur maximale",numeroErreur, NUM_LIGNE);
            break;
    }
    exit(0);
}

//ok
void LIRE_CAR(){
    if (SOURCE != NULL){
        CARLU = fgetc(SOURCE);
        if (CARLU == '\n') NUM_LIGNE++; //Fin de ligne
        if (CARLU == EOF) ERREUR(1); //Fin de fichier
    }
}

void SAUTER_SEPARATEURS() {
    bool flag = false;
    while (!flag) {
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
}// TEST bon


T_UNILEX RECO_ENTIER(){
    while ((int)CARLU >= 48 && (int)CARLU <= 57) { //CODE ASCII : 48 à 57 = chiffres 0 à 9
        if(NOMBRE * 10 + (int)CARLU > MAX_INT) ERREUR(2);
        NOMBRE = NOMBRE * 10 + (int)CARLU; //Chiffre precedent multiplier par 10 sur lequel on additionne le chiffre actuel
        LIRE_CAR();
    }
    return NOMBRE;
}

char RECO_CHAINE(){
    int i = 0;
    LIRE_CAR();
    while (CARLU != "'"){
        if (i  >= LONG_MAX_CHAINE) ERREUR(3);
        CHAINE[i] = CARLU;
        LIRE_CAR();
        i++;
    }
    // voir le type de retour
    return CHAINE;
}

T_UNILEX RECO_IDENT_OU_MOT_RESERVE(){

    int i = 0;
    while(((int)CARLU >= 48 && (int)CARLU <= 57) || ((int)CARLU >= 65 && (int)CARLU <= 90) || ((int)CARLU >= 97 && (int)CARLU <= 122) || CARLU == '_'){
        if (i < LONG_MAX_CHAINE) {
            if ((int)CARLU >= 97 && (int)CARLU <= 122) CARLU -= 32;
            CHAINE[i] = CARLU;
        }
        LIRE_CAR();
        i++;
        if (EST_UN_MOT_RESERVE()) return motcle;
        else return ident;
    }
}

bool EST_UN_MOT_RESERVE(){//metre en locale !!!
    int i = 0;
    do {
        if (CHAINE == TABLE_MOTS_RESERVES[0]) return true;
        i++;
    } while (strcmp(CHAINE, TABLE_MOTS_RESERVES[0]) != 1 || i < NB_MOTS_RESERVES);
    return false;
}


T_UNILEX RECO_SYMB(){
    switch (CARLU)
    {
        case ';':
            return ptvirg;
            break;
        case '.':
            return point;
            break;
        case '=':
            return eg;
            break;
        case '+':
            return plus;
            break;
        case '-':
            return moins;
            break;
        case '*':
            return mult;
            break;
        case '/':
            return divi;
            break;
        case '(':
            return parouv;
            break;
        case ')':
            return parfer;
            break;
        case '>':
            LIRE_CAR();
            if(CARLU == '=' )
                return supe;
            break;
            if(CARLU != '='){
                return sup;
                break;
            }
        case ':':
            LIRE_CAR();
            if(CARLU == '=' )
                return aff;
            break;
            if (CARLU != '='){
                return deuxpts;
                break;
            }
        case '<':
            LIRE_CAR();
            if(CARLU == '=' )
                return infe;
            break;
            if( CARLU == '>'){
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

void ANALEX(){
    if ((CARLU == ' ')|| (CARLU == '{')){
        SAUTER_SEPARATEURS();
    }
    else if (((int)CARLU >= 48) && ((int)CARLU <= 57)){ // un entier
        RECO_ENTIER();
    }
    else if (CARLU == "'"){
        RECO_CHAINE();
    }
    else if ((int)CARLU >= 65 && (int)CARLU <= 90) || ((int)CARLU >= 97 && (int)CARLU <= 122) || CARLU == '_'){
        //reco ident OU MOT CLE
        RECO_IDENT_OU_MOT_RESERVE();
    }
    else
        RECO_SYMB();
}


void INSERE_TABLE_RESERVES(char nouveauMot[]){
    int i = 0;
    bool flag = true;
    while (flag){
        int tableLenght = 0;
        for (tableLenght; tableLenght < NB_MOTS_RESERVES; ++i) {
            if((int)TABLE_MOTS_RESERVES[tableLenght][0] == 0) break;
        }
        int strcmpResult = strcmp(nouveauMot,TABLE_MOTS_RESERVES[i]); //Comparaison entre le nouveauMot et le mot positionné dans la table
        // (strcmpResult == -1)nouveauMot inferieur / on continue
        // (strcmpResult == 0) nouveauMot identique / on continue
        if (strcmpResult == 1) { //nouveauMot devient donc le plus grand mot
            for (int j = tableLenght; j < i + 1; --j) {
                strcpy(TABLE_MOTS_RESERVES[j], TABLE_MOTS_RESERVES[j - 1]);
            }
            flag = false;
        }
        i++;
    }
}

void INITIALISER(){
    NUM_LIGNE = 1;
    SOURCE = fopen("SOURCE.txt", "r");
    char TABLE_MOTS_RESERVES[NB_MOTS_RESERVES][9] = {{0},{0}}; // initialise le tableau avec des '0'
    INSERE_TABLE_RESERVES("PROGRAMME");
    INSERE_TABLE_RESERVES("DEBUT");
    INSERE_TABLE_RESERVES("FIN");
    INSERE_TABLE_RESERVES("CONST");
    INSERE_TABLE_RESERVES("VAR");
    INSERE_TABLE_RESERVES("ECRIRE");
    INSERE_TABLE_RESERVES("LIRE");
    TABLE_MOTS_RESERVES;
}



void TERMINER(){
    fclose(SOURCE);
}
