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
    Card card; //brug evt. en card pointer
    struct Node* prev;
    struct Node* next;
    bool isDummy;
} Node;

typedef struct Deck {
    Node *top;
    int size;
} Deck;


typedef struct Table {
    Node* columns[11];
} Table;


void initializeDeck(Deck *deck);
Deck* shuffleDeck(Deck* deck);
Table* initializeTable();
void dealToStartTable(Deck *deck, Table *table);
void printTable(Table *table, char command[256], char message[256]);
bool isValidCard(const char *cardStr);
bool addCard(Deck *deck, const char value, const char suit);
void setShowAllCards(Table *table, bool isVisible);
Deck* splitter(Deck* deck, int split);
void printDeck(Deck* deck);
#endif //YUKONPROJEKT_DECK_H
