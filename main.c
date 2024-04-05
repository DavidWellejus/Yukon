#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "deck.h"
#include "file.h"

void printStartupScreen();
void initializeDeck(Deck *deck);
bool saveDeckToFile(const Deck *deck, const char *filename);
void shuffleDeck(Deck* deck);
Deck* loadDeckFromFile(const char *filename);
void dealToTableau(Deck *deck, Tableau *tableau); // Antager at denne funktion eksisterer
void printTableau(Tableau *tableau); // Antager at denne funktion eksisterer


int main() {
    Deck *deck = malloc(sizeof(Deck));
    Tableau *tableau = initializeTableau();  // Tilføjet oprettelse af tableau
    if (deck == NULL || tableau == NULL) {
        fprintf(stderr, "Failed to allocate memory for deck or tableau.\n");
        exit(EXIT_FAILURE);
    }

    printStartupScreen();

    char command[256];
    char filename[256];

    while (true) {
        scanf("%255s", command);

        if (strcmp(command, "LD") == 0) {
            if (scanf("%255s", filename) == 1) {
                // Hvis et filnavn er angivet
                if (!loadDeckFromFile(filename)) {
                    printf("Error: Invalid file or unable to load deck.\n");
                } else {
                    dealToTableau(deck, tableau);  // Del kortene ud til tableau
                    printTableau(tableau);  // Viser de 7 bunker af kort i overensstemmelse med startScreen formatet
                }
            }
            else {
                // Hvis ingen filnavn er angivet, initialiserer et nyt ublandet dæk
                initializeDeck(deck);
                shuffleDeck(deck);  // Bland dækket før det deles ud
                dealToTableau(deck, tableau);
                printTableau(tableau);
            }
        } else if (strcmp(command, "QQ") == 0) {
            // Afslutter programmet
            break;
        } else {
            printf("Unknown command.\n");
        }

        // Ryd input buffer for at undgå at gamle input påvirker de næste læsninger
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }

    // Frigiver hukommelsen for dækket og tableauet her
    free(deck);
    free(tableau);  // Antager at du har en funktion til at rydde op i tableauet
    return 0;
}


void printStartupScreen() {

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    printf("\t\t\t\t\t\t\t[] F1\n");
    printf("\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t[] F2\n");
    printf("\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t[] F3\n");
    printf("\t\t\t\t\t\t\t\n");
    printf("\t\t\t\t\t\t\t[] F4\n\n");

    printf("LAST Command:\n");
    printf("Message:\n");
    printf("INPUT > ");
}



