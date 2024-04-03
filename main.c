#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "deck.h"
#define NUM_PILES 7 // Antal kortbunker i Yukon Solitaire



typedef struct {
    CardNode *piles[NUM_PILES]; // Array af pointers til toppen af hver kortbunke
    int size[NUM_PILES]; // Størrelsen af hver bunke, kan udelades hvis ikke nødvendigt
} Tableau;

Deck columns[7]; // Hver 'Deck' repræsenterer nu en kolonne


void printStartupScreen();
void initializeDeck(Deck *deck);
void printDeck(const Deck *deck);
bool saveDeckToFile(const Deck *deck, const char *filename);
void shuffleDeck(Deck *deck);
bool isValidCard(const char *cardStr);
bool loadDeckFromFile(Deck *deck, const char *filename);
void printHiddenDeck(Deck *deck);
bool addCard(Deck *deck, const char value, const char suit);
void initializeTableau(Tableau *tableau);
bool addCardToPile(Tableau *tableau, const char value, const char suit, int pileIndex, bool isHidden);
void printPile(CardNode *pile);
void printTableau(Tableau *tableau);
void freeTableau(Tableau *tableau);


int main() {
    Deck deck;

    initializeDeck(&deck);

    srand(time(NULL));
    shuffleDeck(&deck);
    printDeck(&deck);

    saveDeckToFile(&deck, "C:/DTU/2.Semester/02322MaskinaerProgrammering/Lab/project2_machine/Yukon/deckYukon.txt");
    printStartupScreen();
    char command[3];
    char filename[256];

    if(scanf("%2s %255s", command, filename) == 2){

        if(strcmp(command, "LD") == 0){
            loadDeckFromFile(&deck, "C:/DTU/2.Semester/02322MaskinaerProgrammering/Lab/project2_machine/Yukon/deckYukon.txt");

            printHiddenDeck(&deck);
        }
    }



    // Husk at frigive hukommelsen for dækket her
    return 0;
}


//Bare for at tjekke at dækket bliver lave korrekt.
void printDeck(const Deck *deck) {
    CardNode *current = deck->head;
    while (current != NULL) {
        printf("%c%c ", current->card.value, current->card.suit);
        current = current->next;
    }
    printf("\n");
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


void printHiddenDeck(Deck *deck) {
    // Antallet af kort der skal vises i hver kolonne.
    int cardsInColumns[] = {8, 8, 8, 7, 7, 7, 7};

    // Udskriver kolonneoverskrifterne
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");

    // Find det maksimale antal kort i en kolonne
    int maxRows = cardsInColumns[0]; // Start med antallet af kort i første kolonne
    int sideRow = 1;

    // Print de skjulte kort i hver kolonne og tilføj F1-F4 til højre
    for (int row = 0; row < maxRows; row++) {
        for (int col = 0; col < 7; col++) {
            if (row < cardsInColumns[col]) {
                printf("[ ]\t");
            } else {
                printf("\t");
            }
        }

        // Tilføj fundamenterne på samme linje, til højre
        if (row % 2 == 0) {
            printf("[ ] F%d", sideRow);
            sideRow += 1;
        }
        printf("\n");
    }
    printf("LAST Command: LD\n");
    printf("Message: OK\n");
    printf("INPUT > ");
}

// Initialiser hver bunke i tableau
void initializeTableau(Tableau *tableau) {
    for (int i = 0; i < NUM_PILES; i++) {
        tableau->piles[i] = NULL; // Starter med at alle bunkerne er tomme
        tableau->size[i] = 0;
    }
}

// Tilføj et kort til en specifik bunke
bool addCardToPile(Tableau *tableau, const char value, const char suit, int pileIndex, bool isHidden) {
    if (pileIndex < 0 || pileIndex >= NUM_PILES) {
        return false; // Invalid pile index
    }

    // Opret et nyt kortnode
    CardNode *newCard = (CardNode*)malloc(sizeof(CardNode));
    if (newCard == NULL) {
        fprintf(stderr, "Unable to allocate memory for new CardNode\n");
        return false;
    }

    // Sæt værdierne og status for det nye kort
    newCard->card.value = value;
    newCard->card.suit = suit;
    newCard->card.isHidden = isHidden;
    newCard->next = tableau->piles[pileIndex]; // Tilføj til toppen af bunken
    tableau->piles[pileIndex] = newCard; // Opdater bunke-top pointer
    tableau->size[pileIndex]++; // Opdater størrelsen af bunken

    return true;
}

// Denne funktion kunne bruges til at udskrive en enkelt bunke
void printPile(CardNode *pile) {
    CardNode *current = pile;
    while (current != NULL) {
        if (current->card.isHidden) {
            printf("[ ] ");
        } else {
            printf("%c%c ", current->card.value, current->card.suit);
        }
        current = current->next;
    }
    printf("\n");
}

// For at udskrive hele tableau
void printTableau(Tableau *tableau) {
    for (int i = 0; i < NUM_PILES; i++) {
        printf("C%d: ", i + 1);
        printPile(tableau->piles[i]);
    }
}

// Husk at frigøre hukommelsen for hver bunke, når du er færdig
void freeTableau(Tableau *tableau) {
    for (int i = 0; i < NUM_PILES; i++) {
        CardNode *current = tableau->piles[i];
        while (current != NULL) {
            CardNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
}