//
// Created by david on 04-Apr-24.
//
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

    CardNode *current = deck->top;
    while (current != NULL) {
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
    deck->top = NULL;
    deck->size = 0;

    char cardStr[4]; // 2 characters for the card and 1 for the null-terminator
    int line = 0;

    while (fgets(cardStr, sizeof(cardStr), file) && line < 52) {
        if (cardStr[strlen(cardStr) - 1] == '\n') {
            cardStr[strlen(cardStr) - 1] = '\0'; // Remove newline character
        }

        if (!isValidCard(cardStr)) {
            fprintf(stderr, "Error: Invalid card at line %d.\n", line + 1);
            free(deck);
            fclose(file);
            return NULL;
        }

        if (!addCard(deck, cardStr[0], cardStr[1])) {
            free(deck);
            fclose(file);
            return NULL; // Afslut hvis der opstår en fejl ved tilføjelse af kort
        }

        line++;
    }

    if (line != 52) {
        fprintf(stderr, "Error: Incorrect number of cards. Expected 52, got %d.\n", line);
        free(deck);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return deck; // Returner det fyldte dæk
}
