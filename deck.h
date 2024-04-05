//
// Created by david on 03-Apr-24.
//

#include <stdbool.h>

#ifndef YUKONPROJEKT_DECK_H
#define YUKONPROJEKT_DECK_H

// Kortstruktur, som indeholder information om værdi, kulør og synlighed.
typedef struct Card {
    char value;    // 'A', '2'-'9', 'T', 'J', 'Q', 'K'
    char suit;     // 'C', 'D', 'H', 'S'
    bool isVisible;
} Card;

// En node i en linked liste, som holder på et kort.
typedef struct CardNode {
    Card card;
    struct CardNode* next;
} CardNode;

// En struktur som repræsenterer enten et fuldt dæk eller en kolonne i tableauet.
typedef struct Deck {
    CardNode *top;   // Peger på det øverste kort i dækket/kolonnen.
    int size;        // Antal kort i dækket/kolonnen.
} Deck;

typedef struct Tableau {
    Deck *columns[7]; // Array af pointers til Decks, som repræsenterer kolonner i tableauet.
} Tableau;

// Funktioner til at arbejde med dæk og kort.
void initializeDeck(Deck *deck);
void freeDeck(Deck *deck);
void shuffleDeck(Deck* deck);
Tableau* initializeTableau();
void dealToTableau(Deck *deck, Tableau *tableau);
void printTableau(Tableau *tableau);
bool isValidCard(const char *cardStr);
bool addCard(Deck *deck, const char value, const char suit);

#endif //YUKONPROJEKT_DECK_H
