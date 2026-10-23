#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    typedef struct {
        int id;
        char dest;
        int poids;
    } Colis;

    typedef struct {
        char dest;
        size_t nbColis;
        int poidsTotal;
    } Zone;

    int i, masse;
    char destination;

    printf("Quel est l'identifiant de votre colis ? : ");
    scanf("%d", &i);

    printf("Quel est la zone de destination de votre colis ? : ");
    scanf(" %c", &destination);

    printf("Quel est le poids de votre colis (en kg) ? : ");
    scanf("%d", &masse);

    if(i != NULL && (destination == 'A' || destination == 'B' || destination == 'C') && masse > 0) {
        Colis c1 = {i, destination, masse};
    } else {
        // continuer
    }

    printf("Voici le colis n°%d qui va vers la zone %c et qui pèse %d kg\n", c1.id, c1.dest, c1.poids);
}