#include <stdio.h>
#include "analyseurLexical.h"
#include "tableDesIdentificateurs.h"

void INITIALISER();
void TERMINER();

int main() {
    INITIALISER();
    INSERER("ZEBI",variable);
    INSERER("SABER",constante);
    printf(" est a la position %d\n",CHERCHER("SABER"));
    AFFICHE_TABLE_IDENT();
    TERMINER();
}


void INITIALISER() {
    //AnalyseurLexical
    NUM_LIGNE = 1;
    SOURCE = fopen("SOURCE.txt", "r");
    LIRE_CAR();// pour commencer
    INSERE_TABLE_RESERVES("PROGRAMME");
    INSERE_TABLE_RESERVES("DEBUT");
    INSERE_TABLE_RESERVES("FIN");
    INSERE_TABLE_RESERVES("CONST");
    INSERE_TABLE_RESERVES("VAR");
    INSERE_TABLE_RESERVES("ECRIRE");
    INSERE_TABLE_RESERVES("LIRE");
    //TableDesIdent
    NB_IDENT = 0;
    TABLE_SYMBOLES = malloc(NB_IDENT * sizeof(T_ENREG_IDENT));
    TABLE_INDEX = malloc(NB_IDENT * sizeof(int));
}


void TERMINER() {
    //AnalyseurLexical
    fclose(SOURCE);
    //TableDesIdent
    free(TABLE_SYMBOLES);
}
