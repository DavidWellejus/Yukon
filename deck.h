//
// Created by david on 03-Apr-24.
//

#include <stdbool.h>

#ifndef YUKONPROJEKT_DECK_H
#define YUKONPROJEKT_DECK_H


typedef struct Card {
    char value;    // 'A', '2'-'9', 'T', 'J', 'Q', 'K'
    char suit;     // 'C', 'D', 'H', 'S'
    bool isVisible;
} Card;


typedef struct Node {
    Card card;
    struct Node* next;
} Node;

typedef struct Column {
    int numberOfCards;

}Column;

typedef struct Deck {
    Node *top;
    int size;
} Deck;


typedef struct Table {
    Deck *columns[11];
} Table;


void initializeDeck(Deck *deck);
void freeDeck(Deck *deck);
void freeTable(Table *table);
void shuffleDeck(Deck* deck);
Table* initializeTable();
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table);
bool isValidCard(const char *cardStr);
bool addCard(Deck *deck, const char value, const char suit);
void setShowAllCards(Table *table, bool isVisible);
Deck* splitter(Deck* deck, int split);
void printDeck(Deck* deck);
#endif //YUKONPROJEKT_DECK_H
