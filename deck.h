//
// Created by david on 03-Apr-24.
//

#include <stdbool.h>

#ifndef YUKON_DECK_H
#define YUKON_DECK_H

#endif //YUKON_DECK_H

typedef struct Card {
    char value; // A, 2-9, T, J, Q, K
    char suit;  // C, D, H, S
    bool isHidden;
} Card;

typedef struct CardNode {
    Card card;
    struct CardNode* next;
} CardNode;

typedef struct Deck {
    CardNode *head;
    int size;
} Deck;
