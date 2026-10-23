#include "../includes/pipes.h"

#define TAILLE_MAX 100
#define CHAR_MAJ "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CHAR_MIN "abcdefghijklmnopqrstuvwxyz"

size_t len_tab = strlen(CHAR_MAJ) + strlen(CHAR_MIN);
char tab[len_tab];

/* Sonne l'autre fils : écrit 1 octet dans son tube de signalisation */
static void sonner(int fd) {
  if (write(fd, "x", 1) != 1) {
    perror("sonner");
    exit(EXIT_FAILURE);
  }
}

/* Attend que l'autre fils sonne : read bloque tant que rien n'arrive */
static void attendre(int fd) {
  char c;
  if (read(fd, &c, 1) != 1) {
    /* 0 : l'autre fils est mort sans sonner ; -1 : erreur */
    fprintf(stderr, "attendre : l'autre fils ne répond plus\n");
    exit(EXIT_FAILURE);
  }
}

/* Écrit 2 octets dans le tube de données */
static void ecrire_paire(int fd, const char *p) {
  if (write(fd, p, 2) != 2) {
    perror("write");
    exit(EXIT_FAILURE);
  }
}

static void fils_1(int fd_data, int fd_sonner_2, int fd_attendre_2) {

  char *seq = CHAR_MAJ;
  size_t len = srtlen(seq);

  for (size_t i = 0; i < len; i += 2) {
    ecrire_paire(fd_data, seq + i); /* 1. j'écris AB                   */
    sonner(fd_sonner_2);            /* 2. je dis au fils 2 "à toi"     */
    attendre(fd_attendre_2);        /* 3. j'attends qu'il ait écrit    */
  }

  close(fd_data);
  close(fd_sonner_2);
  close(fd_attendre_2);
  exit(EXIT_SUCCESS);
}