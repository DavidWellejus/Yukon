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
void setShowAllCards(Tableau *tableau, bool isVisible);

int main() {
    Deck *deck = malloc(sizeof(Deck));
    Tableau *tableau = initializeTableau(); // Tilføjet oprettelse af tableau
    if (deck == NULL || tableau == NULL) {
        fprintf(stderr, "Failed to allocate memory for deck or tableau.\n");
        exit(EXIT_FAILURE);
    }
    //initializeDeck(deck); // Dette skal sandsynligvis kun gøres, hvis et nyt dæk skal oprettes
    //saveDeckToFile(deck, "C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/ShuffledDeck.txt");
    printStartupScreen();

    char inputLine[256];
    char command[256];
    char fileName[256];

    while (true) {
        printf("INPUT > ");
        fflush(stdout);

        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }

        if (sscanf(inputLine, "%s", command) == 1) {

            if (strcmp(command, "LD") == 0) {
                if (sscanf(inputLine, "%*s %s", fileName) == 1) {
                    // Hvis et filnavn er angivet
                    if (!loadDeckFromFile(fileName)) {
                        printf("Error: Invalid file or unable to load deck.\n");
                    } else {
                        deck = loadDeckFromFile(fileName); // Opdaterer deck med det indlæste dæk
                        shuffleDeck(deck);
                        dealToTableau(deck, tableau); // Del kortene ud til tableau
                        printTableau(tableau); // Viser de 7 bunker af kort i overensstemmelse med startScreen formatet
                    }
                } else {
                    deck = loadDeckFromFile("C:/DTU/2.Semester/02322MaskinaerProgrammering/lab/project2_machine/Yukon/Deck.txt");
                    dealToTableau(deck, tableau);
                    printTableau(tableau);
                }
            }
            else if (strcmp(command, "SW") == 0) {
                setShowAllCards(tableau, true); // Sætter alle kort til synlige
                printTableau(tableau); // Printer Tableau med synlige kort
            }
            else if (strcmp(command, "QQ") == 0) {
                // Afslutter programmet
                break;
            } else {
                printf("Unknown command.\n");
            }
        } else {
            printf("Error: No command entered.\n");
        }

        // Ryd input buffer for at undgå at gamle input påvirker de næste læsninger
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }

    // Frigiver hukommelsen for dækket og tableauet her
    // Her skal du have en funktion, der korrekt frigiver hele dækket og tableauet,
    // herunder alle CardNodes oprettet dynamisk.
    free(deck);
    free(tableau);

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
    //printf("INPUT > ");
}