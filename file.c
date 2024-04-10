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

    Node *current = deck->top->next; // Starter med det første faktiske kort
    while (current != NULL && !current->isDummy) { // Tjekker for dummy-node
        fprintf(file, "%c%c\n", current->card.value, current->card.suit);
        current = current->next;
    }

    fclose(file);
    return true;
}
Deck* loadDeckFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return NULL;
    }

    Deck *deck = malloc(sizeof(Deck));
    if (deck == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for deck.\n");
        fclose(file);
        return NULL;
    }

    // Initialiser deck med en dummy-node
    initializeDeck(deck);

    char cardStr[4]; // For at rumme 2 tegn + null terminator
    while (fgets(cardStr, sizeof(cardStr), file)) {
        // Fjern newline tegnet, hvis det findes
        cardStr[strcspn(cardStr, "\n")] = 0;

        if (strlen(cardStr) == 2 && isValidCard(cardStr)) {
            // Tilføjer kortet efter dummy-noden
            if (!addCard(deck, cardStr[0], cardStr[1])) {
                // Hvis tilføjelse af kort mislykkes, frigiv ressourcer og afslut
                freeDeck(deck);
                fclose(file);
                return NULL;
            }
        }
        else {
            // Ugyldigt kort fundet
            fprintf(stderr, "Error: Invalid card format in file.\n");
            freeDeck(deck);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return deck;
}

