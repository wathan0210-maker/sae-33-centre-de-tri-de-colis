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

    Colis c1 = {1, 'A', 16};

    printf("Voici le colis n°%d qui va vers la zone %c et qui pèse %d kg\n", c1.id, c1.dest, c1.poids);
}