//
// Created by david on 03-Apr-24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "deck.h"
void initializeDeck(Deck * deck){
    deck->head = NULL;
    deck->size = 0;
    char suits[] = {'C', 'D', 'H', 'S'};
    char value[] = {'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'A'};

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            CardNode *newNode = (CardNode*)malloc(sizeof(CardNode));
            if(newNode == NULL){
                fprintf(stderr, "Unable to allocate memory for new CardNode\n");
                exit(EXIT_FAILURE);
            }
            newNode->card.suit = suits[i];
            newNode->card.value = value[j];
            newNode->card.isHidden = true;
            newNode->next = deck->head;
            deck->head = newNode;
            deck->size++;

        }
    }
}
bool saveDeckToFile(const Deck *deck, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", filename);
        return false;
    }

    CardNode *current = deck->head;
    while (current != NULL) {
        fprintf(file, "%c%c\n", current->card.value, current->card.suit);
        current = current->next;
    }

    fclose(file);
    return true;
}

void shuffleDeck(Deck *deck) {
    if (deck->size < 2) return;  // Ingen grund til at blande, hvis der er 0 eller 1 kort

    // Opret et array til pointere af CardNode for nem adgang
    CardNode *nodesArray[deck->size];
    CardNode *current = deck->head;
    for (int i = 0; i < deck->size; i++) {
        nodesArray[i] = current;
        current = current->next;
    }

    // Brug Fisher-Yates shuffle algoritmen til at blande arrayet
    for (int i = deck->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        // Bytter elementer
        CardNode *temp = nodesArray[i];
        nodesArray[i] = nodesArray[j];
        nodesArray[j] = temp;
    }

    // Genopbyg listerne med de blandet pointere
    deck->head = nodesArray[0];
    current = deck->head;
    for (int i = 1; i < deck->size; i++) {
        current->next = nodesArray[i];
        current = current->next;
    }
    current->next = NULL;  // Sæt det sidste elements 'next' til NULL
}

bool isValidCard(const char *cardStr) {
    // Validerer et kort baseret på dit spils regler
    // Et eksempel på, hvad denne funktion kunne indeholde
    const char *validValues = "A23456789TJQK";
    const char *validSuits = "CDHS";

    // Tjek længden af kortet
    if (strlen(cardStr) != 2) {
        return false;
    }

    // Tjek om værdien er gyldig
    if (strchr(validValues, cardStr[0]) == NULL) {
        return false;
    }

    // Tjek om kuløren er gyldig
    if (strchr(validSuits, cardStr[1]) == NULL) {
        return false;
    }

    return true;
}

bool loadDeckFromFile(Deck *deck, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return false;
    }

    char cardStr[4]; // 2 characters for the card and 1 for the null-terminator
    int line = 0;
    bool success = true;

    while (fgets(cardStr, sizeof(cardStr), file) && line < 52) {
        if (cardStr[strlen(cardStr) - 1] == '\n') {
            cardStr[strlen(cardStr) - 1] = '\0'; // Remove newline character
        }

        if (!isValidCard(cardStr)) {
            fprintf(stderr, "Error: Invalid card at line %d.\n", line + 1);
            success = false;
            break;
        }

        if (!addCard(deck, cardStr[0], cardStr[1])) {
            success = false;
            break; // Afslut hvis der opstår en fejl ved tilføjelse af kort
        }

        line++;
    }

    if (line != 52) {
        fprintf(stderr, "Error: Incorrect number of cards. Expected 52, got %d.\n", line);
        success = false;
    }

    fclose(file);
    return success;
}
bool addCard(Deck *deck, const char value, const char suit) {
    // Opret et nyt kortnode
    CardNode *newCard = (CardNode*)malloc(sizeof(CardNode));
    if (newCard == NULL) {
        fprintf(stderr, "Unable to allocate memory for new CardNode\n");
        return false;
    }

    // Sæt værdierne for det nye kort
    newCard->card.value = value;
    newCard->card.suit = suit;
    newCard->next = NULL;

    // Tilføj det nye kort til toppen af dækket
    if (deck->head == NULL) { // Dækket er tomt
        deck->head = newCard;
    } else { // Dækket indeholder allerede kort
        newCard->next = deck->head;
        deck->head = newCard;
    }

    deck->size++; // Opdater størrelsen af dækket
    return true;
}