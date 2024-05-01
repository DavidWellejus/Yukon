//
// Created by david on 04-Apr-24.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "deck.h"

bool saveDeckToFile(const Deck *deck, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", filename);
        return false;
    }

    Node *current = deck->top->next;
    while (current != NULL && !current->isDummy) {
        fprintf(file, "%c%c\n", current->card.value, current->card.suit);
        current = current->next;
    }

    fclose(file);
    return true;
}

Deck* loadDeckFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    Deck *deck = malloc(sizeof(Deck));
    if (deck == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for deck.\n");
        fclose(file);
        return NULL;
    }

    initializeDeck(deck);

    bool cardSeen[4][13] = {false};
    int cardCount = 0;
    char cardStr[4];
    while (fgets(cardStr, sizeof(cardStr), file)) {
        cardStr[strcspn(cardStr, "\n")] = 0;

        if (strlen(cardStr) == 2 && isValidCard(cardStr)) {
            int suitIndex = getSuitIndex(cardStr[1]);
            int rankIndex = getRankIndex(cardStr[0]);

            if (!cardSeen[suitIndex][rankIndex]) {
                cardSeen[suitIndex][rankIndex] = true;
                if (!addCard(deck, cardStr[0], cardStr[1])) {
                    fprintf(stderr, "Error: Unable to add card to deck.\n");
                    free(deck);
                    fclose(file);
                    return NULL;
                }
                cardCount++;
            } else {
                free(deck);
                fclose(file);
                return NULL;
            }
        } else {
            free(deck);
            fclose(file);
            return NULL;
        }
    }

    if (cardCount != 52) {
        free(deck);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return deck;
}

int getSuitIndex(char suit) {
    switch (suit) {
        case 'C': return 0;
        case 'D': return 1;
        case 'H': return 2;
        case 'S': return 3;
        default: return -1;
    }
}

int getRankIndex(char rank) {
    if (rank >= '2' && rank <= '9') return rank - '2';
    switch (rank) {
        case 'T': return 8;
        case 'J': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        case 'A': return 12;
        default: return -1;
    }
}
